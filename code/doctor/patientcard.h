#ifndef PATIENTCARD_H
#define PATIENTCARD_H

#include <QWidget>

namespace Ui {
class PatientCard;
}

/**
 * @class PatientCard
 * @brief 患者信息卡片
 *
 * 记录三条用户信息和用户证件照。
 * 点击“详细信息”按钮查看用户详细信息。
 */

class PatientCard : public QWidget
{
    Q_OBJECT

public:
    explicit PatientCard(QWidget *parent = nullptr);
    ~PatientCard();

protected:

    /**
     * @brief 打开细节面板
     *
     * 打开一个新窗口，展示患者的详细信息。
     * 新窗口中可以进行诊断。
     */
    void openDetailPanel();

private:
    Ui::PatientCard *ui;
};

#endif // PATIENTCARD_H
