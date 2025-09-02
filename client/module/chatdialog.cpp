#include "chatdialog.h"
#include "ui_chatdialog.h"
#include "chatbox.h"

ChatDialog::ChatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatDialog)
{
    ui->setupUi(this);
    ui->mainPanel->addWidget(new ChatBox(this));
}

ChatDialog::ChatDialog(QWidget *parent , int patientid , int doctorid, bool isPatientClient):
    QDialog(parent),
    ui(new Ui::ChatDialog),
    m_patientid(patientid),
    m_doctorid(doctorid)
{
    ui->setupUi(this);
    ui->mainPanel->addWidget(new ChatBox(this , 19 ,12, isPatientClient));
}

ChatDialog::~ChatDialog()
{
    delete ui;
}
