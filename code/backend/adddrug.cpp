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

void AddDrug::on_confirmBtn_clicked()
{
    QString drug_name=ui->lineEdit->text();
    QString drug_price=ui->lineEdit_2->text();
    QString description=ui->textEdit_2->toPlainText();
    QString precaution=ui->textEdit->toPlainText();
    QString image_url=ui->photoPath->text();
    UiController::get().registerDrug(drug_name,drug_price,description,precaution,image_url);
}
