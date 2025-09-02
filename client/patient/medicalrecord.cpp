#include "medicalrecord.h"
#include "ui_medicalrecord.h"
#include "prescription.h"
#include "paypage.h"
#include "uicontroller.h"

#include <QDate>
#include <QMessageBox>

MedicalRecord::MedicalRecord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MedicalRecord)
{
    ui->setupUi(this);
    connect(ui->prescriptionBtn, &QPushButton::clicked, this, &MedicalRecord::showPrescription);
    connect(ui->payBtn, &QPushButton::clicked, this, &MedicalRecord::showPayPanel);
}

MedicalRecord::~MedicalRecord()
{
    delete ui;
}

void MedicalRecord::showPrescription()
{
    Prescription* patientPrescription = new Prescription(this);
    patientPrescription->buildUpPrescription(m_prescription);
    patientPrescription->exec();
    delete patientPrescription;
}

void MedicalRecord::buildUpMedicalRecord(int patientId, QVariantMap& prescription)
{
    m_prescription = prescription;
    QString formattedText;
    formattedText += "[日期]: " + prescription.value("diagnosis_date").toString() + " | ";
    formattedText += "[科室]: " + prescription.value("department").toString() + " | ";
    formattedText += "[医生]: " + prescription.value("doctor_name").toString();
    ui->prescriptionLabel->setText(formattedText);

    m_appointmentInfo = UiController::get().getAppointmentsByPrescriptionId(patientId, prescription.value("prescription_id").toInt());
}

void MedicalRecord::showPayPanel()
{
    PayPage* payPage = new PayPage(this);
    if (payPage->exec() == QDialog::Accepted)
    {
        updateAppointmentStatus("completed");
    }
    else
    {
        updateAppointmentStatus("canceled");
    }
    delete payPage;
}

void MedicalRecord::updateAppointmentStatus(const QString& status)
{
    UiController& controller = UiController::get();

    if (!m_appointmentInfo.contains("appointment_id"))
    {
        QMessageBox::warning(this, tr("数据错误"), tr("预约记录数据不完整"));
        return;
    }

    bool idOk = false;
    int appointmentId = m_appointmentInfo.find("appointment_id")->toInt(&idOk);
    if (appointmentId > 0)
    {
        try {
            // 尝试更新预约状态
            bool success = controller.updateAppointmentStatus(appointmentId, status);
            if (success)
            {
                // 显示操作结果消息
                QMessageBox::information(this, tr("支付状态"),
                    status == "completed" ? tr("支付成功完成。") : tr("支付已取消。"));
            }
            else
            {
                QMessageBox::warning(this, tr("操作失败"), tr("更新预约状态失败，请稍后重试。"));
            }
        }
        catch (const std::exception& e)
        {
            QMessageBox::critical(this, tr("系统错误"), tr("更新预约状态时发生异常: %1").arg(e.what()));
        }
        catch (...)
        {
            QMessageBox::critical(this, tr("系统错误"), tr("更新预约状态时发生未知异常"));
        }
    }
    else
    {
        QMessageBox::warning(this, tr("错误"), tr("未找到有效的预约记录。请检查输入数据。"));
    }
}


