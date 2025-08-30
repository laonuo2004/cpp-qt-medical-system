#ifndef PATIENTCARDSPAGE_H
#define PATIENTCARDSPAGE_H

#include <QWidget>

namespace Ui {
class PatientCardsPage;
}

class PatientCardsPage : public QWidget
{
    Q_OBJECT

public:
    explicit PatientCardsPage(QWidget *parent = nullptr);
    ~PatientCardsPage();

protected:
    void loadPatientInformation();

private:
    Ui::PatientCardsPage *ui;
};

#endif // PATIENTCARDSPAGE_H
