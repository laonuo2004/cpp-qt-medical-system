#ifndef DOCTORINFOPAGE_H
#define DOCTORINFOPAGE_H

#include <QWidget>

namespace Ui {
class DoctorInfoPage;
}

class DoctorInfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit DoctorInfoPage(QWidget *parent = nullptr);
    ~DoctorInfoPage();

private:
    Ui::DoctorInfoPage *ui;
};

#endif // DOCTORINFOPAGE_H
