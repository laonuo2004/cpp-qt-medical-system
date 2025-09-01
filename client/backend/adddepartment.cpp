#include "adddepartment.h"
#include "ui_adddepartment.h"

AddDepartment::AddDepartment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDepartment)
{
    ui->setupUi(this);
    connect(ui->confirmBtn, &QPushButton::clicked, this, &QDialog::accept);
}

AddDepartment::~AddDepartment()
{
    delete ui;
}
