#ifndef DOCTORINFORMATIONWIDGET_H
#define DOCTORINFORMATIONWIDGET_H

#include <QWidget>

namespace Ui {
class DoctorInformationWidget;
}

class DoctorInformationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DoctorInformationWidget(QWidget *parent = nullptr);
    ~DoctorInformationWidget();

private:
    Ui::DoctorInformationWidget *ui;
};

#endif // DOCTORINFORMATIONWIDGET_H
