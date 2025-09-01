#ifndef PAYSUCCESS_H
#define PAYSUCCESS_H

#include <QWidget>

namespace Ui {
class PaySuccess;
}

class PaySuccess : public QWidget
{
    Q_OBJECT

public:
    explicit PaySuccess(QWidget *parent = nullptr);
    ~PaySuccess();

private:
    Ui::PaySuccess *ui;

signals:
    void confirm() ;

};

#endif // PAYSUCCESS_H
