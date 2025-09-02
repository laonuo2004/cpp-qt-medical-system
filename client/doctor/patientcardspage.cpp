#include "patientcardspage.h"
#include "ui_patientcardspage.h"
#include "patientcard.h"
#include "uicontroller.h"

#include <QDebug>
#include <QComboBox>

PatientCardsPage::PatientCardsPage(QWidget *parent, int doctorId) :
    QWidget(parent),
    ui(new Ui::PatientCardsPage),
    m_doctorId(doctorId)
{
    ui->setupUi(this);
    loadPatientInformation();
    connect(ui->appointmentFilter, &QComboBox::currentTextChanged, this, &PatientCardsPage::loadPatientInformation);
}

PatientCardsPage::~PatientCardsPage()
{
    delete ui;
}

void PatientCardsPage::loadPatientInformation()
{
    UiController& controller = UiController::get();

    QDate today = QDate::currentDate();
    int cnt = 0;

    int days = 0;
    switch (ui->appointmentFilter->currentIndex())
    {
        case 0: days = 3; break;
        case 1: days = 10; break;
        case 2: days = 30; break;
        default: days = 3;
    }

    clearGridLayout(ui->PatientCards);

    for (int i = 0; i < days; ++i)  // 遍历近10天
    {
        QDate date = today.addDays(i);

        QVariantList scheduleList = controller.getDoctorScheduleForDate(
            QString::number(m_doctorId), date);

        for (const QVariant &schedule : scheduleList)
        {
            if (!schedule.isValid() || !schedule.canConvert<QVariantMap>())
            {
                qWarning() << "Invalid schedule item found";
                continue;
            }
            QVariantMap scheduleMap = schedule.toMap();

            PatientCard* newPatientCard = new PatientCard(this);
            ui->PatientCards->addWidget(newPatientCard, cnt / 2, cnt % 2);
            cnt++;

            newPatientCard->buildUpPatientCard(m_doctorId, scheduleMap);
        }
    }
}

void PatientCardsPage::clearGridLayout(QGridLayout* layout)
{
    if (!layout) return;

    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr)
    {
        if (QWidget* widget = item->widget())
        {
            widget->setParent(nullptr);
            widget->deleteLater();
        }
        delete item;
    }
}
