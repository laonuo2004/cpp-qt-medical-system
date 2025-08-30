#include "doctorleaveitem.h"
#include "ui_doctorleaveitem.h"

DoctorLeaveItem::DoctorLeaveItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DoctorLeaveItem)
{
    ui->setupUi(this);
    connect(ui->CancelBtn, &QPushButton::clicked, this, &DoctorLeaveItem::cancelLeaveRequset);
}

void DoctorLeaveItem::setupLeaveItem(QString& LeaveMsg)
{
    ui->LeaveMsg->setText(LeaveMsg);
}

void DoctorLeaveItem::cancelLeaveRequset()
{
    ui->CancelBtn->setDisabled(true);
    ui->LeaveMsg->setText("[已销假] " + ui->LeaveMsg->text());
    // 数据库操作，标记请假已经消除
}

DoctorLeaveItem::~DoctorLeaveItem()
{
    delete ui;
}
