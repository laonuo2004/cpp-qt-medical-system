#ifndef LEAVERECORD_H
#define LEAVERECORD_H

#include <QWidget>

namespace Ui {
class LeaveRecord;
}

class LeaveRecord : public QWidget
{
    Q_OBJECT

public:
    explicit LeaveRecord(QWidget *parent = nullptr);
    void setupLeaveItem(QString& LeaveMsg);
    void cancelLeaveRequest();
    ~LeaveRecord();

private:
    Ui::LeaveRecord *ui;
};

#endif // LEAVERECORD_H
