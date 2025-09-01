#ifndef DOCTORCLIENT_H
#define DOCTORCLIENT_H

#include <QMainWindow>

namespace Ui {
class DoctorClient;
}

/**
 * @class DoctorClient
 * @brief 医生客户端主窗口类
 *
 * 该类代表医生端的主界面，继承自 QMainWindow，负责展示多个功能页面，
 * 如医生信息、患者和考勤页面。通过一个堆叠窗口控件 (stackedWidget) 来切换不同的页面。
 * 医生信息页面：显示医生的个人信息。
 * 患者界面：显示已挂号的患者卡片，可以点击查看信息和进行就诊。
 * 考勤界面：签到、签退、请假、销假。
 */

class DoctorClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit DoctorClient(QWidget *parent = nullptr);
    ~DoctorClient();

private:
    Ui::DoctorClient *ui;
    class DoctorInfoPage* m_doctorInfoPage; /* 医生信息页面指针 */
    class PatientCardsPage* m_patientCardsPage; /* 病人卡片页面指针 */
    class AttendancePage* m_AttendancePage; /* 考勤页面指针 */
    class QButtonGroup* m_navButtonGroup;

    int m_doctorId = 0;
};

#endif // DOCTORCLIENT_H
