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
    explicit DoctorInfo(QWidget *parent = nullptr);
    ~DoctorInfo();

    /**
     * @brief 根据传入数据设置显示的医生信息。
     *
     * 该函数由DoctorCard在创建此窗口时调用。
     */
    void buildUpDoctorInfo(QVariantMap doctorInfo);

protected:

    /**
     * @brief 打开确认挂号界面，并处理挂号成功或失败逻辑
     *
     * 创建确认挂号对话框，处理对话框接受或拒绝时的逻辑。
     */
    void patientRegister();

    void startChat();

private:
    Ui::DoctorInfo *ui;
};

#endif // DOCTORINFO_H
