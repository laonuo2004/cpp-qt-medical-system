#ifndef PATIENTCARDSPAGE_H
#define PATIENTCARDSPAGE_H

#include <QWidget>

namespace Ui {
class PatientCardsPage;
}

/**
 * @class PatientCardsPage
 * @brief 医生端界面主窗口的“患者”页面。
 *
 * 在患者信息管理界面，展示患者的卡片信息。
 * 通过 loadPatientInformation() 函数动态加载患者卡片，并显示在界面上。
 * 可根据患者的数量和界面布局调整卡片的排列方式。
 */

class PatientCardsPage : public QWidget
{
    Q_OBJECT

public:
    explicit PatientCardsPage(QWidget *parent = nullptr, int doctorId = 0);
    ~PatientCardsPage();

protected:

    /**
     * @brief 加载患者信息并展示
     *
     * 动态生成并展示患者的卡片。每个卡片通过 UserInfoCard 类创建，并添加到布局中。
     * 目前加载的是测试数据，通过循环控制卡片的排列。
     */
    void loadPatientInformation();

private:
    Ui::PatientCardsPage *ui;
    int m_doctorId;
};

#endif // PATIENTCARDSPAGE_H
