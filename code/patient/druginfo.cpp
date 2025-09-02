#include "druginfo.h"
#include "ui_druginfo.h"

DrugInfo::DrugInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DrugInfo)
{
    ui->setupUi(this);
    ui->Photo->setFixedSize(200, 200);
}

DrugInfo::~DrugInfo()
{
    delete ui;
}
