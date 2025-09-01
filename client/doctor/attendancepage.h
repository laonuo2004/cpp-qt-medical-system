#ifndef ATTENDANCEPAGE_H
#define ATTENDANCEPAGE_H

#include <QWidget>

namespace Ui {
class AttendancePage;
}

/**
 * @class AttendancePage
 * @brief 医生端界面主窗口的“考勤”页面。
 *
 * 在医生端的主界面侧边栏，点击“考勤”，即可打开此页面。
 * 它的主要控件是一个Tab控件，包含“签到”和“请假”两个Tab。
 * “签到”Tab下有两个列表，通过点击签到、签退按钮进行记录。
 * “请假”Tab下有一个请假记录表，点击“新建请假申请”即可打开表单，创建申请。
 */

class AttendancePage : public QWidget
{
    Q_OBJECT

public:
    explicit AttendancePage(QWidget *parent = nullptr, int doctorId = 0);
    ~AttendancePage();

protected:

    /**
     * @brief 签到操作，记录当前时间并禁用签到按钮。
     *
     * 该函数会创建一个新的 QListWidgetItem，设置其文本为当前时间，
     * 然后将其添加到签到列表中，并禁用签到按钮，以避免重复操作。
     */
    void signIn();


    /**
     * @brief 签退操作，记录当前时间并禁用签退按钮。
     *
     * 该函数会创建一个新的 QListWidgetItem，设置其文本为当前时间，
     * 然后将其添加到签退列表中，并禁用签退按钮，以避免重复操作。
     */
    void signOut();


    /**
     * @brief 请假操作，打开请假表单并提交请假信息。
     *
     * 该函数会创建一个新的 LeaveForm 对话框，并显示给用户填写请假信息。
     * 如果用户确认，请假信息将被提取并添加到请假记录列表中。
     */
    void RequestLeave();

    void checkHistory();

private:
    Ui::AttendancePage *ui;
    int m_doctorId;
};

#endif // ATTENDANCEPAGE_H
