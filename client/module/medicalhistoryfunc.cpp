#include "medicalhistoryfunc.h"
#include "ui_medicalhistoryfunc.h"
#include <QMessageBox>
#include <QDebug>

MedicalHistoryFunc::MedicalHistoryFunc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MedicalHistoryFunc),
    controller(new UiController(this)) // 使用初始化列表初始化controller
{
    ui->setupUi(this);
    connect(ui->searchMedicalHistory, &QPushButton::clicked, this, &MedicalHistoryFunc::searchMedicalHistory);
}

MedicalHistoryFunc::~MedicalHistoryFunc()
{
    delete ui;
    // controller由Qt父对象系统自动管理，无需手动删除
}

void MedicalHistoryFunc::searchMedicalHistory()
{
    // 1. 验证患者ID输入
    bool conversionOk = false;
    int patientId = ui->PatientId->text().toUInt(&conversionOk);

    if (!conversionOk || patientId <= 0)
    {
        QMessageBox::warning(this, "输入错误", "请输入有效的患者ID（正整数）");
        ui->PatientId->setFocus();
        ui->PatientId->selectAll();
        return;
    }

    // 2. 检查控制器是否有效
    if (!controller)
    {
        QMessageBox::critical(this, "系统错误", "控制器未正确初始化");
        ui->MedicalHistoryInfo->setPlainText("系统错误：控制器未初始化");
        return;
    }

    try
    {
        // 3. 获取病历历史
        QVariantList medicalHistory = controller->getPatientMedicalHistory(patientId);

        // 4. 处理获取到的病历数据
        QString historyString;

        if (medicalHistory.isEmpty())
        {
            historyString = QString("未找到患者ID为 %1 的病历记录。").arg(patientId);
        }
        else
        {
            historyString = QString("找到 %1 条病历记录:\n\n").arg(medicalHistory.size());

            for (const QVariant &record : medicalHistory)
            {
                // 检查记录是否有效
                if (!record.isValid() || !record.canConvert<QVariantMap>())
                {
                    qWarning() << "Invalid medical record item found";
                    continue;
                }

                QVariantMap recordMap = record.toMap();

                // 检查必要字段是否存在
                if (!recordMap.contains("record_id") || !recordMap.contains("diagnosis_date"))
                {
                    qWarning() << "Medical record missing required fields";
                    continue;
                }

                historyString += "记录ID: " + recordMap.value("record_id").toString() + "\n";
                historyString += "预约ID: " + recordMap.value("appointment_id").toString() + "\n";
                historyString += "诊断: " + recordMap.value("diagnosis_notes").toString() + "\n";
                historyString += "日期: " + recordMap.value("diagnosis_date").toString() + "\n";
                historyString += "创建时间: " + recordMap.value("created_at").toString() + "\n";

                // 添加医生信息（如果存在）
                if (recordMap.contains("doctor_name"))
                {
                    historyString += "医生: " + recordMap.value("doctor_name").toString() + "\n";
                }

                historyString += "----------------------------\n"; // 分隔线
            }
        }

        ui->MedicalHistoryInfo->setPlainText(historyString);
    }
    catch (const std::exception& e)
    {
        QMessageBox::critical(this, "查询错误",
                             QString("查询病历历史时发生异常: %1").arg(e.what()));
        ui->MedicalHistoryInfo->setPlainText("查询病历历史时发生错误");
    }
    catch (...)
    {
        QMessageBox::critical(this, "查询错误", "查询病历历史时发生未知异常");
        ui->MedicalHistoryInfo->setPlainText("查询病历历史时发生错误");
    }
}
