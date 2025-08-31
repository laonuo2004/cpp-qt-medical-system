#ifndef PATIENTINFORMATIONDETAIL_H
#define PATIENTINFORMATIONDETAIL_H

#include <QDialog>

namespace Ui {
class PatientInformationDetail;
}

class PatientInformationDetail : public QDialog
{
    Q_OBJECT

public:
    explicit PatientInformationDetail(QWidget *parent = nullptr);
    ~PatientInformationDetail();

private:
    Ui::PatientInformationDetail *ui;
};

#endif // PATIENTINFORMATIONDETAIL_H
