#ifndef DOCTORCARD_H
#define DOCTORCARD_H

#include <QWidget>
#include <QVariant>

namespace Ui {
class DoctorCard;
}

/**
 * @class DoctorCard
 * @brief 医生信息卡片
 *
 * 记录三条用户信息和用户证件照。
 * 点击“详细信息”按钮查看用户详细信息。
 */

class DoctorCard : public QWidget
{
    Q_OBJECT

public:
    explicit DoctorCard(QWidget *parent = nullptr);
    ~DoctorCard();

    void buildUpDoctorCard(QVariantMap& doctorInfo);

protected:
    /**
     * @brief 打开细节面板
     *
     * 打开一个新窗口，展示医生的详细信息。
     * 新窗口中可以进行挂号。
     */
    void openDetailPanel();

private:
    Ui::DoctorCard *ui;
    QVariantMap m_doctorInfo;
};

#endif // DOCTORCARD_H
