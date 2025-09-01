#include "chattool.h"
#include "ui_chattool.h"

chattool::chattool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chattool)
{
    ui->setupUi(this);

    connect(ui->doctor, &QPushButton::clicked, this, &chattool::doc);
    connect(ui->patient , &QPushButton::clicked , this , &chattool::patient ) ;
}

chattool::~chattool()
{
    delete ui;
}

