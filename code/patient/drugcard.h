#ifndef DRUGCARD_H
#define DRUGCARD_H

#include <QWidget>

namespace Ui {
class DrugCard;
}

/**
 * @class DrugCard
 * @brief 药品信息卡片
 *
 * 用于展示药品搜索结果。
 * 显示药品名称和药品图片。
 * 点击“详细信息”按钮查看药品详细信息。
 */

class DrugCard : public QWidget
{
    Q_OBJECT

public:
    explicit DrugCard(QWidget *parent = nullptr);
    ~DrugCard();

protected:
    /**
     * @brief 打开药品详细信息界面
     *
     * 创建药品详细信息界面并打开。
     */
    void showDrugInfo();

private:
    Ui::DrugCard *ui;
};

#endif // DRUGCARD_H
