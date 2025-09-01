#ifndef MEDICALRECORD_H
#define MEDICALRECORD_H

#include <QWidget>
#include <QVariant>

namespace Ui {
class MedicalRecord;
}

/**
 * @class MedicalRecord
 * @brief 就诊记录条目
 *
 * 在报告子界面，用作就诊记录列表中的单个条目。
 * 包含就诊信息概述，如时间、科室等。
 * 提供“查看处方”和“在线支付按钮”，前者打开新界面显示处方。
 * 后者打开支付模块（如展示二维码）。
 */

class MedicalRecord : public QWidget
{
    Q_OBJECT

public:
    explicit MedicalRecord(QWidget *parent = nullptr);
    ~MedicalRecord();

    /**
     * @brief 设置控件显示的信息
     *
     * 由报告页面调用，在创建控件后，传入数据，设置处方信息。
     */
    void buildUpMedicalRecord(int patientId, QVariantMap& prescription);

protected:

    /**
     * @brief 显示处方
     *
     * 在查看处方按钮触发后，打开新界面，显示处方内容。
     */
    void showPrescription();

    /**
     * @brief 显示支付界面
     *
     * 在在线支付按钮触发后，打开新界面，显示二维码。
     */
    void showPayPanel();

    void updateAppointmentStatus(const QString& status);

private:
    Ui::MedicalRecord *ui;
    QVariantMap m_appointmentInfo;
    QVariantMap m_prescription;
};

#endif // MEDICALRECORD_H
