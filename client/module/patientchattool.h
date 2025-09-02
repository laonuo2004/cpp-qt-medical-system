#ifndef PATIENTCHATTOOL_H
#define PATIENTCHATTOOL_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QMessageBox>
#include "uicontroller.h"
#include <QTimer>

namespace Ui {
class PatientChatTool;
}

class PatientChatTool : public QDialog
{
    Q_OBJECT

public:
    explicit PatientChatTool(QWidget *parent = nullptr);
    PatientChatTool(QWidget *parent, int patientid , int doctorid);
    ~PatientChatTool();

private:
    Ui::PatientChatTool *ui;
    int m_patientid;
    int m_doctorid;
    UiController* controller;
    QTimer* m_refreshTimer = nullptr;

    void displayMessage(int senderId, int receiverId, const QString &message);

private slots:
    void  sendMessage() ;

private slots:
    void loadChatHistory();  // 拉取并展示最新聊天记录

};

#endif // PATIENTCHATTOOL_H
