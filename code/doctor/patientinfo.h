#ifndef PATIENTINFO_H
#define PATIENTINFO_H

#include <QDialog>

namespace Ui {
class PatientInfo;
}

class PatientInfo : public QDialog
{
    Q_OBJECT

public:
    explicit PatientInfo(QWidget *parent = nullptr);
    ~PatientInfo();

private:
    Ui::PatientInfo *ui;
};

#endif // PATIENTINFO_H
