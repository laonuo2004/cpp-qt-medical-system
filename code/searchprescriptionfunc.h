#ifndef SEARCHPRESCRIPTIONFUNC_H
#define SEARCHPRESCRIPTIONFUNC_H

#include <QWidget>
#include "uiController.h"
namespace Ui {
class SearchprescriptionFunc;
}

class SearchprescriptionFunc : public QWidget
{
    Q_OBJECT

public:
    explicit SearchprescriptionFunc(QWidget *parent = nullptr);
    ~SearchprescriptionFunc();

private:
    Ui::SearchprescriptionFunc *ui;
    UiController* controller ;

private slots:
    void getPatientPrescriptions();
};

#endif // SEARCHPRESCRIPTIONFUNC_H
