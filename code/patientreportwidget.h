#ifndef PATIENTREPORTWIDGET_H
#define PATIENTREPORTWIDGET_H

#include <QWidget>

namespace Ui {
class PatientReportWidget;
}

class PatientReportWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PatientReportWidget(QWidget *parent = nullptr);
    ~PatientReportWidget();

protected:
    void loadPatientHistory();

private:
    Ui::PatientReportWidget *ui;
};

#endif // PATIENTREPORTWIDGET_H
