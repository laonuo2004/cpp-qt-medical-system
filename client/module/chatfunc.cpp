#include "chatfunc.h"
#include "ui_chatfunc.h"

chatfunc::chatfunc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chatfunc)
{
    ui->setupUi(this);

    // 连接信号
    connect(ui->chat, &QPushButton::clicked, this, &chatfunc::gochat);
}

chatfunc::~chatfunc()
{
    delete ui;
}



