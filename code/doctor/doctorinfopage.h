#ifndef DOCTORINFOPAGE_H
#define DOCTORINFOPAGE_H

#include <QWidget>

namespace Ui {
class DoctorInfoPage;
}

/**
 * @class DoctorInfoPage
 * @brief 医生端界面主窗口的“个人信息”页面。
 *
 * 医生端主界面的子页面，展示医生信息，医生端可以编辑并保存。
 * 包含姓名、工号、科室、出生年月、联系方式、家庭住址、证件照字段。
 */

class DoctorInfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit DoctorInfoPage(QWidget *parent = nullptr);
    ~DoctorInfoPage();

private:
    Ui::DoctorInfoPage *ui;
};

#endif // DOCTORINFOPAGE_H
