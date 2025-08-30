#ifndef PATIENTCLIENT_H
#define PATIENTCLIENT_H

#include <QMainWindow>

namespace Ui {
class PatientClient;
}

class PatientClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit PatientClient(QWidget *parent = nullptr);
    ~PatientClient();

private:
    Ui::PatientClient *ui;
    class PatientInformationWidget* m_patientInformationWidget;
    class PatientRegisterWidget* m_patientRegisterWidget;
    class PatientReportWidget* m_patientReportWidget;
};

#endif // PATIENTCLIENT_H
