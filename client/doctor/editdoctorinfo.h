#ifndef EDITDOCTORINFO_H
#define EDITDOCTORINFO_H

#include <QDialog>

namespace Ui {
class EditDoctorInfo;
}

/**
 * @class EditDoctorInfo
 * @brief 修改医生个人信息的弹窗
 *
 * 在医生个人信息界面点击“修改个人信息”，创建此弹窗。
 * 输入新的个人信息后点击提交，更新医生的个人信息界面。
 */

class EditDoctorInfo : public QDialog
{
    Q_OBJECT

public:
    explicit EditDoctorInfo(QWidget *parent = nullptr);
    ~EditDoctorInfo();

    void getInfoSet(QList<QString>& InfoSet);

    QPixmap getImage();

protected:

    /**
     * @brief 选择医生新的照片
     *
     * 打开资源管理器，选中照片后，自动更新当前编辑页面的照片。
     */
    void onChooseImage();

private:
    Ui::EditDoctorInfo *ui;
};

#endif // EDITDOCTORINFO_H
