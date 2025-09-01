#ifndef PATIENTINFOPAGE_H
#define PATIENTINFOPAGE_H

#include <QWidget>

namespace Ui {
class PatientInfoPage;
}

/**
 * @class PatientInfoPage
 * @brief 患者端界面主窗口的“个人信息”页面。
 *
 * 患者端主界面的子页面，展示患者信息，患者可以编辑并保存。
 * 包含姓名、身份证号、性别、出生年月、年龄、手机号、家庭住址、证件照字段。
 * 包含紧急联系人姓名、电话字段。
 */

class PatientInfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit PatientInfoPage(QWidget *parent = nullptr);
    ~PatientInfoPage();

protected:
    /* 创建编辑个人信息弹窗 */
    void editPatientInfo();

private:
    Ui::PatientInfoPage *ui;
};

#endif // PATIENTINFOPAGE_H
