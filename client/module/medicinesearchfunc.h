#ifndef MEDICINESEARCHFUNC_H
#define MEDICINESEARCHFUNC_H

#include <QWidget>
#include "uicontroller.h"

namespace Ui {
class MedicineSearchFunc;
}

class MedicineSearchFunc : public QWidget
{
    Q_OBJECT

public:
    explicit MedicineSearchFunc(QWidget *parent = nullptr);
    ~MedicineSearchFunc();

private:
    Ui::MedicineSearchFunc *ui;
    UiController* controller;

private slots:
    void SearchDrugs();
    void GetDrugDetails();
};

#endif // MEDICINESEARCHFUNC_H
