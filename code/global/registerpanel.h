#ifndef REGISTERPANEL_H
#define REGISTERPANEL_H

#include <QDialog>

namespace Ui {
class RegisterPanel;
}

class RegisterPanel : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterPanel(QWidget *parent = nullptr);
    ~RegisterPanel();

private:
    Ui::RegisterPanel *ui;
};

#endif // REGISTERPANEL_H
