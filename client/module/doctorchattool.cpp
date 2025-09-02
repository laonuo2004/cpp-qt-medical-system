#include "doctorchattool.h"
#include "ui_doctorchattool.h"
#include "uicontroller.h"

DoctorChatTool::DoctorChatTool(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DoctorChatTool),
  controller(new UiController(this)) // 使用初始化列表初始化controller
{
    ui->setupUi(this);
    connect(ui->SendBtn, &QPushButton::clicked, this, &DoctorChatTool::sendMessage);
    // 启动时先拉一次
        loadChatHistory();

        // === 定时刷新 ===
        m_refreshTimer = new QTimer(this);
        m_refreshTimer->setInterval(3000); // 每3秒刷新一次
        connect(m_refreshTimer, &QTimer::timeout, this, &DoctorChatTool::loadChatHistory);
        m_refreshTimer->start();
}

DoctorChatTool::DoctorChatTool(QWidget *parent, int patientid , int doctorid) :
    QDialog(parent),
    ui(new Ui::DoctorChatTool),
    m_patientid(patientid),
    m_doctorid(doctorid),
    controller(new UiController(this)) // 使用初始化列表初始化controller
{
    ui->setupUi(this);
    connect(ui->SendBtn, &QPushButton::clicked, this, &DoctorChatTool::sendMessage);
    loadChatHistory();

    // === 定时刷新 ===
    m_refreshTimer = new QTimer(this);
    m_refreshTimer->setInterval(3000); // 每3秒刷新一次
    connect(m_refreshTimer, &QTimer::timeout, this, &DoctorChatTool::loadChatHistory);
    m_refreshTimer->start();
}

DoctorChatTool::~DoctorChatTool()
{
    delete ui;
}

void DoctorChatTool::sendMessage()
{
    /*
    // 1. 验证发送者ID
    bool conversionOk = false;
    int senderId = ui->senderID->text().toInt(&conversionOk);
    if (!conversionOk || senderId <= 0) {
        QMessageBox::warning(this, "输入错误", "请输入有效的发送者ID（正整数）");
        ui->senderID->setFocus();
        ui->senderID->selectAll();
        return;
    }

    // 2. 验证接收者ID
    int receiverId = ui->ReceiverID->text().toInt(&conversionOk);
    if (!conversionOk || receiverId <= 0) {
        QMessageBox::warning(this, "输入错误", "请输入有效的接收者ID（正整数）");
        ui->ReceiverID->setFocus();
        ui->ReceiverID->selectAll();
        return;
    }
    */

    // 3. 验证消息内容
    QString message = ui->Message->text().trimmed();
    if (message.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "消息内容不能为空");
        ui->Message->setFocus();
        return;
    }

    // 4. 检查消息长度
    if (message.length() > 500) {
        QMessageBox::warning(this, "输入错误", "消息内容过长（最多500个字符）");
        ui->Message->setFocus();
        ui->Message->selectAll();
        return;
    }

    // 5. 检查控制器是否有效
    if (!controller) {
        QMessageBox::critical(this, "系统错误", "控制器未正确初始化");
        return;
    }

    try {
        // 6. 尝试发送消息
        bool success = controller->sendMessage(m_patientid, m_doctorid, message);

        if (success) {
            // 7. 显示发送的消息
            displayMessage(m_patientid, m_doctorid , message);

            // 8. 清空消息输入框
            ui->Message->clear();

            // 9. 可选：显示发送成功提示
            QMessageBox::information(this, "发送成功", "消息已成功发送");
        } else {
            QMessageBox::warning(this, "发送失败", "消息发送失败，请稍后重试");
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "发送错误",
                             QString("发送消息时发生异常: %1").arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "发送错误", "发送消息时发生未知异常");
    }
}

void DoctorChatTool::displayMessage(int senderId, int receiverId, const QString &message)
{
    // 1. 检查消息显示控件是否有效
    if (!ui->MessageBox) {
        qWarning() << "MessageBox widget is not initialized";
        return;
    }

    // 2. 格式化消息显示
    QString formattedMessage;

    // 添加时间戳
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timestamp = currentTime.toString("yyyy-MM-dd hh:mm:ss");

    // 格式化消息
    formattedMessage = QString("[%1] 用户%2 发送消息 %3 接收者%4")
                          .arg(timestamp)
                          .arg(senderId)
                          .arg(message)
                          .arg(receiverId);

    // 3. 添加到消息框
    ui->MessageBox->append(formattedMessage);

    // 4. 自动滚动到底部
    QTextCursor cursor = ui->MessageBox->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->MessageBox->setTextCursor(cursor);
}

void DoctorChatTool::loadChatHistory()
{
    if (!ui->MessageBox) {
        qWarning() << "MessageBox widget is not initialized";
        return;
    }
    if (!controller) {
        qWarning() << "UiController is not initialized";
        return;
    }

    // 取 doctor 与 patient 的会话（与患者端一致）
    QVariantList list = controller->getChatHistory(m_patientid, m_doctorid);

    ui->MessageBox->clear();

    // DB 按 sent_at DESC(最新在前)；为了最新显示在底部，这里倒序追加
    for (int i = list.size() - 1; i >= 0; --i) {
        const QVariantMap m = list[i].toMap();

        const int senderId   = m.value("sender_id").toInt();
        const int receiverId = m.value("receiver_id").toInt();
        const QString content = m.value("content").toString();

        const QString senderName   = m.value("sender_name").toString().isEmpty()
                ? QStringLiteral("用户%1").arg(senderId)
                : m.value("sender_name").toString();
        const QString receiverName = m.value("receiver_name").toString().isEmpty()
                ? QStringLiteral("用户%1").arg(receiverId)
                : m.value("receiver_name").toString();

        QDateTime ts;
        const QVariant sentVar = m.value("sent_at");
        if (sentVar.canConvert<QDateTime>()) {
            ts = sentVar.toDateTime();
        } else {
            ts = QDateTime::fromString(sentVar.toString(), Qt::ISODate);
            if (!ts.isValid())
                ts = QDateTime::fromString(sentVar.toString(), "yyyy-MM-dd hh:mm:ss");
        }
        const QString timestamp = ts.isValid()
                ? ts.toString("yyyy-MM-dd hh:mm:ss")
                : QStringLiteral("时间未知");

        const QString line = QString("[%1] %2(%3) → %4(%5): %6")
                                 .arg(timestamp)
                                 .arg(senderName).arg(senderId)
                                 .arg(receiverName).arg(receiverId)
                                 .arg(content);

        ui->MessageBox->append(line);
    }

    // 滚到底
    QTextCursor cursor = ui->MessageBox->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->MessageBox->setTextCursor(cursor);
}
