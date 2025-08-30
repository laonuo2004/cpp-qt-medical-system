#include "payonline.h"
#include "ui_payonline.h"
#include <QPixmap>
#include <QFile>
#include <QDebug>

PayOnline::PayOnline(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PayOnline)
{
    ui->setupUi(this);
    //setPaymentImage(":/new/prefix1/QR.jpg");
    connect(ui->pay, &QPushButton::clicked, this, &PayOnline::paymentRequested);
    connect(ui->back , &QPushButton::clicked , this , &PayOnline::paymentDenied ) ;
}

PayOnline::~PayOnline()
{
    delete ui;
}

