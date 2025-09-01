#include "payinterface.h"
#include "ui_payinterface.h"

PayInterface::PayInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PayInterface)
{
    ui->setupUi(this);

    stackedPages = new QStackedWidget(this) ;

    payfuncPage = new PayFunc(this) ;
    payOnlinePage = new PayOnline(this);
    paymentPage   = new PayPage(this);
    successPage   = new PaySuccess(this);
    failPage      = new PayFail(this);

    stackedPages->addWidget(payfuncPage);     // 索引 0 = “主”视图
    stackedPages->addWidget(payOnlinePage);   // 索引 1
    stackedPages->addWidget(paymentPage);     // 索引 2
    stackedPages->addWidget(successPage);     // 索引 3
    stackedPages->addWidget(failPage);        // 索引 4

    stackedPages->setCurrentWidget(payfuncPage) ;
    // 始终从 0（你的“this”页面）开始，gopay 在“this”页面上

    // 通过直接更改堆叠索引来连接导航
    connect(payfuncPage, &PayFunc::gopay,
            [=](){ stackedPages->setCurrentWidget(payOnlinePage); });

    connect(payOnlinePage, &PayOnline::paymentRequested,
            [=](){ stackedPages->setCurrentWidget(paymentPage); });

    connect(payOnlinePage, &PayOnline::paymentDenied,
            [=](){ stackedPages->setCurrentWidget(payfuncPage); });

    connect(successPage, &PaySuccess::confirm,
            [=](){ stackedPages->setCurrentWidget(payfuncPage); });

    connect(failPage, &PayFail::confirm_2,
            [=](){ stackedPages->setCurrentWidget(payfuncPage); });

}

PayInterface::~PayInterface()
{
    delete ui;
}
