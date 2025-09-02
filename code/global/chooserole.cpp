#include "chooserole.h"
#include "ui_chooserole.h"
#include "engine.h"

ChooseRole::ChooseRole(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseRole)
{
    ui->setupUi(this);
    setFixedSize(1000, 600);
    connect(ui->patientBtn, &QPushButton::clicked, [this](){ Engine::get().runLoginPanel(0); });
    connect(ui->doctorBtn, &QPushButton::clicked, [this](){ Engine::get().runLoginPanel(1); });
    connect(ui->adminBtn, &QPushButton::clicked, [this](){ Engine::get().runLoginPanel(2); });
}

ChooseRole::~ChooseRole()
{
    delete ui;
}


