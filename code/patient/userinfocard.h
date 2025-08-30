#ifndef USERINFOCARD_H
#define USERINFOCARD_H

#include <QWidget>

namespace Ui {
class UserInfoCard;
}

class UserInfoCard : public QWidget
{
    Q_OBJECT

public:
    explicit UserInfoCard(QWidget *parent = nullptr, bool isPatient = false);
    ~UserInfoCard();

protected:
    void openDetailPanel();

private:
    Ui::UserInfoCard *ui;
    bool m_isPatient;
};

#endif // USERINFOCARD_H
