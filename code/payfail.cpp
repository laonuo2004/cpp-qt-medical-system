#include "payfail.h"
#include "ui_payfail.h"

PayFail::PayFail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PayFail)
{
    ui->setupUi(this);
    connect( ui->return_2, &QPushButton::clicked , this , &PayFail::confirm_2) ;
}

PayFail::~PayFail()
{
    delete ui;
}
