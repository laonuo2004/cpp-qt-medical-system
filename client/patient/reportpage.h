#ifndef REPORTPAGE_H
#define REPORTPAGE_H

#include <QWidget>

namespace Ui {
class ReportPage;
}

/**
 * @class RegisterPage
 * @brief 患者端界面主窗口的“报告”页面。
 *
 * 报告界面，主要控件为一个就诊记录列表。
 * 包含就诊信息概述，如时间、科室等。
 * 提供“查看处方”和“在线支付按钮”，前者打开新界面显示处方。
 * 后者打开支付模块（如展示二维码）。
 */

class ReportPage : public QWidget
{
    Q_OBJECT

public:
    explicit ReportPage(QWidget *parent = nullptr);
    ~ReportPage();

protected:
    /**
     * @brief 加载就诊历史记录并展示
     *
     * 动态生成并展示诊历史记录条目。
     * 目前加载的是测试数据，通过循环控制卡片的排列。
     */
    void loadPatientHistory();

private:
    Ui::ReportPage *ui;
};

#endif // REPORTPAGE_H
