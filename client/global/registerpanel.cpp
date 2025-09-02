#include "registerpanel.h"
#include "ui_registerpanel.h"
#include <QDebug>
#include <QMessageBox>

RegisterPanel::RegisterPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterPanel),
    m_controller(UiController::get())
{
    ui->setupUi(this);
    
    // 连接 UiController 的注册信号
    connect(&m_controller, &UiController::registrationSuccess, this, &RegisterPanel::handleRegistrationSuccess);
    connect(&m_controller, &UiController::registrationFailed, this, &RegisterPanel::handleRegistrationFailed);
    connect(ui->confirmBtn, &QPushButton::clicked, this, &RegisterPanel::handleRegister);
}

RegisterPanel::~RegisterPanel()
{
    delete ui;
}

void RegisterPanel::handleRegister()
{
    QString username = ui->UserNameEdit->text();
    QString email = ui->EmailEdit->text();
    QString password = ui->PasswordEdit->text();
    // 从 QComboBox 中获取当前选中项的自定义数据
    m_controller.registerUser(username, email, password, UserRole::Patient);
}

// 槽函数：处理注册成功
void RegisterPanel::handleRegistrationSuccess()
{
    qDebug() << "User registered successfully!";
    QMessageBox::information(this, "注册成功", "恭喜，用户注册成功！");
    // 如果 RegisterPanel 是一个 QDialog，成功后可以关闭它
    accept(); // 或者 close();
    // 如果它是一个 QWidget 且不是独立的窗口，你可能需要父窗口来切换界面
}

// 槽函数：处理注册失败
void RegisterPanel::handleRegistrationFailed(const QString &reason)
{
    qDebug() << "User registration failed:" << reason;
    // 注册失败时，显示错误信息，并保持当前界面（RegisterPanel）打开
    QMessageBox::warning(this, "注册失败", "注册失败，原因：" + reason);

    // 此时 RegisterPanel 界面会保持不变，用户可以修改输入再次尝试
}
