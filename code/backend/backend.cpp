#include "backend.h"
#include "ui_backend.h"
#include "engine.h"

Backend::Backend(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Backend)
{
    ui->setupUi(this);
    connect(ui->LogoutBtn, &QPushButton::clicked, &Engine::get(), &Engine::Login);
}

Backend::~Backend()
{
    delete ui;
}
