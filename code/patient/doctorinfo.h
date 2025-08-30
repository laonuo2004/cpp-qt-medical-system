#ifndef DOCTORINFO_H
#define DOCTORINFO_H

#include <QDialog>

namespace Ui {
class DoctorInfo;
}

/**
 * @class DoctorInfo
 * @brief 挂号页面的医生详细信息
 *
 * 在挂号信息界面，点击医生卡片“详细信息”按钮打开此界面。
 * 包含姓名、性别、年龄、科室、职务、电话字段。
 * 提供“医患交流”、“选择时段”按钮。
 * 前者调用聊天模块，后者打开新界面选择可就诊时段，最后完成预约。
 */

class DoctorInfo : public QDialog
{
    Q_OBJECT

public:
    explicit DoctorInfo(QDialog *parent = nullptr);
    ~DoctorInfo();

private:
    Ui::DoctorInfo *ui;
};

#endif // DOCTORINFO_H
