#include "searchprescriptionfunc.h"
#include "ui_searchprescriptionfunc.h"
#include <QMessageBox>
#include <QDebug>

SearchprescriptionFunc::SearchprescriptionFunc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchprescriptionFunc),
    controller(new UiController(this)) // 确保controller被正确初始化
{
    ui->setupUi(this);
    connect(ui->SearchBtn, &QPushButton::clicked, this, &SearchprescriptionFunc::getPatientPrescriptions);
    connect(ui->closeBtn , &QPushButton::clicked , this , &SearchprescriptionFunc::close);
}

SearchprescriptionFunc::~SearchprescriptionFunc()
{
    delete ui;
    // controller由Qt父对象系统自动管理，无需手动删除
}

void SearchprescriptionFunc::getPatientPrescriptions()
{
    // 1. 验证患者ID输入
    bool conversionOk = false;
    int patientId = ui->PatientId->text().toUInt(&conversionOk);

    if (!conversionOk || patientId <= 0)
    {
        QMessageBox::warning(this, "输入错误", "请输入有效的患者ID（正整数）");
        ui->PatientId->setFocus();
        return;
    }

    // 2. 检查控制器是否有效
    if (!controller)
    {
        QMessageBox::critical(this, "系统错误", "控制器未正确初始化");
        return;
    }

    try
    {
        // 3. 获取处方信息
        QVariantList prescriptions = controller->getPatientPrescriptions(patientId);

        // 4. 检查是否成功获取数据
        if (prescriptions.isEmpty())
        {
            ui->prescriptionInfo->setText("未找到该患者的处方记录");
            return;
        }

        // 5. 格式化并显示处方信息
        QString formattedText;

        for (const QVariant &item : prescriptions)
        {
            if (!item.isValid() || !item.canConvert<QVariantMap>())
            {
                qWarning() << "Invalid prescription item found";
                continue;
            }

            QVariantMap prescription = item.toMap();

            // 检查必要字段是否存在
            if (!prescription.contains("prescription_id"))
            {
                qWarning() << "Prescription item missing prescription_id";
                continue;
            }

            formattedText += "处方ID: " + prescription.value("prescription_id").toString() + "\n";
            formattedText += "开具时间: " + prescription.value("issued_at").toString() + "\n";
            formattedText += "详情: " + prescription.value("details").toString() + "\n";
            formattedText += "诊断日期: " + prescription.value("diagnosis_date").toString() + "\n";
            formattedText += "诊断说明: " + prescription.value("diagnosis_notes").toString() + "\n";
            formattedText += "医生ID: " + prescription.value("doctor_id").toString() + "\n";
            formattedText += "医生姓名: " + prescription.value("doctor_name").toString() + "\n";
            formattedText += "科室: " + prescription.value("department").toString() + "\n";
            formattedText += "----------------------------------------\n\n";
        }

        if (formattedText.isEmpty())
        {
            ui->prescriptionInfo->setText("获取到的处方数据格式不正确");
        }
        else
        {
            ui->prescriptionInfo->setText(formattedText);
        }

    }
    catch (const std::exception& e)
    {
        QMessageBox::critical(this, "系统错误",
                             QString("获取处方信息时发生异常: %1").arg(e.what()));
        ui->prescriptionInfo->setText("获取处方信息失败");
    }
    catch (...)
    {
        QMessageBox::critical(this, "系统错误", "获取处方信息时发生未知异常");
        ui->prescriptionInfo->setText("获取处方信息失败");
    }
}
