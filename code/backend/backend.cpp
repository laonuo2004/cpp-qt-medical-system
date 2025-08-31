#include "backend.h"
#include "ui_backend.h"
#include "engine.h"

Backend::Backend(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Backend)
{
    ui->setupUi(this);
    // 点击“退出登录”按钮，关闭主窗口，打开登录界面
    connect(ui->LogoutBtn, &QPushButton::clicked, &Engine::get(), &Engine::startApplicationFlow);
}

Backend::~Backend()
{
    delete ui;
}
