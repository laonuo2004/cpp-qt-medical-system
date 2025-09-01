#ifndef MEDICALHISTORYFUNC_H
#define MEDICALHISTORYFUNC_H

#include <QWidget>
#include "uicontroller.h"

namespace Ui {
class MedicalHistoryFunc;
}

class MedicalHistoryFunc : public QWidget
{
    Q_OBJECT

public:
    explicit MedicalHistoryFunc(QWidget *parent = nullptr);
    ~MedicalHistoryFunc();

private:
    Ui::MedicalHistoryFunc *ui;
    UiController *controller ;

private slots:
    void searchMedicalHistory();
};

#endif // MEDICALHISTORYFUNC_H
