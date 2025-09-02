#include "attendancepage.h"
#include "ui_attendancepage.h"
#include "leaverecord.h"
#include "leaveform.h"
#include "uicontroller.h"

#include <QDateTime>
#include <QListWidgetItem>
#include <QDebug>
#include <QTableWidgetItem>
#include <QVariantMap>

AttendancePage::AttendancePage(QWidget *parent, int doctorId) :
    QWidget(parent),
    ui(new Ui::AttendancePage),
    m_doctorId(doctorId)
{
    ui->setupUi(this);

    connect(ui->tabWidget, &QTabWidget::currentChanged, this,
            [this](int index){
                if (index == ui->tabWidget->indexOf(ui->LeaveTab))
                {
                    const QVariantList rows =
                        UiController::get().getLeaveRequests(QString::number(m_doctorId));
                    printLeaveRequests(rows);
                }
            });

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

void AttendancePage::onTabChanged(int index)
{
    // 只在切到“请假记录”这页时触发
    if (index == ui->tabWidget->indexOf(ui->LeaveTab))
    {
        const QVariantList rows =
            UiController::get().getLeaveRequests(QString::number(m_doctorId));
        printLeaveRequests(rows);
    }
}

void AttendancePage::printLeaveRequests(const QVariantList& rows)
{
    ui->textEdit->clear();

    auto prettyDate = [](const QVariant &v) -> QString {
        if (v.canConvert<QDate>()) return v.toDate().toString("yyyy-MM-dd");
        const QString s = v.toString();
        QDate d = QDate::fromString(s, Qt::ISODate);
        if (!d.isValid()) d = QDate::fromString(s, "yyyy-MM-dd");
        return d.isValid() ? d.toString("yyyy-MM-dd") : s;
    };

    QStringList lines;
    lines.reserve(rows.size());

    for (const QVariant &row : rows)
    {
        if (row.canConvert<QVariantMap>()) {
            const QVariantMap m = row.toMap();
            const QString reason = m.value("reason").toString();
            const QString type   = m.contains("request_type")
                                     ? m.value("request_type").toString()
                                     : m.value("type").toString();   // 兜底
            const QString start  = prettyDate(m.value("start_date"));
            const QString end    = prettyDate(m.value("end_date"));

            lines << QString("%1\t%2\t%3\t%4").arg(reason, type, start, end);
        } else {
            // 若返回是 list-of-list，按列顺序：
            // 0 request_id, 1 doctor_id, 2 request_type, 3 start_date, 4 end_date, 5 reason, 6 status
            const QVariantList l = row.toList();
            const QString reason = l.value(5).toString();
            const QString type   = l.value(2).toString();
            const QString start  = prettyDate(l.value(3));
            const QString end    = prettyDate(l.value(4));

            lines << QString("%1\t%2\t%3\t%4").arg(reason, type, start, end);
        }
    }

    ui->textEdit->append(lines.join("\n"));
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

