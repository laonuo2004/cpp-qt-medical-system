#include "chatfunc.h"
#include "ui_chatfunc.h"

chatfunc::chatfunc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chatfunc)
{
    ui->setupUi(this);
    Chatpage = new chattool(this);
    DC = new DoctorChatTool(this);
    PC = new PatientChatTool(this);

    Chatpage->hide();
    DC->hide() ;
    PC->hide() ;

    currentPage = this ;
    // 连接信号
    connect(ui->chat, &QPushButton::clicked, this, &chatfunc::showChatPage);
    connect(Chatpage , &chattool::doc , this , &chatfunc::showDocChatPage);
    connect(Chatpage, &chattool::patient , this , &chatfunc::showPatientChatPage);
}

chatfunc::~chatfunc()
{
    delete ui;
}

void chatfunc::switchPage(QWidget *page)
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
        ui->chat->hide();
    }
}

// step sequence
void chatfunc::showChatPage()  // 1
{
    switchPage(Chatpage);
}

void chatfunc::showDocChatPage()  // 1
{
    switchPage(DC) ;
}

void chatfunc::showPatientChatPage()  // 1
{
    switchPage(PC) ;
}

