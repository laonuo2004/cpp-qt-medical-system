#ifndef PATIENTINFORMATIONWIDGET_H
#define PATIENTINFORMATIONWIDGET_H

#include <QWidget>

namespace Ui {
class PatientInformationWidget;
}

class PatientInformationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PatientInformationWidget(QWidget *parent = nullptr);
    ~PatientInformationWidget();

private:
    Ui::PatientInformationWidget *ui;
};

#endif // PATIENTINFORMATIONWIDGET_H
