#ifndef ADDDEPARTMENT_H
#define ADDDEPARTMENT_H

#include <QDialog>
#include "global/uicontroller.h"
#include "databasemanager.h"

namespace Ui {
class AddDepartment;
}

/**
 * @class AddDepartment
 * @brief 添加科室表单控件
 *
 * 点击添加科室按钮后，弹出此表单。
 * 点击确认后，若添加成功，则显示弹窗。
 */

class AddDepartment : public QDialog
{
    Q_OBJECT

public:
    explicit AddDepartment(QWidget *parent = nullptr);
    ~AddDepartment();

private slots:
    void on_confirmBtn_clicked();

private:
    Ui::AddDepartment *ui;
};

#endif // ADDDEPARTMENT_H
