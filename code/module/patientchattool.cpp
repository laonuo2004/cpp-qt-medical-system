#include "patientchattool.h"
#include "ui_patientchattool.h"
#include <QMessageBox>
#include <QDebug>
#include <QRegularExpression>

PatientChatTool::PatientChatTool(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PatientChatTool),
    controller(new UiController(this)) // 使用初始化列表初始化controller
{
    ui->setupUi(this);
    connect(ui->SendBtn, &QPushButton::clicked, this, &PatientChatTool::sendMessage);
}

PatientChatTool::~PatientChatTool()
{
    delete ui;
    // controller由Qt父对象系统自动管理，无需手动删除
}

void PatientChatTool::sendMessage()
{
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
        bool success = controller->sendMessage(senderId, receiverId, message);

        if (success) {
            // 7. 显示发送的消息
            displayMessage(senderId, message);

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

void PatientChatTool::displayMessage(int senderId, const QString &message)
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
    formattedMessage = QString("[%1] 用户%2: %3")
                          .arg(timestamp)
                          .arg(senderId)
                          .arg(message);

    // 3. 添加到消息框
    ui->MessageBox->append(formattedMessage);

    // 4. 自动滚动到底部
    QTextCursor cursor = ui->MessageBox->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->MessageBox->setTextCursor(cursor);
}

