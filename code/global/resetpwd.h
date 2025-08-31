#ifndef RESETPWD_H
#define RESETPWD_H

#include <QDialog>
#include "uicontroller.h"

namespace Ui {
class ResetPwd;
}

/**
 * @class ResetPwd
 * @brief 重置密码面板
 *
 * 该类负责处理用户重置密码操作。
 */

class ResetPwd : public QDialog
{
    Q_OBJECT

public:
    explicit ResetPwd(QWidget *parent = nullptr);
    ~ResetPwd();

private slots:
    void handleReset();
    void onPasswordResetSuccess();
    void onPasswordResetFailed(const QString &reason);

private:
    Ui::ResetPwd *ui;
    UiController& m_controller;
};

#endif // RESETPWD_H
