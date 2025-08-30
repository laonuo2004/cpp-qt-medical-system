#include "attendancepage.h"
#include "ui_attendancepage.h"
#include "leaverecord.h"
#include "leaveform.h"

#include <QDateTime>
#include <QListWidgetItem>

AttendancePage::AttendancePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AttendancePage)
{
    ui->setupUi(this);
    connect(ui->SignInBtn, &QPushButton::clicked, this, &AttendancePage::signIn);
    connect(ui->SignOutBtn, &QPushButton::clicked, this, &AttendancePage::signOut);
    connect(ui->LeaveBtn, &QPushButton::clicked, this, &AttendancePage::RequestLeave);

    // 读取数据库中的签到记录并显示
    // 检查数据库中最新的签到，如果事件为“今天”，则禁用签到按钮
}

AttendancePage::~AttendancePage()
{
    delete ui;
}

void AttendancePage::signIn()
{
    QListWidgetItem* signInRecord = new QListWidgetItem;
    signInRecord->setText(QDateTime::currentDateTime().toString());
    ui->SignInList->addItem(signInRecord);
    ui->SignInBtn->setDisabled(true);
}

void AttendancePage::signOut()
{
    QListWidgetItem* signOutRecord = new QListWidgetItem;
    signOutRecord->setText(QDateTime::currentDateTime().toString());
    ui->SignOutList->addItem(signOutRecord);
    ui->SignOutBtn->setDisabled(true);
}

void AttendancePage::RequestLeave()
{
    LeaveForm* newLeaveForm = new LeaveForm(this);
    if (newLeaveForm->exec() == QDialog::Accepted)
    {
        QString LeaveMsg = "无记录";
        newLeaveForm->getLeaveMsg(LeaveMsg);
        LeaveRecord* newLeaveItem = new LeaveRecord(this);
        newLeaveItem->setupLeaveItem(LeaveMsg);
        ui->LeaveList->addWidget(newLeaveItem);
    }
    delete newLeaveForm;
}

