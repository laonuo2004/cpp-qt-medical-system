#ifndef DOCTORATTENDANCEWIDGET_H
#define DOCTORATTENDANCEWIDGET_H

#include <QWidget>

namespace Ui {
class DoctorAttendanceWidget;
}

class DoctorAttendanceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DoctorAttendanceWidget(QWidget *parent = nullptr);
    ~DoctorAttendanceWidget();

private:
    Ui::DoctorAttendanceWidget *ui;
};

#endif // DOCTORATTENDANCEWIDGET_H
