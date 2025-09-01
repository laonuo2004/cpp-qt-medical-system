#include "patientcardspage.h"
#include "ui_patientcardspage.h"
#include "patientcard.h"
#include "uicontroller.h"

#include <QDebug>

PatientCardsPage::PatientCardsPage(QWidget *parent, int doctorId) :
    QWidget(parent),
    ui(new Ui::PatientCardsPage),
    m_doctorId(doctorId)
{
    ui->setupUi(this);
    loadPatientInformation();
}

PatientCardsPage::~PatientCardsPage()
{
    delete ui;
}

void PatientCardsPage::loadPatientInformation()
{
    UiController& controller = UiController::get();

    QVariantList scheduleList = controller.getDoctorScheduleForDate(QString::number(m_doctorId), QDate::currentDate());

    for (const QVariant &schedule : scheduleList)
    {
        if (!schedule.isValid() || !schedule.canConvert<QVariantMap>())
        {
            qWarning() << "Invalid prescription item found";
            continue;
        }
        QVariantMap scheduleMap = schedule.toMap();

        int cnt = 0;
        PatientCard* newPatientCard = new PatientCard(this);
        ui->PatientCards->addWidget(newPatientCard, cnt / 2, cnt % 2);
        cnt++;

        // 构建单个记录
        newPatientCard->buildUpPatientCard(m_doctorId, scheduleMap);
    }
}
