#include "drugcard.h"
#include "ui_drugcard.h"
#include "druginfo.h"

#include <QMessageBox>

DrugCard::DrugCard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrugCard)
{
    ui->setupUi(this);
    ui->Photo->setFixedSize(200, 200);
    connect(ui->DetailBtn, &QPushButton::clicked, this, &DrugCard::showDrugInfo);
}

DrugCard::~DrugCard()
{
    delete ui;
}

void DrugCard::showDrugInfo()
{
    DrugInfo* drugInfo = new DrugInfo(this);
    drugInfo->exec();
    delete drugInfo;
}
