#include "diagnose.h"
#include "ui_diagnose.h"

Diagnose::Diagnose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Diagnose)
{
    ui->setupUi(this);
    connect(ui->saveBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->confirmBtn, &QPushButton::clicked, this, &QDialog::accept);
}

Diagnose::~Diagnose()
{
    delete ui;
}
