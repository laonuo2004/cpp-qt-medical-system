#ifndef DOCTORPATIENTPROFILEWIDGET_H
#define DOCTORPATIENTPROFILEWIDGET_H

#include <QWidget>

namespace Ui {
class DoctorPatientProfileWidget;
}

class DoctorPatientProfileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DoctorPatientProfileWidget(QWidget *parent = nullptr);
    ~DoctorPatientProfileWidget();

protected:
    void loadPatientInformation();

private:
    Ui::DoctorPatientProfileWidget *ui;
};

#endif // DOCTORPATIENTPROFILEWIDGET_H
