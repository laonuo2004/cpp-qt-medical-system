#include "basicfunc.h"
#include "ui_basicfunc.h"

BasicFunc::BasicFunc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BasicFunc)
{
    ui->setupUi(this);

    connect(ui->payfunc, &QPushButton::clicked, this, &BasicFunc::onShowPayInterface);
    connect(ui->chatfunc , &QPushButton::clicked , this , &BasicFunc::onShowChatInterface) ;
    connect(ui->chart , &QPushButton::clicked , this , &BasicFunc::onShowLineChartFunc ) ;

    CF = new ChatInterface() ;
    CF->hide();

    AF = new APIGet() ;
    AF->hide() ;
}

BasicFunc::~BasicFunc()
{
    delete ui;
}

void BasicFunc::onShowPayInterface()
{
    if (PF)
    {
        PF->show();  // 显示PayINterface窗口
    }
}

void BasicFunc::onShowChatInterface()
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
