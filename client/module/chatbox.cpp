#include "chatbox.h"
#include "ui_chatbox.h"
#include "chatbubbleleft.h"
#include "chatbubbleright.h"
#include "uicontroller.h"

#include <QScrollBar>
#include <QMessageBox>
#include <QLayoutItem>

ChatBox::ChatBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatBox),
    m_controller(new UiController(this))   // 挂到 this，避免泄漏
{
    ui->setupUi(this);
    connect(ui->sendBtn, &QPushButton::clicked, this, &ChatBox::sendMsg);

    m_scrollBar = ui->scrollArea->verticalScrollBar();

    // 如果未设置会话双方，这里不会拉取（loadHistoryMsg里有保护）
    loadHistoryMsg();

    // 定时器：每3秒增量拉新
    m_refreshTimer = new QTimer(this);
    m_refreshTimer->setInterval(3000);
    connect(m_refreshTimer, &QTimer::timeout, this, &ChatBox::pullNewMessages);
    m_refreshTimer->start();

    if (m_scrollBar) m_scrollBar->setValue(m_scrollBar->maximum());
}

ChatBox::ChatBox(QWidget *parent , int patientid , int doctorid, bool isPatientClient) :
    QWidget(parent),
    ui(new Ui::ChatBox),
    m_isPatientClient(isPatientClient),
    m_patientid(patientid),
    m_doctorid(doctorid),
    m_controller(new UiController(this))
{
    ui->setupUi(this);
    connect(ui->sendBtn, &QPushButton::clicked, this, &ChatBox::sendMsg);

    m_scrollBar = ui->scrollArea->verticalScrollBar();

    if (isPatientClient)
    {
        setLeftAvatar(":/img/picture/doctorAvatar.png");
        setRightAvatar(":/img/picture/patientAvatar.png");
    }
    else
    {
        setLeftAvatar(":/img/picture/patientAvatar.png");
        setRightAvatar(":/img/picture/doctorAvatar.png");
    }


    loadHistoryMsg();

    m_refreshTimer = new QTimer(this);
    m_refreshTimer->setInterval(3000);
    connect(m_refreshTimer, &QTimer::timeout, this, &ChatBox::pullNewMessages);
    m_refreshTimer->start();

    if (m_scrollBar) m_scrollBar->setValue(m_scrollBar->maximum());
}

ChatBox::~ChatBox()
{
    delete ui;
}

void ChatBox::setLeftAvatar(const QString& LeftAvatarUrl)
{
    m_leftAvatarUrl = LeftAvatarUrl;
}

void ChatBox::setRightAvatar(const QString& RightAvatarUrl)
{
    m_rightAvatarUrl = RightAvatarUrl;
}

void ChatBox::newMsg(const QString& msg, bool isSelf)
{
    if (isSelf) {
        auto* bubble = new ChatBubbleRight(this);
        bubble->buildUpBubble(msg, m_rightAvatarUrl);
        ui->chatList->addWidget(bubble);
    } else {
        auto* bubble = new ChatBubbleLeft(this);
        bubble->buildUpBubble(msg, m_leftAvatarUrl);
        ui->chatList->addWidget(bubble);
    }
}

QDateTime ChatBox::parseTimestamp(const QVariant& v) const
{
    if (v.canConvert<QDateTime>()) {
        return v.toDateTime();
    }
    const QString s = v.toString();
    QDateTime ts = QDateTime::fromString(s, Qt::ISODate);
    if (ts.isValid()) return ts;
    ts = QDateTime::fromString(s, "yyyy-MM-dd hh:mm:ss");
    return ts;
}

// 历史/轮询获取的消息统一走这里：全部显示为“左侧气泡”。
// 若该条消息是本地刚发过的（文本相同），则消耗待去重计数并跳过插入。
void ChatBox::newMsg(const QVariantMap& row)
{
    const int senderId     = row.value("sender_id").toInt();
    const QString content  = row.value("content").toString();
    const qint64 messageId = row.value("message_id").toLongLong();

    // 1) message_id 去重
    if (messageId != 0 && m_seenMsgIds.contains(messageId)) {
        return;
    }

    // 2) 若这条来自“我自己”，且本地刚刚已经插了右侧气泡，则轮询回来时跳过
    // 约定：m_patientid = 本端用户ID（自己）
    if (senderId == m_patientid) {
        auto it = m_pendingSelfMsgCounts.find(content);
        if (it != m_pendingSelfMsgCounts.end() && it.value() > 0) {
            // 消耗一次待去重计数，同时登记已见ID
            it.value()--;
            if (it.value() == 0) m_pendingSelfMsgCounts.erase(it);

            if (messageId != 0) {
                m_seenMsgIds.insert(messageId);
                if (messageId > m_lastMessageId) m_lastMessageId = messageId;
            }
            return; // 跳过左侧再插一遍
        }
    }

    // 3) 历史消息/对方消息/非待去重的自发消息 —— 根据 senderId 判断左右
    if (m_isPatientClient)
    {
        const bool isSelf = (senderId == m_patientid);
        newMsg(content, isSelf);
    }
    else
    {
        const bool isSelf = (senderId == m_doctorid);
        newMsg(content, isSelf);
    }

    // 4) 记录已见ID与最大ID
    if (messageId != 0) {
        m_seenMsgIds.insert(messageId);
        if (messageId > m_lastMessageId) m_lastMessageId = messageId;
    }
}

void ChatBox::loadHistoryMsg()
{
    if (!m_controller) return;
    if (m_patientid <= 0 || m_doctorid <= 0) return;

    const QVariantList list = m_controller->getChatHistory(m_patientid, m_doctorid);

    // 清空旧气泡
    while (QLayoutItem* item = ui->chatList->takeAt(0)) {
        if (auto* w = item->widget()) w->deleteLater();
        delete item;
    }

    // 重置去重集合与最大 id；本地未入库消息计数也清空
    m_seenMsgIds.clear();
    m_pendingSelfMsgCounts.clear();
    m_lastMessageId = -1;

    // DB 常为 sent_at DESC，这里倒序遍历以正序显示
    for (int i = list.size() - 1; i >= 0; --i) {
        newMsg(list[i].toMap());   // 统一入口
    }

    if (m_scrollBar) m_scrollBar->setValue(m_scrollBar->maximum());
}

void ChatBox::pullNewMessages()
{
    if (!m_controller) return;
    if (m_patientid <= 0 || m_doctorid <= 0) return;

    const QVariantList list = m_controller->getChatHistory(m_patientid, m_doctorid);

    bool appended = false;
    for (int i = list.size() - 1; i >= 0; --i) {
        const qint64 before = m_lastMessageId;
        newMsg(list[i].toMap());     // 统一入口（内部去重）
        if (m_lastMessageId > before) appended = true;
    }

    if (appended && m_scrollBar) {
        const bool atBottom = (m_scrollBar->value() >= m_scrollBar->maximum() - 2);
        if (atBottom) m_scrollBar->setValue(m_scrollBar->maximum());
    }
}

void ChatBox::sendMsg()
{
    const QString msg = ui->msgInput->toPlainText().trimmed();
    if (msg.isEmpty()) return;
    if (!m_controller) return;
    if (m_patientid <= 0 || m_doctorid <= 0) {
        QMessageBox::warning(this, "发送失败", "会话双方ID未设置");
        return;
    }
    if (msg.length() > 500) {
        QMessageBox::warning(this, "输入错误", "消息内容过长（最多500个字符）");
        return;
    }

    const bool ok = m_controller->sendMessage(m_patientid, m_doctorid, msg);
    if (!ok) {
        QMessageBox::warning(this, "发送失败", "消息发送失败，请稍后重试");
        return;
    }

    // 本地立即插入右侧气泡（自己）
    newMsg(msg, /*isSelf=*/true);

    // 登记到“待去重表”：等轮询拿到同一文本的DB记录时跳过
    m_pendingSelfMsgCounts[msg] += 1;

    ui->msgInput->clear();

    // 立刻拉新：同时也能拿到对方刚回的消息
    pullNewMessages();

    if (m_scrollBar) m_scrollBar->setValue(m_scrollBar->maximum());
}
