#ifndef DRUGSEARCHPAGE_H
#define DRUGSEARCHPAGE_H

#include <QWidget>

namespace Ui {
class DrugSearchPage;
}

/**
 * @class DrugSearchPage
 * @brief 患者端界面主窗口的“药品搜索”页面。
 *
 * 工具界面，在顶部的搜索框键入药品名称，点击搜索按钮，
 * 即可展示出符合的药品列表，点击卡片查看详情。
 */

class DrugSearchPage : public QWidget
{
    Q_OBJECT

public:
    explicit DrugSearchPage(QWidget *parent = nullptr);
    ~DrugSearchPage();

protected:
    void loadDrugInformation();
    void searchDrug();

private:
    Ui::DrugSearchPage *ui;
};

#endif // DRUGSEARCHPAGE_H
