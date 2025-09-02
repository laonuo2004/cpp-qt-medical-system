#include "editdoctorinfo.h"
#include "ui_editdoctorinfo.h"

#include <QFileDialog>

EditDoctorInfo::EditDoctorInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditDoctorInfo)
{
    ui->setupUi(this);
    ui->DoctorPhoto->setFixedSize(200, 250);
    connect(ui->changePhotoBtn, &QPushButton::clicked, this, &EditDoctorInfo::onChooseImage);
    connect(ui->confirmBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

EditDoctorInfo::~EditDoctorInfo()
{
    delete ui;
}

void EditDoctorInfo::onChooseImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择一张图片", "", "Images(*.png *.jpg *.jpeg *.bmp *.gif)");

    if (!fileName.isEmpty())
    {
        QPixmap pix(fileName);
        ui->DoctorPhoto->setPixmap(pix.scaled(
            ui->DoctorPhoto->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        ));
    }
}

void EditDoctorInfo::getInfoSet(QList<QString>& InfoSet)
{
    InfoSet.push_back(ui->nameInput->text());
    InfoSet.push_back(ui->idInput->text());
    InfoSet.push_back(ui->departmentInput->currentText());
    InfoSet.push_back(ui->birthdayInput->text());
    InfoSet.push_back(ui->contactInput->text());
    InfoSet.push_back(ui->descriptionInput->toPlainText());
}

QPixmap EditDoctorInfo::getImage()
{
    if (ui->DoctorPhoto->pixmap()) {
        return *ui->DoctorPhoto->pixmap();
    }
    return QPixmap();
}
