#include "attendancepage.h"
#include "ui_attendancepage.h"
#include "leaverecord.h"
#include "leaveform.h"
#include "uicontroller.h"

#include <QDateTime>
#include <QListWidgetItem>
#include <QDebug>
#include <QListWidget>
#include <QDateEdit>

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


void AttendancePage::on_historyBtn_clicked()
{
    QDate start=ui->startDate->date();
    QDate end=ui->endDate->date();
    QString doctorid="12";
    QVariantList attendanceRecords=UiController::get().getAttendanceHistory(doctorid,start,end);
    //ui->SignInList->clear();
    // 5. 遍历考勤记录并显示在 QListWidget 中
    QLocale chineseLocale(QLocale::Chinese); // 使用中文区域设置以获取中文星期和月份

    for (const QVariant& recordVariant : attendanceRecords) {
        if (recordVariant.type() == QVariant::Map) {
            QVariantMap recordMap = recordVariant.toMap();

            // 获取日期 (格式: "yyyy-MM-dd")
            QString dateString = recordMap["date"].toString();
            QDate recordDate = QDate::fromString(dateString, "yyyy-MM-dd");

            if (!recordDate.isValid()) {
                ui->SignInList->addItem(QString("错误: 无效的日期格式：%1").arg(dateString));
                continue;
            }

            // 获取签到时间 (格式: "hh:mm:ss")
            QString checkInTimeString = recordMap["check_in_time"].toString();
            // 获取签退时间 (格式: "hh:mm:ss")
            QString checkOutTimeString = recordMap["check_out_time"].toString();

            // 如果有签到时间，则显示签到记录
            if (!checkInTimeString.isEmpty() && checkInTimeString != "NULL") {
                QDateTime checkInDateTime = QDateTime::fromString(dateString + " " + checkInTimeString, "yyyy-MM-dd hh:mm:ss");
                if (checkInDateTime.isValid()) {
                    QString formattedDate = chineseLocale.toString(checkInDateTime, "ddd M月 d hh:mm:ss yyyy");
                    QString displayString = QString("[签到]：%1").arg(formattedDate);
                    ui->SignInList->addItem(displayString);
                } else {
                    //ui->SignInList->addItem(QString("错误: 无效的签到日期/时间格式：%1 %2").arg(dateString, checkInTimeString));
                }
            }

            // 如果有签退时间，则显示签退记录
            // 注意：一个日期可能既有签到又有签退，所以两者独立判断
            if (!checkOutTimeString.isEmpty() && checkOutTimeString != "NULL") {
                QDateTime checkOutDateTime = QDateTime::fromString(dateString + " " + checkOutTimeString, "yyyy-MM-dd hh:mm:ss");
                if (checkOutDateTime.isValid()) {
                    QString formattedDate = chineseLocale.toString(checkOutDateTime, "ddd M月 d hh:mm:ss yyyy");
                    QString displayString = QString("[签退]：%1").arg(formattedDate);
                    ui->SignInList->addItem(displayString);
                } else {
                    //ui->SignInList->addItem(QString("错误: 无效的签退日期/时间格式：%1 %2").arg(dateString, checkOutTimeString));
                }
            }

            // 如果既没有签到也没有签退（理论上不会发生，因为date是NOT NULL）
            if ((checkInTimeString.isEmpty() || checkInTimeString == "NULL") &&
                (checkOutTimeString.isEmpty() || checkOutTimeString == "NULL")) {
                ui->SignInList->addItem(QString("日期 %1: 无签到或签退记录").arg(dateString));
            }

        } else {
            // 处理非 QVariantMap 类型的记录
            ui->SignInList->addItem("错误: 考勤记录格式不正确。");
        }
    }
}
