#ifndef ADDDRUG_H
#define ADDDRUG_H

#include <QDialog>
#include "global/uicontroller.h"

namespace Ui {
class AddDrug;
}

/**
 * @class AddDrug
 * @brief 添加药品表单控件
 *
 * 点击添加药品按钮后，弹出此表单。
 * 点击确认后，若添加成功，则显示弹窗。
 */

class AddDrug : public QDialog
{
    Q_OBJECT

public:
    explicit AddDrug(QWidget *parent = nullptr);
    ~AddDrug();

protected:

    /**
     * @brief 添加药品图片
     *
     * 打开资源管理器添加药品图片。
     * 将药品图片的链接显示在下方label上。
     */
    void addPhoto();

private slots:
    void on_confirmBtn_clicked();

private:
    Ui::AddDrug *ui;
};

#endif // ADDDRUG_H
