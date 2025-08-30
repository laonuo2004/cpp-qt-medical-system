#include "basicfunc.h"
#include "ui_basicfunc.h"

BasicFunc::BasicFunc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BasicFunc)
{
    ui->setupUi(this);

    connect(ui->payfunc, &QPushButton::clicked, this, &BasicFunc::onShowPayFunc);
    connect(ui->chatfunc , &QPushButton::clicked , this , &BasicFunc::onShowChatFunc) ;
    connect(ui->chart , &QPushButton::clicked , this , &BasicFunc::onShowLineChartFunc ) ;

    PF = new PayFunc();
    PF->hide();  // 初始时隐藏

    CF = new chatfunc() ;
    CF->hide();

    AF = new APIGet() ;
    AF->hide() ;
}

BasicFunc::~BasicFunc()
{
    delete ui;
}

void BasicFunc::onShowPayFunc()
{
    if (PF)
    {
        PF->show();  // 显示PayFunc窗口
    }
}

void BasicFunc::onShowChatFunc()
{
    if (CF)
    {
        CF->show();  // 显示ChatFunc窗口
    }
}

void BasicFunc::onShowLineChartFunc()
{
    if (AF)
    {
        AF->show();
    }
}
