#ifndef PAYPAGE_H
#define PAYPAGE_H

#include <QWidget>
#include "uiController.h"

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
    UiController* controller ;
    void setupConnections();

    void PsetupConnections();

    void onPaymentConfirmed();

    void onPaymentCancelled();

    void updateAppointmentStatus(const QString& status);

    int getFirstAppointmentId(bool& ok) const ;

    QDate getSelectedDate(bool& ok) const ;

signals:
    void paymentConfirmed(); // 支付确认信号
    void paymentCancelled(); // 支付取消信号

};

#endif // PAYPAGE_H
