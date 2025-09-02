#include "chatdialog.h"
#include "ui_chatdialog.h"
#include "chatbox.h"

ChatDialog::ChatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatDialog)
{
    ui->setupUi(this);
    ui->mainPanel->addWidget(new ChatBox(this));
}

ChatDialog::~ChatDialog()
{
    delete ui;
}
