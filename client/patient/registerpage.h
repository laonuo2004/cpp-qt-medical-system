#ifndef REGISTERPAGE_H
#define REGISTERPAGE_H

#include <QWidget>

namespace Ui {
class RegisterPage;
}

/**
 * @class RegisterPage
 * @brief 患者端界面主窗口的“挂号”页面。
 *
 * 挂号界面，展示医生卡片信息。
 * 选择科室，显示该科室所有医生的卡片，可以点击查看信息，选择时段挂号。
 * 通过 loadPatientInformation() 函数动态加载医生卡片，并显示在界面上。
 * 可根据医生的数量和界面布局调整卡片的排列方式。
 */

class RegisterPage : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterPage(QWidget *parent = nullptr, int patientId = 0);
    ~RegisterPage();

protected:
    /**
     * @brief 加载医生信息并展示
     *
     * 动态生成并展示医生的卡片。每个卡片通过 UserInfoCard 类创建，并添加到布局中。
     * 目前加载的是测试数据，通过循环控制卡片的排列。
     */
    void loadDoctorInformation();

    void loadDepartments();

    void setPatientId(int id);

private:
    Ui::RegisterPage *ui;

    int m_patientId;
};

#endif // REGISTERPAGE_H
