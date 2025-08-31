#include "drugsearchpage.h"
#include "ui_drugsearchpage.h"
#include "drugcard.h"

DrugSearchPage::DrugSearchPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrugSearchPage)
{
    ui->setupUi(this);
    connect(ui->SearchBtn, &QPushButton::clicked, this, &DrugSearchPage::loadDrugInformation);
}

DrugSearchPage::~DrugSearchPage()
{
    delete ui;
}

void DrugSearchPage::searchDrug()
{
    // 搜索药品逻辑
    loadDrugInformation();
}

void DrugSearchPage::loadDrugInformation()
{
    // Test
    for (int i = 0; i < 5; i++)
    {
        DrugCard* NewDrugCard = new DrugCard(this);
        ui->DrugList->addWidget(NewDrugCard, i / 3, i % 3);
    }
}
