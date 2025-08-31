#include "patientchattool.h"
#include "ui_patientchattool.h"

PatientChatTool::PatientChatTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientChatTool)
{
    ui->setupUi(this);
    controller = new UiController(this);// 依赖注入
    connect(ui->SendBtn , &QPushButton::clicked , this , &PatientChatTool::sendMessage) ;
}

PatientChatTool::~PatientChatTool()
{
    delete ui;
}

void PatientChatTool::sendMessage()
{
    // 调用uiController接口而不是直接Socket
    int senderId = ui->senderID->text().toInt() ;
    int receiverId = ui->ReceiverID->text().toInt();
    QString message = ui->Message->text();
    controller->sendMessage(senderId, receiverId, message);
    displayMessage(senderId , message);
}

void PatientChatTool::displayMessage(int senderId, const QString &message)
{
    // 格式化消息显示
    QString formattedMessage;
    formattedMessage = QString(message);

    // 添加到消息框
    ui->MessageBox->append(formattedMessage);
}
//end functions

