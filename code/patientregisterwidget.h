#ifndef PATIENTREGISTERWIDGET_H
#define PATIENTREGISTERWIDGET_H

#include <QWidget>

namespace Ui {
class PatientRegisterWidget;
}

class PatientRegisterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PatientRegisterWidget(QWidget *parent = nullptr);
    ~PatientRegisterWidget();

protected:
    void loadDoctorInformation();

private:
    Ui::PatientRegisterWidget *ui;
};

#endif // PATIENTREGISTERWIDGET_H
