#ifndef USERINFOCARD_H
#define USERINFOCARD_H

#include <QWidget>

namespace Ui {
class UserInfoCard;
}

/**
 * @class UserInfoCard
 * @brief 用户信息卡片
 *
 * 记录三条用户信息和用户证件照。
 * 点击“详细信息”按钮查看用户详细信息。
 * 分为“医生”和“患者”两类，分别用于DoctorInfo和PatientInfo类。
 * 详细信息面板中包含不同功能，如“预约时段”，“就诊”等。
 */

class UserInfoCard : public QWidget
{
    Q_OBJECT

public:
    explicit UserInfoCard(QWidget *parent = nullptr, bool isPatient = false);
    ~UserInfoCard();

protected:
    /**
     * @brief 打开细节面板
     *
     * 打开一个新窗口，展示用户（医生或患者）的详细信息。
     */
    void openDetailPanel();

private:
    Ui::UserInfoCard *ui;
    bool m_isPatient;
};

#endif // USERINFOCARD_H
