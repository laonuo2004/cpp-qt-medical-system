#include "medicinesearchfunc.h"
#include "ui_medicinesearchfunc.h"
#include "widget.h"
#include <QApplication>
#include "databasemanager.h"
#include <QDebug>
MedicineSearchFunc::MedicineSearchFunc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MedicineSearchFunc)
{
    ui->setupUi(this);
    controller = new UiController(this);
    connect(ui->SearchDrug , &QPushButton::clicked , this , &MedicineSearchFunc::SearchDrugs) ;
    connect(ui->GetDetail , &QPushButton::clicked , this , &MedicineSearchFunc::GetDrugDetails) ;
}

MedicineSearchFunc::~MedicineSearchFunc()
{
    delete ui;
}

void MedicineSearchFunc::SearchDrugs()
{
    QString drugname = ui->DrugName->text() ;
    QVariantList drugList = controller->searchDrugs(drugname) ;

    QString resultText;
        resultText += QString("找到 %1 个匹配的药品:\n\n").arg(drugList.size());

        for (const QVariant &drugVariant : drugList) {
            QVariantMap drug = drugVariant.toMap();

            resultText += "────────────────────────────────────────\n";
            resultText += QString("药品ID: %1\n").arg(drug["drug_id"].toInt());
            resultText += QString("药品名称: %1\n").arg(drug["drug_name"].toString());
            resultText += QString("描述: %1\n").arg(drug["description"].toString());
            resultText += QString("用法: %1\n").arg(drug["usage"].toString());
            resultText += QString("注意事项: %1\n").arg(drug["precautions"].toString());
            resultText += QString("价格: %1元\n").arg(drug["drug_price"].toDouble());

            if (drug.contains("image_url") && !drug["image_url"].toString().isEmpty()) {
                resultText += QString("图片URL: %1\n").arg(drug["image_url"].toString());
            }

            resultText += "\n";
        }

        ui->DrugBox->setText(resultText);
}

void MedicineSearchFunc::GetDrugDetails()
{
    int drugId = ui->DrugId->text().toUInt();
    QVariantMap details = controller->getDrugDetails(drugId) ;

    QString infoText;

    infoText += QString("药品ID: %1\n").arg(details["drug_id"].toInt());
    infoText += QString("药品名称: %1\n").arg(details["drug_name"].toString());
    infoText += QString("描述: %1\n").arg(details["description"].toString());
    infoText += QString("用法: %1\n").arg(details["usage"].toString());
    infoText += QString("注意事项: %1\n").arg(details["precautions"].toString());
    infoText += QString("价格: %1元\n").arg(details["drug_price"].toDouble());

    ui->DetailBox->setText(infoText);
}
