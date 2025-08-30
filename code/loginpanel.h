#ifndef LOGINPANEL_H
#define LOGINPANEL_H

#include <QDialog>

namespace Ui {
class LoginPanel;
}

class LoginPanel : public QDialog
{
    Q_OBJECT

signals:
    void loginSuccessful();

public:
    explicit LoginPanel(QWidget *parent = nullptr);
    int getMainWindowType();
    ~LoginPanel();

private slots:
    void on_RegisterBtn_clicked();

    void on_LoginBtn_clicked();

private:
    Ui::LoginPanel *ui;
};

#endif // LOGINPANEL_H
