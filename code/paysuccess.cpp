#include "paysuccess.h"
#include "ui_paysuccess.h"

PaySuccess::PaySuccess(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaySuccess)
{
    ui->setupUi(this);
    connect( ui->return_2 , &QPushButton::clicked , this , &PaySuccess::confirm ) ;
}

PaySuccess::~PaySuccess()
{
    delete ui;
}
