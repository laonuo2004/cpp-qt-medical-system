#ifndef PAYFUNC_H
#define PAYFUNC_H

#include <QWidget>
#include <QMainWindow>
#include "payonline.h"
#include "paypage.h"
#include "paysuccess.h"
#include "payfail.h"
#include "doctorchattool.h"
#include "patientchattool.h"
namespace Ui {
class PayFunc;
}

class PayFunc : public QWidget
{
    Q_OBJECT

public:
    explicit PayFunc(QWidget *parent = nullptr);
    ~PayFunc();

private:
    Ui::PayFunc *ui;

private slots:
    void showPaymentPage();
    void showCompletionPage();
    void showFailPage() ;
    void showPayPage();
    void showWidgetPage();

private:

    void setupUI();
    void switchPage(QWidget *page);


    PayOnline *PayOnlinePage;
    PayPage *paymentPage;
    QWidget *previousPage;
    QWidget *currentPage;
    PaySuccess *successpage;
    PayFail *failpage ;
};

#endif // PAYFUNC_H
