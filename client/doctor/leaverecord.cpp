#include "leaverecord.h"
#include "ui_leaverecord.h"

LeaveRecord::LeaveRecord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LeaveRecord)
{
    ui->setupUi(this);
    connect(ui->CancelBtn, &QPushButton::clicked, this, &LeaveRecord::cancelLeaveRequest);
}

void LeaveRecord::setupLeaveItem(QString& LeaveMsg)
{
    ui->LeaveMsg->setText(LeaveMsg);
}

void LeaveRecord::cancelLeaveRequest()
{
    ui->CancelBtn->setDisabled(true);
    ui->LeaveMsg->setText("[已销假] " + ui->LeaveMsg->text());
    // 数据库操作，标记请假已经消除
}

LeaveRecord::~LeaveRecord()
{
    delete ui;
}
