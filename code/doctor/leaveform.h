#ifndef LEAVEFORM_H
#define LEAVEFORM_H

#include <QDialog>

namespace Ui {
class LeaveForm;
}

/**
 * @class LeaveForm
 * @brief 请假申请表单
 *
 * 在考勤子界面，请假Tab中，点击“新建请假申请”创建此表单。
 * 包含请假性质、开始日期、结束日期、请假事由字段。
 * 提交后生成一段请假信息，显示在请假列表中。
 */

class LeaveForm : public QDialog
{
    Q_OBJECT

public:
    explicit LeaveForm(QWidget *parent = nullptr);

    /**
     * @brief 返回表单中填写的请假信息
     *
     * 该函数从表单中提取用户输入的请假信息，并将其格式化为一段字符串。
     * 包含请假性质、开始日期和结束日期。格式为：
     * "请假性质: <请假性质> | 开始日期: <开始日期> | 结束日期: <结束日期>"。
     *
     * @param leaveMsg 请假信息的输出参数，保存格式化后的请假信息。
     */
    void getLeaveMsg(QString& leaveMsg);
    ~LeaveForm();

private:
    Ui::LeaveForm *ui;
};

#endif // LEAVEFORM_H
