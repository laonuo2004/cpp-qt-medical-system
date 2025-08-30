#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include <QDialog>

namespace Ui {
class Prescription;
}

class Prescription : public QDialog
{
    Q_OBJECT

public:
    explicit Prescription(QWidget *parent = nullptr);
    ~Prescription();

private:
    Ui::Prescription *ui;
};

#endif // PRESCRIPTION_H
