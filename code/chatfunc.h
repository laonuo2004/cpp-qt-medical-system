#ifndef CHATFUNC_H
#define CHATFUNC_H

#include <QWidget>
#include "chattool.h"
#include "doctorchattool.h"
#include "patientchattool.h"

namespace Ui {
class chatfunc;
}

class chatfunc : public QWidget
{
    Q_OBJECT

public:
    explicit chatfunc(QWidget *parent = nullptr);
    ~chatfunc();

private:
    Ui::chatfunc *ui;

signals:
    void gochat() ;


};

#endif // CHATFUNC_H
