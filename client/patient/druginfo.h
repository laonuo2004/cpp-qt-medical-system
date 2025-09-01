#ifndef DRUGINFO_H
#define DRUGINFO_H

#include <QDialog>

namespace Ui {
class DrugInfo;
}

/**
 * @class DrugInfo
 * @brief 药品详细信息界面
 *
 * 点击药品卡片的“详细信息”按钮打开此界面。
 * 包含药品名字、价格、用法等字段。
 */

class DrugInfo : public QDialog
{
    Q_OBJECT

public:
    explicit DrugInfo(QWidget *parent = nullptr);
    ~DrugInfo();

    void buildUpDrugInfo(const QVariantMap& drugInfo);

private:
    Ui::DrugInfo *ui;
};

#endif // DRUGINFO_H
