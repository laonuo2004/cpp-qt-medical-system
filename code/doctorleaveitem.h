#ifndef DOCTORLEAVEITEM_H
#define DOCTORLEAVEITEM_H

#include <QWidget>

namespace Ui {
class DoctorLeaveItem;
}

class DoctorLeaveItem : public QWidget
{
    Q_OBJECT

public:
    explicit DoctorLeaveItem(QWidget *parent = nullptr);
    void setupLeaveItem(QString& LeaveMsg);
    void cancelLeaveRequset();
    ~DoctorLeaveItem();

private:
    Ui::DoctorLeaveItem *ui;
};

#endif // DOCTORLEAVEITEM_H
