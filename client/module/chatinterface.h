#ifndef CHATINTERFACE_H
#define CHATINTERFACE_H

#include <QWidget>
#include "chattool.h"
#include "doctorchattool.h"
#include "patientchattool.h"
#include "chatfunc.h"
#include <QStackedWidget>

namespace Ui {
class ChatInterface;
}

class ChatInterface : public QWidget
{
    Q_OBJECT

public:
    explicit ChatInterface(QWidget *parent = nullptr);
    ~ChatInterface();

private:
    Ui::ChatInterface *ui;

    QStackedWidget* stack_;
    chatfunc*     chat ;
    chattool*     Chatpage;
    DoctorChatTool* DC;
    PatientChatTool* PC;

};

#endif // CHATINTERFACE_H
