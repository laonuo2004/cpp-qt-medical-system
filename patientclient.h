#ifndef PATIENTCLIENT_H
#define PATIENTCLIENT_H

#include <QMainWindow>

namespace Ui {
class PatientClient;
}

/**
 * @class PatientClient
 * @brief 患者客户端主窗口类
 *
 * 该类代表患者端的主界面，继承自 QMainWindow，负责展示多个功能页面，
 * 如个人信息、挂号和报告页面。通过一个堆叠窗口控件 (stackedWidget) 来切换不同的页面。
 * 个人信息页面：显示患者的个人信息。
 * 患者界面：选择科室，显示该科室所有医生的卡片，可以点击查看信息，选择时段挂号。
 * 报告界面：查看就诊记录，查看医生开出的处方，并进行支付。
 */

class PatientClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit PatientClient(QWidget *parent = nullptr);
    ~PatientClient();

private:
    Ui::PatientClient *ui;
    class PatientInfoPage* m_patientInfoPage; /* 个人信息界面 */
    class RegisterPage* m_registerPage; /* 挂号界面 */
    class ReportPage* m_reportPage; /* 查看报告界面 */
    class DrugSearchPage* m_drugSearchPage; /* 工具箱界面 */
    class QButtonGroup* m_navButtonGroup;

    int m_patientId;
};

#endif // PATIENTCLIENT_H
