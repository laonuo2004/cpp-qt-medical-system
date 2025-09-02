#ifndef DOCTORCHATTOOL_H
#define DOCTORCHATTOOL_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include <QIntValidator>
#include "uicontroller.h"
#include <QTimer>

namespace Ui {
class DoctorChatTool;
}

class DoctorChatTool : public QDialog
{
    Q_OBJECT

public:
    explicit DoctorChatTool(QWidget *parent = nullptr);
    DoctorChatTool(QWidget *parent , int patientid ,int doctorid);
    ~DoctorChatTool();

private slots:

private:
    Ui::DoctorChatTool *ui;
    int m_patientid;
    int m_doctorid;
    UiController* controller;
    QTimer* m_refreshTimer = nullptr;  // 新增：定时器

    void displayMessage(int senderId, int receiverId, const QString &message);
    void loadChatHistory();   // 新增：拉取并展示最新聊天记录#include

private slots:
    void  sendMessage() ;

};

#endif // DOCTORCHATTOOL_H
