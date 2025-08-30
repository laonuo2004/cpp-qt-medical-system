#ifndef DOCTORINFORMATIONDETAIL_H
#define DOCTORINFORMATIONDETAIL_H

#include <QDialog>

namespace Ui {
class DoctorInformationDetail;
}

class DoctorInformationDetail : public QDialog
{
    Q_OBJECT

public:
    explicit DoctorInformationDetail(QDialog *parent = nullptr);
    ~DoctorInformationDetail();

private:
    Ui::DoctorInformationDetail *ui;
};

#endif // DOCTORINFORMATIONDETAIL_H
