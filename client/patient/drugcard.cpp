#include "drugcard.h"
#include "ui_drugcard.h"
#include "druginfo.h"

#include <QVariantMap>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

DrugCard::DrugCard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrugCard)
{
    ui->setupUi(this);
    connect(ui->DetailBtn, &QPushButton::clicked, this, &DrugCard::showDrugInfo);
}

DrugCard::~DrugCard()
{
    delete ui;
}

void DrugCard::showDrugInfo()
{
    DrugInfo* drugInfo = new DrugInfo(this);
    drugInfo->buildUpDrugInfo(m_drugInfo);
    drugInfo->exec();
    delete drugInfo;
}

void DrugCard::buildUpDrugCard(QVariantMap& drugInfo)
{
    m_drugInfo = drugInfo;
    QString drugText = "[药名]: " + drugInfo.value("drug_name").toString()
            + "  |  [价格]: " + drugInfo.value("drug_price").toString() + "￥";
    ui->detailInput->setText(drugText);
}
