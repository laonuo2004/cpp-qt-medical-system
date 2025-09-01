#ifndef PATIENTHISTORYITEM_H
#define PATIENTHISTORYITEM_H

#include <QWidget>
#include "payinterface.h"
namespace Ui {
class PatientHistoryItem;
}

class PatientHistoryItem : public QWidget
{
    Q_OBJECT

public:
    explicit PatientHistoryItem(QWidget *parent = nullptr);
    ~PatientHistoryItem();

private slots:
    void on_prescriptionBtn_clicked();


private:
    Ui::PatientHistoryItem *ui;
    PayInterface* payInterface;
};

#endif // PATIENTHISTORYITEM_H
