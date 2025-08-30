#include "doctorattendancewidget.h"
#include "ui_doctorattendancewidget.h"
#include "doctorleaveitem.h"
#include "leaveform.h"

#include <QDateTime>
#include <QListWidgetItem>

DoctorAttendanceWidget::DoctorAttendanceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DoctorAttendanceWidget)
{
    ui->setupUi(this);
    connect(ui->SignInBtn, &QPushButton::clicked, this, &DoctorAttendanceWidget::signIn);
    connect(ui->SignOutBtn, &QPushButton::clicked, this, &DoctorAttendanceWidget::signOut);
    connect(ui->LeaveBtn, &QPushButton::clicked, this, &DoctorAttendanceWidget::RequestLeave);

    // 读取数据库中的签到记录并显示
    // 检查数据库中最新的签到，如果事件为“今天”，则禁用签到按钮
}

DoctorAttendanceWidget::~DoctorAttendanceWidget()
{
    delete ui;
}

void DoctorAttendanceWidget::signIn()
{
    QListWidgetItem* signInRecord = new QListWidgetItem;
    signInRecord->setText(QDateTime::currentDateTime().toString());
    ui->SignInList->addItem(signInRecord);
    ui->SignInBtn->setDisabled(true);
}

void DoctorAttendanceWidget::signOut()
{
    QListWidgetItem* signOutRecord = new QListWidgetItem;
    signOutRecord->setText(QDateTime::currentDateTime().toString());
    ui->SignOutList->addItem(signOutRecord);
    ui->SignOutBtn->setDisabled(true);
}

void DoctorAttendanceWidget::RequestLeave()
{
    LeaveForm* newLeaveForm = new LeaveForm(this);
    if (newLeaveForm->exec() == QDialog::Accepted)
    {
        QString LeaveMsg = "无记录";
        newLeaveForm->getLeaveMsg(LeaveMsg);
        DoctorLeaveItem* newLeaveItem = new DoctorLeaveItem(this);
        newLeaveItem->setupLeaveItem(LeaveMsg);
        ui->LeaveList->addWidget(newLeaveItem);
    }
}

void DoctorAttendanceWidget::CancelLeave()
{

}
