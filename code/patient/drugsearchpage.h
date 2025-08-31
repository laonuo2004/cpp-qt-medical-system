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
    /**
     * @brief 加载药品卡片并展示
     *
     * 动态生成并展示搜索到的药品卡片。
     * 目前加载的是测试数据，通过循环控制卡片的排列。
     */
    void loadDrugInformation();

    /**
     * @brief 根据名字搜索药品
     *
     * 点击搜索按钮后调用此函数，在数据库中进行搜索
     * 之后调用loadDrugInformation展示搜索到的数据。
     * 参数待修改。
     */
    void searchDrug();

private:
    Ui::DrugSearchPage *ui;
};

#endif // DRUGSEARCHPAGE_H
