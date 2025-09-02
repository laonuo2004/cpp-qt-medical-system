#ifndef REGISTERINFO_H
#define REGISTERINFO_H

#include <QDialog>
#include "doctorinfo.h"

namespace Ui {
class RegisterInfo;
}

/**
 * @class RegisterPanel
 * @brief 挂号详细信息面板
 *
 * 在点击医生详细信息中的“挂号”按钮后，打开此界面（二次确认窗口）。
 * 此界面包含挂号信息的汇总，包括费用、科室、医生等。
 * 点击“确认挂号按钮”执行挂号逻辑，判断是否成功。
 */

class RegisterInfo : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterInfo(QWidget *parent = nullptr);
    RegisterInfo(const QVariantMap& doctorInfo, const QString& timeslot, QWidget *parent = nullptr);
    ~RegisterInfo();

protected:

    /**
     * @brief 判断挂号是否成功
     *
     * 调用UiController判断挂号是否成功，如当前时段是否有号源。
     * 医生详细信息界面根据Dialog状态判断结果。
     */
    void handlePatientRegister();

private:
    Ui::RegisterInfo *ui;
    QVariantMap m_doctorInfo;
    QString m_timeslot;
    void applyToUi();
};

#endif // REGISTERINFO_H
