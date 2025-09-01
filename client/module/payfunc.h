#ifndef PAYFUNC_H
#define PAYFUNC_H
#include <QWidget>
#include <QMainWindow>
#include <QStackedWidget>

#include "payonline.h"
#include "paypage.h"
#include "paysuccess.h"
#include "payfail.h"

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

signals:
    void gopay() ;

private:
    void setupUI();
};

#endif // PAYFUNC_H
