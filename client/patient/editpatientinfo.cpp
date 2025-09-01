#include "editpatientinfo.h"
#include "ui_editpatientinfo.h"

#include <QFileDialog>

EditPatientInfo::EditPatientInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditPatientInfo)
{
    ui->setupUi(this);
    ui->PatientPhoto->setFixedSize(200, 250);
    connect(ui->changePhotoBtn, &QPushButton::clicked, this, &EditPatientInfo::onChooseImage);
    connect(ui->confirmBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

EditPatientInfo::~EditPatientInfo()
{
    delete ui;
}

void EditPatientInfo::onChooseImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择一张图片", "", "Images(*.png *.jpg *.jpeg *.bmp *.gif)");

    if (!fileName.isEmpty())
    {
        QPixmap pix(fileName);
        ui->PatientPhoto->setPixmap(pix.scaled(
            ui->PatientPhoto->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        ));
    }
}

void EditPatientInfo::getInfoSet(QList<QString>& InfoSet)
{
    InfoSet.push_back(ui->nameInput->text());
    InfoSet.push_back(ui->idInput->text());
    InfoSet.push_back(ui->genderInput->currentText());
    InfoSet.push_back(ui->birthdayInput->text());
    InfoSet.push_back(ui->ageInput->text());
    InfoSet.push_back(ui->contactInput->text());
    InfoSet.push_back(ui->emailInput->text());
    InfoSet.push_back(ui->emerNameInput->text());
    InfoSet.push_back(ui->emerContactInput->text());
    InfoSet.push_back(ui->historyInput->toPlainText());
}

QPixmap EditPatientInfo::getImage()
{
    if (ui->PatientPhoto->pixmap()) {
        return *ui->PatientPhoto->pixmap();
    }
    return QPixmap();
}
