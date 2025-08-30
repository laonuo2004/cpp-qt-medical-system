#ifndef PATIENTCHATTOOL_H
#define PATIENTCHATTOOL_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class PatientChatTool;
}

class PatientChatTool : public QWidget
{
    Q_OBJECT

public:
    explicit PatientChatTool(QWidget *parent = nullptr);
    ~PatientChatTool();

private:
    Ui::PatientChatTool *ui;
    QTcpSocket * myTcpClient;//当前客户端
    QTcpServer * myTcpServer;//对应的服务器
    bool connectStatus;//当前客户端的连接状态
    void init();//初始化函数

private slots:
    void on_connectServerBtn_clicked();
    void on_SendBtn_clicked();

    void slotConnected();
    void slotReadyRead();
    void slotDisconnected();
    void slotError(QAbstractSocket::SocketError error);
};

#endif // PATIENTCHATTOOL_H
