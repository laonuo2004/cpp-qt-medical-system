#include "registerpage.h"
#include "ui_registerpage.h"
#include "doctorcard.h"
#include "uicontroller.h"

#include <QMessageBox>
#include <QDebug>

RegisterPage::RegisterPage(QWidget *parent, int patientId) :
    QWidget(parent),
    ui(new Ui::RegisterPage),
    m_patientId(patientId)
{
    ui->setupUi(this);
    loadDepartments();
    loadDoctorInformation();

    connect(ui->DepartmentInput, &QComboBox::currentTextChanged, this, &RegisterPage::loadDoctorInformation);
}

RegisterPage::~RegisterPage()
{
    delete ui;
}

void RegisterPage::loadDepartments()
{
    QVariantList departmentList = UiController::get().getAllDepartments();
    if (departmentList.isEmpty())
    {
        qWarning() << "No department loaded!";
    }

    for (const QVariant &departmentItem : departmentList)
    {
        if (!departmentItem.isValid() || !departmentItem.canConvert<QVariantMap>())
        {
            qWarning() << "Invalid drug item found";
            continue;
        }

        QVariantMap department = departmentItem.toMap();
        QString departmentName = department.value("department_name").toString();
        ui->DepartmentInput->addItem(departmentName);
    }
}


void RegisterPage::loadDoctorInformation()
{
    if (ui->DepartmentInput->count() == 0)
    {
        qWarning() << "No doctor loaded!";
        return;
    }

    QVariantList doctorList = UiController::get().getAvailableDoctors(ui->DepartmentInput->currentText());
    if (doctorList.isEmpty())
    {
        QMessageBox::information(this, "显示医生", "该科室当天没有医生排班！");
        return;
    }

    int cnt = 0;
    for (const QVariant &doctorItem : doctorList)
    {
        if (!doctorItem.isValid() || !doctorItem.canConvert<QVariantMap>())
        {
            qWarning() << "Invalid drug item found";
            continue;
        }
        QVariantMap doctorInfo = doctorItem.toMap();

        DoctorCard* NewDoctorCard = new DoctorCard(this);
        ui->DoctorCardList->addWidget(NewDoctorCard, cnt / 2, cnt % 2);
        NewDoctorCard->buildUpDoctorCard(doctorInfo);
        cnt++;
    }
}
