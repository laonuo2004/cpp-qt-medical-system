#include "paypage.h"
#include "ui_paypage.h"

PayPage::PayPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PayPage)
{
    ui->setupUi(this);
    connect(ui->yes, &QPushButton::clicked, this, &PayPage::paymentConfirmed);
    connect(ui->no, &QPushButton::clicked, this, &PayPage::paymentCancelled);
}

PayPage::~PayPage()
{
    delete ui;
}
