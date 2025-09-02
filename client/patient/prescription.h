#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include <QDialog>

namespace Ui {
class Prescription;
}

/**
 * @class Prescription
 * @brief 处方页面。
 *
 * 患者端在报告界面，选择就诊记录条目，点击查看处方打开此界面。
 * 包含序号、日期、科室、主治医生、详细处方字段。
 */

class Prescription : public QDialog
{
    Q_OBJECT

public:
    explicit Prescription(QWidget *parent = nullptr);
    ~Prescription();

    void startChat();

    void buildUpPrescription(const QVariantMap& prescription);

private:
    Ui::Prescription *ui;
};

#endif // PRESCRIPTION_H
