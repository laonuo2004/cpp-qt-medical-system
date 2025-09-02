#include "attendancepage.h"
#include "ui_attendancepage.h"
#include "leaverecord.h"
#include "leaveform.h"
#include "uicontroller.h"

#include <QDateTime>
#include <QListWidgetItem>
#include <QDebug>

AttendancePage::AttendancePage(QWidget *parent, int doctorId) :
    QWidget(parent),
    ui(new Ui::AttendancePage),
    m_doctorId(doctorId)
{
    ui->setupUi(this);
    connect(ui->SignInBtn, &QPushButton::clicked, this, &AttendancePage::signIn);
    connect(ui->SignOutBtn, &QPushButton::clicked, this, &AttendancePage::signOut);
    connect(ui->LeaveBtn, &QPushButton::clicked, this, &AttendancePage::RequestLeave);
    connect(ui->historyBtn, &QPushButton::clicked, this, &AttendancePage::checkHistory);
    ui->startDate->setDate(QDate::currentDate());
    ui->endDate->setDate(QDate::currentDate());
}

AttendancePage::~AttendancePage()
{
    delete ui;
}

void AttendancePage::checkHistory()
{
    QVariantList attendanceHistory = UiController::get().getAttendanceHistory(QString::number(m_doctorId), ui->startDate->date(), ui->endDate->date());
    for (const QVariant &attendance : attendanceHistory)
    {
        if (!attendance.isValid() || !attendance.canConvert<QVariantMap>())
        {
            qWarning() << "Invalid prescription item found";
            continue;
        }
        QVariantMap attendanceInfo = attendance.toMap();
        QString msg;
        msg + "考勤ID: " + attendanceInfo.value("attendance_id").toString()
               + " | 医生ID: " + attendanceInfo.value("doctor_id").toString()
               + " | 日期: " + attendanceInfo.value("date").toString()
               + " | 签到时间: " + attendanceInfo.value("check_in_time").toString()
               + " | 签退时间: " + attendanceInfo.value("check_out_time").toString();

        ui->SignInList->addItem(msg);
    }
}

void AttendancePage::signIn()
{
    UiController::get().checkIn(QString::number(m_doctorId));
    QListWidgetItem* signInRecord = new QListWidgetItem;
    signInRecord->setText("[签到]：" + QDateTime::currentDateTime().toString());
    ui->SignInList->addItem(signInRecord);
    ui->SignInBtn->setDisabled(true);
}

void AttendancePage::signOut()
{
    UiController::get().checkOut(QString::number(m_doctorId));
    QListWidgetItem* signOutRecord = new QListWidgetItem;
    signOutRecord->setText("[签退]：" + QDateTime::currentDateTime().toString());
    ui->SignInList->addItem(signOutRecord);
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

