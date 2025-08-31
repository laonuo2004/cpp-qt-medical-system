#include "paypage.h"
#include "ui_paypage.h"
#include <QMessageBox>
#include <QDate>

PayPage::PayPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PayPage),
    controller(new UiController(this))  // 初始化列表初始化controller
{
    ui->setupUi(this);
    setupConnections();
}

PayPage::~PayPage()
{
    delete ui;
    // controller由Qt父对象系统自动管理，无需手动删除
}

void PayPage::setupConnections()
{
    connect(ui->yes, &QPushButton::clicked, this, &PayPage::onPaymentConfirmed);
    connect(ui->no, &QPushButton::clicked, this, &PayPage::paymentCancelled);
}

void PayPage::onPaymentConfirmed()
{
    updateAppointmentStatus("completed");
}

void PayPage::onPaymentCancelled()
{
    updateAppointmentStatus("canceled");
}

void PayPage::updateAppointmentStatus(const QString& status)
{
    bool ok = false;
    const int appointmentId = getFirstAppointmentId(ok);

    if (ok) {
        controller->updateAppointmentStatus(appointmentId, status);

        // 显示操作结果消息
        QMessageBox::information(this,
            tr("Payment"),
            status == "completed" ?
                tr("Payment completed successfully.") :
                tr("Payment has been canceled.")
        );
    } else {
        QMessageBox::warning(this,
            tr("Error"),
            tr("No valid appointment found. Please check the input data.")
        );
    }
}

int PayPage::getFirstAppointmentId(bool& ok) const
{
    ok = false;

    // 获取并验证患者ID
    const uint patientId = ui->PatientId->text().toUInt(&ok);
    if (!ok) return -1;

    // 获取并验证日期
    const QDate date = getSelectedDate(ok);
    if (!ok) return -1;

    // 获取预约列表
    const QVariantList appointments = controller->getPatientAppointments(patientId, date);
    if (appointments.isEmpty()) return -1;

    // 提取第一个预约ID
    const QVariantMap firstAppointment = appointments.first().toMap();
    if (!firstAppointment.contains("appointment_id")) return -1;

    ok = true;
    return firstAppointment["appointment_id"].toInt();
}

QDate PayPage::getSelectedDate(bool& ok) const
{
    ok = false;

    const int year = ui->yearEdit->text().toInt(&ok);
    if (!ok) return QDate();

    const int month = ui->monthEdit->text().toInt(&ok);
    if (!ok) return QDate();

    const int day = ui->dayEdit->text().toInt(&ok);
    if (!ok) return QDate();

    const QDate date(year, month, day);
    ok = date.isValid();
    return date;
}
