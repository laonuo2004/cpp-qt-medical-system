#ifndef DOCTORCLIENT_H
#define DOCTORCLIENT_H

#include <QMainWindow>

namespace Ui {
class DoctorClient;
}

class DoctorClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit DoctorClient(QWidget *parent = nullptr);
    ~DoctorClient();

private:
    Ui::DoctorClient *ui;
    class DoctorInformationWidget* m_doctorInformationWidget;
    class DoctorPatientProfileWidget* m_doctorPatientProfileWidget;
    class DoctorAttendanceWidget* m_doctorAttendanceWidget;
};

#endif // DOCTORCLIENT_H
