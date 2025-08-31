#ifndef CHOOSEROLE_H
#define CHOOSEROLE_H

#include <QDialog>

namespace Ui {
class ChooseRole;
}

/**
 * @class ChooseRole
 * @brief 选择用户身份面板
 *
 * 该类在引擎启动时被创建，用户可选择作为医生、患者、管理员之一的身份登录。
 * 选择身份后引擎启动loginPanel面板。
 */

class ChooseRole : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseRole(QWidget *parent = nullptr);
    ~ChooseRole();

private:
    Ui::ChooseRole *ui;
};

#endif // CHOOSEROLE_H
