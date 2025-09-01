#ifndef PATIENTCHATTOOL_H
#define PATIENTCHATTOOL_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QMessageBox>
#include "uicontroller.h"

namespace Ui {
class PatientChatTool;
}

class PatientChatTool : public QDialog
{
    Q_OBJECT

public:
    explicit PatientChatTool(QWidget *parent = nullptr);
    ~PatientChatTool();

private:
    Ui::PatientChatTool *ui;
    UiController* controller;
    void displayMessage(int senderId, const QString &message);

private slots:
    void  sendMessage() ;

};

#endif // PATIENTCHATTOOL_H
