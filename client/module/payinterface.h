#ifndef PAYINTERFACE_H
#define PAYINTERFACE_H

#include <QWidget>

#include "payonline.h"
#include "paypage.h"
#include "paysuccess.h"
#include "payfail.h"
#include "payfunc.h"

namespace Ui {
class PayInterface;
}

class PayInterface : public QWidget
{
    Q_OBJECT

public:
    explicit PayInterface(QWidget *parent = nullptr);
    ~PayInterface();

private:
    Ui::PayInterface *ui;

    QStackedWidget*  stackedPages;
    PayFunc*         payfuncPage;
    PayOnline*       payOnlinePage;
    PayPage*         paymentPage;
    PaySuccess*      successPage;
    PayFail*         failPage;


};

#endif // PAYINTERFACE_H
