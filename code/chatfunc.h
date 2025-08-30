#ifndef CHATFUNC_H
#define CHATFUNC_H

#include <QWidget>
#include "chattool.h"
#include "doctorchattool.h"
#include "patientchattool.h"

namespace Ui {
class chatfunc;
}

class chatfunc : public QWidget
{
    Q_OBJECT

public:
    explicit chatfunc(QWidget *parent = nullptr);
    ~chatfunc();


private slots:
    void showChatPage();
    void showDocChatPage() ;
    void showPatientChatPage() ;

private:
    Ui::chatfunc *ui;

    chattool *Chatpage;

    DoctorChatTool *DC;
    PatientChatTool *PC;

    QWidget *previousPage;
    QWidget *currentPage;

    void switchPage(QWidget *page);

};

#endif // CHATFUNC_H
