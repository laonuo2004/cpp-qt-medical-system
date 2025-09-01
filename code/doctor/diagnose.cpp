#include "diagnose.h"
#include "ui_diagnose.h"
#include "uicontroller.h"

Diagnose::Diagnose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Diagnose)
{
    ui->setupUi(this);
    connect(ui->saveBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->confirmBtn, &QPushButton::clicked, this, &QDialog::accept);
}

Diagnose::~Diagnose()
{
    delete ui;
}

void Diagnose::buildUpDiagnose(int doctorId, const QVariantMap& patientInfo)
{
    QString message;
    message = QString(
        "患者基本信息：\n"
        "姓名: %1\n"
        "性别: %2\n"
        "年龄: %3\n"
        "出生日期: %4\n"
        "身份证号: %5\n"
        "联系电话: %6\n"
        "地址: %7\n"
        "\n"
        "紧急联系人：\n"
        "姓名: %8\n"
        "电话: %9\n"
    )
    .arg(patientInfo.value("full_name").toString())
    .arg(patientInfo.value("sex").toString())
    .arg(patientInfo.value("age").toString())
    .arg(patientInfo.value("birth_date").toString())
    .arg(patientInfo.value("id_card_no").toString())
    .arg(patientInfo.value("phone_no").toString())
    .arg(patientInfo.value("address").toString())
    .arg(patientInfo.value("sos_name").toString())
    .arg(patientInfo.value("sos_phone_no").toString());

    ui->patientInfoInput->setText(message);
}
