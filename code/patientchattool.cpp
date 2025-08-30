#include "patientchattool.h"
#include "ui_patientchattool.h"

PatientChatTool::PatientChatTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientChatTool)
{
    ui->setupUi(this);
    init() ;
}

PatientChatTool::~PatientChatTool()
{
    delete ui;
}

//functions
void PatientChatTool::init()
{
    connectStatus = false;//默认连接状态为false
    myTcpClient = new QTcpSocket(this);//使用当前对象为父对象,以备释放空间
    myTcpServer = new QTcpServer(this);
    this->setWindowTitle("我是客户端");
    ui->tips->setDisabled(true);//禁止写入
}
//end functions

//slots
//当点击连接到服务器按钮时
void PatientChatTool::on_connectServerBtn_clicked(){
    //获取要连接到服务器的ip和port
    QString servIp = ui->serverIP->text();
    QString servPort = ui->serverPort->text();
    //myClient->connectToHost(QHostAddress(servIp),servPort.toUInt());//没有返回值,不知道是否连接成功
    //连接到服务器,默认模式是读写模式
    myTcpClient->connectToHost(QHostAddress(servIp),servPort.toInt());
    //因为连接没有返回值,所以使用信号和槽方式检测连接状态
    connect(myTcpClient,SIGNAL(connected()),this,SLOT(slotConnected()));
    //读取服务器发送过来的数据
    connect(myTcpClient,SIGNAL(readyRead()),this,SLOT(slotReadyRead()));
    //断开连接
    connect(myTcpClient,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
    //连接错误
    connect(myTcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotError(QAbstractSocket::SocketError)));
}
//连接到服务器
void PatientChatTool::slotConnected()
{
    QString msg = QString("连接服务器[%1:%2]成功").
            arg(myTcpClient->peerAddress().toString()).
            arg(myTcpClient->peerPort());
    ui->tips->append(msg);//处理显示信息
    connectStatus = true;//设置连接状态为true(已连接)
    //ui->connectServerBtn->setText(tr("disconnect"));//连接成功设置按钮文字
    ui->connectServerBtn->setEnabled(false);
}
//读取服务器发送的信息
void PatientChatTool::slotReadyRead()
{
   QByteArray msgContent;
   //循环到缓冲区结束
   while(!myTcpClient->atEnd()){
       msgContent = myTcpClient->readAll();
   }
   QString msg =  QString("服务器[%1:%2]回应:%3").
           arg(myTcpClient->peerAddress().toString()).
           arg(myTcpClient->peerPort()).
           arg(QString(msgContent));
   ui->tips->append(msg);//处理显示信息
}
//断开和服务器之间的连接
void PatientChatTool::slotDisconnected()
{
    QString msg =  QString("服务器[%1:%2]已关闭:").
            arg(myTcpClient->peerAddress().toString()).
            arg(myTcpClient->peerPort());
    ui->tips->append(msg);
    ui->connectServerBtn->setEnabled(true);//改变按钮状态
}
//连接错误时的处理
void PatientChatTool::slotError(QAbstractSocket::SocketError error)
{
    qDebug() << "slotError(QAbstractSocket::SocketError error)" << error;
}

//点击发送信息按钮时
void PatientChatTool::on_SendBtn_clicked()
{
    QString msg = ui->message->toPlainText();
    int ret= myTcpClient->write(msg.toUtf8());//防止乱码产生
    if(ret <=0){//如果没有发送信息可以报错
        QMessageBox::warning(this,tr("错误"),tr("没有信息写入"));
        return ;
    }
    ui->tips->append(msg);//追加信息
    ui->message->clear();//清空输入框
}
