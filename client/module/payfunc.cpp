#include "payfunc.h"
#include "ui_payfunc.h"

PayFunc::PayFunc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PayFunc)
{
    ui->setupUi(this);
    connect(ui->gopay , &QPushButton::clicked , this , &PayFunc::gopay );
}

PayFunc::~PayFunc()
{
    delete ui;
}

