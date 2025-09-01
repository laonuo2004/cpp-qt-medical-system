#ifndef PAYFAIL_H
#define PAYFAIL_H

#include <QWidget>

namespace Ui {
class PayFail;
}

class PayFail : public QWidget
{
    Q_OBJECT

public:
    explicit PayFail(QWidget *parent = nullptr);
    ~PayFail();

private:
    Ui::PayFail *ui;

signals:
    void confirm_2() ;
};

#endif // PAYFAIL_H
