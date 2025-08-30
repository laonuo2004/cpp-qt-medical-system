#include "leaveform.h"
#include "ui_leaveform.h"

LeaveForm::LeaveForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LeaveForm)
{
    ui->setupUi(this);
    setFixedWidth(500);
    connect(ui->SubmitBtn, &QPushButton::clicked, this, &QDialog::accept);
}

void LeaveForm::getLeaveMsg(QString& leaveMsg)
{
    leaveMsg = QString("请假性质: %1 | 开始日期: %2 | 结束日期: %3")
        .arg(ui->LeaveTypeInput->currentText())
        .arg(ui->StartDateInput->text())
        .arg(ui->EndDateInput->text());
}

LeaveForm::~LeaveForm()
{
    delete ui;
}
