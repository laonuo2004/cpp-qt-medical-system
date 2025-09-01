#include "chatinterface.h"
#include "ui_chatinterface.h"

ChatInterface::ChatInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatInterface)
{
    ui->setupUi(this);
    stack_ = new QStackedWidget(this)  ;

    // 创建你的页面
    chat = new chatfunc(this) ;
    Chatpage = new chattool(this);
    DC       = new DoctorChatTool(this);
    PC       = new PatientChatTool(this);

    // 添加到堆栈
    stack_->addWidget(chat);       // 索引 0 = 主按钮面板
    stack_->addWidget(Chatpage);       // 索引 1
    stack_->addWidget(DC);             // 索引 2
    stack_->addWidget(PC);             // 索引 3

    stack_->setCurrentWidget(chat) ;

    // 直接连接按钮以显示页面
    connect(chat, &chatfunc::gochat,    [this]{ stack_->setCurrentWidget(Chatpage); });
    //connect(ui->chat, &QPushButton::clicked,    [this]{ /* 或返回索引 0 */ });
    connect(Chatpage, &chattool::doc,           [this]{ stack_->setCurrentWidget(DC); });
    connect(Chatpage, &chattool::patient,       [this]{ stack_->setCurrentWidget(PC); });

}

ChatInterface::~ChatInterface()
{
    delete ui;
}
