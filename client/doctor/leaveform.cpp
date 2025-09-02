#include "leaveform.h"
#include "ui_leaveform.h"
#include "uicontroller.h"

LeaveForm::LeaveForm(QWidget *parent, int doctorId) :
    QDialog(parent),
    ui(new Ui::LeaveForm),
    m_doctorId(doctorId)
{
    ui->setupUi(this);
    setFixedWidth(500);
    connect(ui->SubmitBtn, &QPushButton::clicked, this, &LeaveForm::submit);
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

void LeaveForm::submit()
{
    if (UiController::get().submitLeaveRequest
    (
        QString::number(m_doctorId),
        ui->LeaveTypeInput->currentText(),
        ui->StartDateInput->date(),
        ui->EndDateInput->date(),
        ui->LeaveReasonInput->toPlainText()
    ))
    {
        emit accept();
    }
    else
    {
        emit reject();
    }
}
