#include "chatbox.h"
#include "ui_chatbox.h"
#include "chatbubbleleft.h"
#include "chatbubbleright.h"

#include <QScrollBar>

ChatBox::ChatBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatBox)
{
    ui->setupUi(this);

    connect(ui->sendBtn, &QPushButton::clicked, this, &ChatBox::sendMsg);
    m_scrollBar = ui->scrollArea->verticalScrollBar();

    loadHistoryMsg();

    newMsg("在线吗，胡医生？", false);
    newMsg("在的！", true);

    m_scrollBar->setValue(m_scrollBar->maximum());
}

ChatBox::~ChatBox()
{
    delete ui;
}

void ChatBox::newMsg(const QString& msg, bool isSelf)
{
    if (isSelf)
    {
        ChatBubbleRight* chatBubble = new ChatBubbleRight(this);
        chatBubble->buildUpBubble(msg, m_rightAvatarUrl);
        ui->chatList->addWidget(chatBubble);

    }
    else
    {
        ChatBubbleLeft* chatBubble = new ChatBubbleLeft(this);
        chatBubble->buildUpBubble(msg, m_leftAvatarUrl);
        ui->chatList->addWidget(chatBubble);
    }
}

void ChatBox::loadHistoryMsg()
{
    // 加载历史消息，从数据库中获取历史消息，调用newMsg发送。
}

void ChatBox::setLeftAvatar(const QString& LeftAvatarUrl)
{
    m_leftAvatarUrl = LeftAvatarUrl;
}

void ChatBox::setRightAvatar(const QString& RightAvatarUrl)
{
    m_rightAvatarUrl = RightAvatarUrl;
}

void ChatBox::sendMsg()
{
    QString msg = ui->msgInput->toPlainText();
    if (msg == "")
    {
        return;
    }
    newMsg(msg, true);

    m_scrollBar->setValue(m_scrollBar->maximum());
    ui->msgInput->clear();
}
