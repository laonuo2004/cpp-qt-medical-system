#include "drugsearchpage.h"
#include "ui_drugsearchpage.h"
#include "drugcard.h"
#include "uicontroller.h"

#include <QMessageBox>
#include <QDebug>
#include <QRegularExpression>

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
    // 1. 验证药品名称输入
    QString drugname = ui->drugName->text().trimmed();
    if (drugname.isEmpty())
    {
        QMessageBox::warning(this, "输入错误", "请输入药品名称");
        ui->drugName->setFocus();
        return;
    }

    // 检查药品名称是否包含潜在的危险字符（基本的SQL注入防护）
    QRegularExpression dangerousChars("[;\"'\\|`]");
    if (drugname.contains(dangerousChars))
    {
        QMessageBox::warning(this, "输入错误", "药品名称包含无效字符");
        ui->drugName->setFocus();
        return;
    }

    QVariantList drugList = UiController::get().searchDrugs(drugname);
    if (drugList.isEmpty())
    {
        QMessageBox::information(this, "搜索结果", QString("未找到与\"%1\"相关的药品").arg(drugname));
        return;
    }

    int cnt = 0;
    for (const QVariant &drugVariant : drugList)
    {
        if (!drugVariant.isValid() || !drugVariant.canConvert<QVariantMap>())
        {
            qWarning() << "Invalid drug item found";
            continue;
        }

        QVariantMap drug = drugVariant.toMap();

        // 检查必要字段是否存在
        if (!drug.contains("drug_id") || !drug.contains("drug_name"))
        {
            qWarning() << "Drug item missing required fields";
            continue;
        }

        DrugCard* newDrugCard = new DrugCard(this);
        ui->DrugList->addWidget(newDrugCard, cnt / 4, cnt % 4);
        newDrugCard->buildUpDrugCard(drug);
        cnt++;
    }
}


