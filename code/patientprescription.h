#ifndef PATIENTPRESCRIPTION_H
#define PATIENTPRESCRIPTION_H

#include <QDialog>

namespace Ui {
class PatientPrescription;
}

class PatientPrescription : public QDialog
{
    Q_OBJECT

public:
    explicit PatientPrescription(QWidget *parent = nullptr);
    ~PatientPrescription();

private:
    Ui::PatientPrescription *ui;
};

#endif // PATIENTPRESCRIPTION_H
