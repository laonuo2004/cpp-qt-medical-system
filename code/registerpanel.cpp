#include "registerpanel.h"
#include "ui_registerpanel.h"
#include <QDebug>

RegisterPanel::RegisterPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterPanel)
{
    ui->setupUi(this);
}

RegisterPanel::~RegisterPanel()
{
}
