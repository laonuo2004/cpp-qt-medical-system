#include "adddrug.h"
#include "ui_adddrug.h"

#include <QFileDialog>

AddDrug::AddDrug(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDrug)
{
    ui->setupUi(this);
    connect(ui->confirmBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->addPhoto, &QPushButton::clicked, this, &AddDrug::addPhoto);
}

AddDrug::~AddDrug()
{
    delete ui;
}

void AddDrug::addPhoto()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择一张图片", "", "Images(*.png *.jpg *.jpeg *.bmp *.gif)");
    ui->photoPath->setText(fileName);
}
