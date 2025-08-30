#ifndef ATTENDANCEPAGE_H
#define ATTENDANCEPAGE_H

#include <QWidget>

namespace Ui {
class AttendancePage;
}

class AttendancePage : public QWidget
{
    Q_OBJECT

public:
    explicit AttendancePage(QWidget *parent = nullptr);
    ~AttendancePage();

protected:
    void signIn();
    void signOut();
    void RequestLeave();

private:
    Ui::AttendancePage *ui;
};

#endif // ATTENDANCEPAGE_H
