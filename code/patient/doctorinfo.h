#ifndef DOCTORINFO_H
#define DOCTORINFO_H

#include <QDialog>

namespace Ui {
class DoctorInfo;
}

class DoctorInfo : public QDialog
{
    Q_OBJECT

public:
    explicit DoctorInfo(QDialog *parent = nullptr);
    ~DoctorInfo();

private:
    Ui::DoctorInfo *ui;
};

#endif // DOCTORINFO_H
