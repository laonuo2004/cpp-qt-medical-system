#ifndef PATIENTINFO_H
#define PATIENTINFO_H

#include <QDialog>
#include <QVariant>

namespace Ui {
class PatientInfo;
}

/**
 * @class PatientInfo
 * @brief 患者页面的患者详细信息
 *
 * 在患者信息界面，点击患者卡片“详细信息”按钮打开此界面。
 * 包含姓名、性别、年龄、电话、既往病史、病症描述字段。
 * 提供“医患交流”、“诊断”按钮。
 * 前者调用聊天模块，后者打开新界面（未实现）供医生填写处方、医嘱。
 */

class PatientInfo : public QDialog
{
    Q_OBJECT

public:
    explicit PatientInfo(QWidget *parent = nullptr);
    ~PatientInfo();

    void buildUpPatientInfo(int doctorId, const QVariantMap& scheduleInfo);

protected:
    void diagnose();

    void startChat();

private:
    Ui::PatientInfo *ui;
    QVariantMap m_patientInfo;
    int m_doctorId;
};

#endif // PATIENTINFO_H
