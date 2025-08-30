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
    class PatientInfoPage* m_patientInfoPage;
    class RegisterPage* m_registerPage;
    class ReportPage* m_reportPage;
};

#endif // PATIENTCLIENT_H
