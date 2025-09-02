#ifndef EDITPATIENTINFO_H
#define EDITPATIENTINFO_H

#include <QDialog>

namespace Ui {
class EditPatientInfo;
}

/**
 * @class EditPatientInfo
 * @brief 修改患者个人信息的弹窗
 *
 * 在患者个人信息界面点击“修改个人信息”，创建此弹窗。
 * 输入新的个人信息后点击提交，更新患者的个人信息界面。
 */

class EditPatientInfo : public QDialog
{
    Q_OBJECT

public:
    explicit EditPatientInfo(QWidget *parent = nullptr);
    ~EditPatientInfo();

    void getInfoSet(QList<QString>& InfoSet);

    QPixmap getImage();

protected:

    /**
     * @brief 选择患者新的照片
     *
     * 打开资源管理器，选中照片后，自动更新当前编辑页面的照片。
     */
    void onChooseImage();

private:
    Ui::EditPatientInfo *ui;
};

#endif // EDITPATIENTINFO_H
