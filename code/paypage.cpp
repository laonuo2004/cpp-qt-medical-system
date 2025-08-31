#include "paypage.h"
#include "ui_paypage.h"
#include <QMessageBox>
PayPage::PayPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PayPage)
{
    ui->setupUi(this);
    controller = new UiController(this);
    int patientId = ui->PatientId->text().toUInt();
    int firstAppointmentId;

    QString yearText = ui->yearEdit->text().trimmed();
    QString monthText = ui->monthEdit->text().trimmed();
    QString dayText = ui->dayEdit->text().trimmed();

    int year = yearText.toInt();
    int month = monthText.toInt();
    int day = dayText.toInt();

    QDate date(year, month, day);

    QVariantList appointments = controller->getPatientAppointments(patientId, date);

    // 提取所有 appointment_id
    QList<int> appointmentIds;
    for (const QVariant &appointment : appointments)
    {
        QVariantMap appointmentMap = appointment.toMap();
        if (appointmentMap.contains("appointment_id")) {
            appointmentIds.append(appointmentMap["appointment_id"].toInt());
        }
    }

    if (!appointments.isEmpty()) {
        QVariantMap firstAppointment = appointments.first().toMap();
        firstAppointmentId = firstAppointment["appointment_id"].toInt();
    }

    connect(ui->yes, &QPushButton::clicked, this, &PayPage::paymentConfirmed);
    connect(this, &PayPage::paymentConfirmed, [=]() {
        controller->updateAppointmentStatus(firstAppointmentId, "completed");
    });
    connect(ui->no, &QPushButton::clicked, this, &PayPage::paymentCancelled);
    connect(this, &PayPage::paymentCancelled, [=]() {
        controller->updateAppointmentStatus(firstAppointmentId, "canceled");
    });
}


PayPage::~PayPage()
{
    delete ui;
}
