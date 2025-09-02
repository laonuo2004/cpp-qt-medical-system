#ifndef PAYONLINE_H
#define PAYONLINE_H

#include <QWidget>
#include <QPushButton>

#include <QLabel>

namespace Ui {
class PayOnline;
}

class PayOnline : public QWidget
{
    Q_OBJECT

public:
    explicit PayOnline(QWidget *parent = nullptr);
    ~PayOnline();

signals:
    void paymentRequested(); // 支付请求信号
    void paymentDenied() ; //cancel signal

private:
    Ui::PayOnline *ui;


};

#endif // PAYONLINE_H
