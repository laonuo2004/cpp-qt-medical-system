#ifndef PAYPAGE_H
#define PAYPAGE_H

#include <QDialog>
#include <QDate>
#include "uicontroller.h"

namespace Ui {
class PayPage;
}

class PayPage : public QDialog
{
    Q_OBJECT

public:
    explicit PayPage(QWidget *parent = nullptr);
    ~PayPage();

private:
    Ui::PayPage *ui;
};

#endif // PAYPAGE_H
