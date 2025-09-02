#include "backend.h"
#include "ui_backend.h"
#include "engine.h"
#include "adduser.h"
#include "adddrug.h"
#include "adddepartment.h"

#include <QMessageBox>

Backend::Backend(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Backend)
{
    ui->setupUi(this);
    // 点击“退出登录”按钮，关闭主窗口，打开登录界面
    connect(ui->LogoutBtn, &QPushButton::clicked, &Engine::get(), &Engine::startEngine);
    connect(ui->addPatientAction, &QAction::triggered, [this](){ addUser(0); });
    connect(ui->addDoctorAction, &QAction::triggered, [this](){ addUser(1); });
    connect(ui->addAdminAction, &QAction::triggered, [this](){ addUser(2); });
    connect(ui->addDrug, &QAction::triggered, [this](){ openDialog(new AddDrug, "添加药品", "药品添加成功！"); });
    connect(ui->addDepartment, &QAction::triggered, [this](){ openDialog(new AddDepartment, "添加部门", "部门添加成功！"); });
}

Backend::~Backend()
{
    delete ui;
}

void Backend::addUser(int userRole)
{
    AddUser* newAddUserPanel = new AddUser(this);
    newAddUserPanel->buildByRole(userRole);
    if (newAddUserPanel->exec() == QDialog::Accepted)
    {
        QMessageBox::information(this, tr("添加用户"), tr("新用户添加成功！"));
    }
    delete newAddUserPanel;
}

void Backend::openDialog(QDialog* dialog, const QString& msgTitle, const QString& msgContent)
{
    if (dialog->exec() == QDialog::Accepted)
    {
        QMessageBox::information(this, msgTitle, msgContent);
    }
    delete dialog;
}
