#include "reportpage.h"
#include "ui_reportpage.h"
#include "medicalrecord.h"

#include "uicontroller.h"

#include <QDebug>

ReportPage::ReportPage(QWidget *parent, int patientId) :
    QWidget(parent),
    ui(new Ui::ReportPage),
    m_patientId(patientId)
{
    ui->setupUi(this);
    loadPatientHistory();
}

ReportPage::~ReportPage()
{
    delete ui;
}

void ReportPage::loadPatientHistory()
{
    // 通过控制器获取所有处方
    UiController& controller = UiController::get();
    QVariantList prescriptions = controller.getPatientPrescriptions(m_patientId);

    for (const QVariant &prescription : prescriptions)
    {
        // 为每个处方创建展示控件
        MedicalRecord* NewHistoryItem = new MedicalRecord(this);
        ui->PatientHistoryList->addWidget(NewHistoryItem);

        // 类型转化为散列表
        if (!prescription.isValid() || !prescription.canConvert<QVariantMap>())
        {
            qWarning() << "Invalid prescription item found";
            continue;
        }
        QVariantMap newPrescription = prescription.toMap();

        // 校验必要字段 - 处方id
        if (!newPrescription.contains("prescription_id"))
        {
            qWarning() << "Prescription item missing prescription_id";
            continue;
        }

        // 构建单个记录
        NewHistoryItem->buildUpMedicalRecord(m_patientId, newPrescription);
    }
}
