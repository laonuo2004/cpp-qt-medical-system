#ifndef DIAGNOSE_H
#define DIAGNOSE_H

#include <QDialog>

namespace Ui {
class Diagnose;
}

/**
 * @class Diagnose
 * @brief 医生诊断界面
 *
 * 在患者界面点击患者卡片，点击详细信息中“诊断”按钮进入此界面。
 */

class Diagnose : public QDialog
{
    Q_OBJECT

public:
    explicit Diagnose(QWidget *parent = nullptr);
    ~Diagnose();

private:
    Ui::Diagnose *ui;
};

#endif // DIAGNOSE_H
