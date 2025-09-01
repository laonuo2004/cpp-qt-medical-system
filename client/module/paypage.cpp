#include "paypage.h"
#include "ui_paypage.h"

PayPage::PayPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PayPage)
{
    ui->setupUi(this);
    ui->QRCode->setFixedSize(200, 200);
    connect(ui->confirmBtn, &QPushButton::clicked, this, &QDialog::accept);
}

PayPage::~PayPage()
{
    delete ui;
}
