#ifndef LOGINPANEL_H
#define LOGINPANEL_H

#include <QWidget>

namespace Ui {
class LoginPanel;
}

class LoginPanel : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPanel(QWidget *parent = nullptr);
    ~LoginPanel();

private slots:
    void on_RegisterBtn_clicked();

private:
    Ui::LoginPanel *ui;
};

#endif // LOGINPANEL_H
