#ifndef LEAVEFORM_H
#define LEAVEFORM_H

#include <QDialog>

namespace Ui {
class LeaveForm;
}

class LeaveForm : public QDialog
{
    Q_OBJECT

public:
    explicit LeaveForm(QWidget *parent = nullptr);
    void getLeaveMsg(QString& leaveMsg);
    ~LeaveForm();

private:
    Ui::LeaveForm *ui;
};

#endif // LEAVEFORM_H
