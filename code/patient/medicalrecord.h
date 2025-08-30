#ifndef MEDICALRECORD_H
#define MEDICALRECORD_H

#include <QWidget>

namespace Ui {
class MedicalRecord;
}

class MedicalRecord : public QWidget
{
    Q_OBJECT

public:
    explicit MedicalRecord(QWidget *parent = nullptr);
    ~MedicalRecord();

private slots:
    void on_prescriptionBtn_clicked();

private:
    Ui::MedicalRecord *ui;
};

#endif // MEDICALRECORD_H
