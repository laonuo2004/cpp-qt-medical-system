#ifndef REPORTPAGE_H
#define REPORTPAGE_H

#include <QWidget>

namespace Ui {
class ReportPage;
}

class ReportPage : public QWidget
{
    Q_OBJECT

public:
    explicit ReportPage(QWidget *parent = nullptr);
    ~ReportPage();

protected:
    void loadPatientHistory();

private:
    Ui::ReportPage *ui;
};

#endif // REPORTPAGE_H
