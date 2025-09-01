#ifndef LEAVERECORD_H
#define LEAVERECORD_H

#include <QWidget>

namespace Ui {
class LeaveRecord;
}

/**
 * @class LeaveRecord
 * @brief 请假记录条目
 *
 * 在考勤子界面，请假Tab中，用作请假列表中的单个条目。
 * 包含请假信息的展示及销假按钮，用户可通过该界面查看和销假。
 * 通过 cancelLeaveRequest() 函数销假，更新数据库状态。
 */

class LeaveRecord : public QWidget
{
    Q_OBJECT

public:
    explicit LeaveRecord(QWidget *parent = nullptr);

    /**
     * @brief 设置请假信息
     *
     * 将传入的请假信息字符串设置为显示的请假内容。
     *
     * @param LeaveMsg 传入的请假信息字符串
     */
    void setupLeaveItem(QString& LeaveMsg);

    /**
     * @brief 取消请假请求
     *
     * 该函数在用户点击“取消”按钮时被调用，标记请假已被销除。
     * 更新界面文本并禁用按钮，防止重复操作。
     * 同时，更新数据库中请假状态为已销假。
     */
    void cancelLeaveRequest();
    ~LeaveRecord();

private:
    Ui::LeaveRecord *ui;
    int m_requestId;
};

#endif // LEAVERECORD_H
