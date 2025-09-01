#include "medicinesearchfunc.h"
#include "ui_medicinesearchfunc.h"
//#include "widget.h"
#include <QApplication>
#include "databasemanager.h"
#include <QMessageBox>
#include <QDebug>
#include <QRegularExpression>

MedicineSearchFunc::MedicineSearchFunc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MedicineSearchFunc),
    controller(new UiController(this)) // 使用初始化列表初始化controller
{
    ui->setupUi(this);
    connect(ui->SearchDrug, &QPushButton::clicked, this, &MedicineSearchFunc::SearchDrugs);
    connect(ui->GetDetail, &QPushButton::clicked, this, &MedicineSearchFunc::GetDrugDetails);
}

MedicineSearchFunc::~MedicineSearchFunc()
{
    delete ui;
    // controller由Qt父对象系统自动管理，无需手动删除
}

void MedicineSearchFunc::SearchDrugs()
{
    // 1. 验证药品名称输入
    QString drugname = ui->DrugName->text().trimmed();
    if (drugname.isEmpty())
    {
        QMessageBox::warning(this, "输入错误", "请输入药品名称");
        ui->DrugName->setFocus();
        return;
    }

    // 检查药品名称是否包含潜在的危险字符（基本的SQL注入防护）
    QRegularExpression dangerousChars("[;\"'\\|`]");
    if (drugname.contains(dangerousChars))
    {
        QMessageBox::warning(this, "输入错误", "药品名称包含无效字符");
        ui->DrugName->setFocus();
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
        // 3. 搜索药品
        QVariantList drugList = controller->searchDrugs(drugname);

        // 4. 处理搜索结果
        if (drugList.isEmpty())
        {
            ui->DrugBox->setText(QString("未找到与\"%1\"相关的药品").arg(drugname));
            return;
        }

        QString resultText;
        resultText += QString("找到 %1 个匹配的药品:\n\n").arg(drugList.size());

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

            resultText += "────────────────────────────────────────\n";
            resultText += QString("药品ID: %1\n").arg(drug.value("drug_id").toInt());
            resultText += QString("药品名称: %1\n").arg(drug.value("drug_name").toString());
            resultText += QString("描述: %1\n").arg(drug.value("description").toString());
            resultText += QString("用法: %1\n").arg(drug.value("usage").toString());
            resultText += QString("注意事项: %1\n").arg(drug.value("precautions").toString());
            resultText += QString("价格: %1元\n").arg(drug.value("drug_price").toDouble());

            if (drug.contains("image_url") && !drug["image_url"].toString().isEmpty()) {
                resultText += QString("图片URL: %1\n").arg(drug.value("image_url").toString());
            }

            resultText += "\n";
        }

        ui->DrugBox->setText(resultText);

    }
    catch (const std::exception& e)
    {
        QMessageBox::critical(this, "搜索错误",
                             QString("搜索药品时发生异常: %1").arg(e.what()));
        ui->DrugBox->setText("搜索药品时发生错误");
    }
    catch (...)
    {
        QMessageBox::critical(this, "搜索错误", "搜索药品时发生未知异常");
        ui->DrugBox->setText("搜索药品时发生错误");
    }
}

void MedicineSearchFunc::GetDrugDetails()
{
    // 1. 验证药品ID输入
    bool conversionOk = false;
    int drugId = ui->DrugId->text().toUInt(&conversionOk);

    if (!conversionOk || drugId <= 0)
    {
        QMessageBox::warning(this, "输入错误", "请输入有效的药品ID（正整数）");
        ui->DrugId->setFocus();
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
        // 3. 获取药品详情
        QVariantMap details = controller->getDrugDetails(drugId);

        // 4. 检查是否成功获取详情
        if (details.isEmpty() || !details.contains("drug_id"))
        {
            ui->DetailBox->setText(QString("未找到ID为%1的药品").arg(drugId));
            return;
        }

        QString infoText;
        infoText += QString("药品ID: %1\n").arg(details.value("drug_id").toInt());
        infoText += QString("药品名称: %1\n").arg(details.value("drug_name").toString());
        infoText += QString("描述: %1\n").arg(details.value("description").toString());
        infoText += QString("用法: %1\n").arg(details.value("usage").toString());
        infoText += QString("注意事项: %1\n").arg(details.value("precautions").toString());
        infoText += QString("价格: %1元\n").arg(details.value("drug_price").toDouble());

        // 添加其他可能存在的字段
        if (details.contains("image_url") && !details["image_url"].toString().isEmpty())
        {
            infoText += QString("图片URL: %1\n").arg(details.value("image_url").toString());
        }

        if (details.contains("stock_quantity"))
        {
            infoText += QString("库存数量: %1\n").arg(details.value("stock_quantity").toInt());
        }

        if (details.contains("manufacturer"))
        {
            infoText += QString("生产厂商: %1\n").arg(details.value("manufacturer").toString());
        }

        ui->DetailBox->setText(infoText);

    }
    catch (const std::exception& e)
    {
        QMessageBox::critical(this, "获取详情错误",
                             QString("获取药品详情时发生异常: %1").arg(e.what()));
        ui->DetailBox->setText("获取药品详情时发生错误");
    }
    catch (...)
    {
        QMessageBox::critical(this, "获取详情错误", "获取药品详情时发生未知异常");
        ui->DetailBox->setText("获取药品详情时发生错误");
    }
}
