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

protected:
    void userRegister();
    void userLogin();

private:
    Ui::LoginPanel *ui;
};

#endif // LOGINPANEL_H
