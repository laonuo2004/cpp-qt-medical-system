#include "payfunc.h"
#include "ui_payfunc.h"

PayFunc::PayFunc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PayFunc)
{
    ui->setupUi(this);

    // 创建页面对象，并将它们设置为PayFunc的子部件
    PayOnlinePage = new PayOnline(this);
    paymentPage = new PayPage(this);
    successpage = new PaySuccess(this);
    failpage = new PayFail(this);

    // 初始时隐藏所有页面（除了主页面）
    PayOnlinePage->hide();
    paymentPage->hide();
    successpage->hide();
    failpage->hide();

    // 设置当前页面为主页面
    currentPage = this;

    // 连接信号
    connect(ui->gopay, &QPushButton::clicked, this, &PayFunc::showPayPage);
    connect(PayOnlinePage, &PayOnline::paymentRequested, this, &PayFunc::showPaymentPage);
    connect(PayOnlinePage, &PayOnline::paymentDenied, this, &PayFunc::showWidgetPage);
    connect(paymentPage, &PayPage::paymentConfirmed, this, &PayFunc::showCompletionPage);
    connect(paymentPage, &PayPage::paymentCancelled, this, &PayFunc::showFailPage);
    connect(successpage, &PaySuccess::confirm, this, &PayFunc::showWidgetPage);
    connect(failpage, &PayFail::confirm_2, this, &PayFunc::showWidgetPage);
}

PayFunc::~PayFunc()
{
    delete ui;
}

void PayFunc::switchPage(QWidget *page)
{
    if (currentPage == page)
    {
        return; // 已经是当前页面，不需要切换
    }

    // 记录上一个页面
    previousPage = currentPage;

    // 显示新页面
    currentPage = page;
    if (currentPage)
    {
        currentPage->show();
    }

    // 隐藏上一个页面
    if (previousPage != this )
    {
        previousPage->hide();
    }
    else
    {
        ui->gopay->hide();
    }
}

// step sequence
void PayFunc::showPayPage()  // 1
{
    switchPage(PayOnlinePage) ;
}

void PayFunc::showPaymentPage()  // 2
{
    switchPage(paymentPage);
}

void PayFunc::showCompletionPage()  // 3
{
    switchPage(successpage);
}

void PayFunc::showFailPage()  // 3
{
    switchPage(failpage);
}

void PayFunc::showWidgetPage() // 4
{
    switchPage(this);
    ui->gopay->show();
}

