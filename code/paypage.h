#ifndef PAYPAGE_H
#define PAYPAGE_H

#include <QWidget>

namespace Ui {
class PayPage;
}

class PayPage : public QWidget
{
    Q_OBJECT

public:
    explicit PayPage(QWidget *parent = nullptr);
    ~PayPage();

private:
    Ui::PayPage *ui;

signals:
    void paymentConfirmed(); // 支付确认信号
    void paymentCancelled(); // 支付取消信号

};

#endif // PAYPAGE_H
