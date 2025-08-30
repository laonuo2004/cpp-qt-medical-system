#ifndef REGISTERPANEL_H
#define REGISTERPANEL_H

#include <QDialog>

namespace Ui {
class RegisterPanel;
}

/**
 * @class RegisterPanel
 * @brief 注册面板
 *
 * 该类负责处理用户注册操作，未完成。
 */

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
