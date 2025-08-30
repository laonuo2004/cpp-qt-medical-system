#ifndef PATIENTINFOPAGE_H
#define PATIENTINFOPAGE_H

#include <QWidget>

namespace Ui {
class PatientInfoPage;
}

class PatientInfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit PatientInfoPage(QWidget *parent = nullptr);
    ~PatientInfoPage();

private:
    Ui::PatientInfoPage *ui;
};

#endif // PATIENTINFOPAGE_H
