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
    QObject::connect(&m_controller, &UiController::registrationSuccess,
                     this, &RegisterPanel::handleRegistrationSuccess);
    QObject::connect(&m_controller, &UiController::registrationFailed,
                     this, &RegisterPanel::handleRegistrationFailed);
    
    // 清空现有项目 (可选)
    ui->RoleComboBox->clear();
    
    // 添加角色选项并设置自定义数据
    ui->RoleComboBox->addItem("管理员", QVariant::fromValue(UserRole::Admin));
    ui->RoleComboBox->addItem("医生", QVariant::fromValue(UserRole::Doctor));
    ui->RoleComboBox->addItem("患者", QVariant::fromValue(UserRole::Patient));
}

RegisterPanel::~RegisterPanel()
{
    delete ui;
}

void RegisterPanel::handleRegister()
{
    // This method is kept for backward compatibility but not used
    // The actual registration is handled by on_buttonBox_accepted()
}

void RegisterPanel::on_buttonBox_accepted()
{
    QString username = ui->UserNameEdit->text();
    QString email = ui->EmailEdit->text();
    QString password = ui->PasswordEdit->text();
    // 从 QComboBox 中获取当前选中项的自定义数据
    QVariant selectedRoleData = ui->RoleComboBox->currentData();

    // 检查是否成功获取到数据，并尝试转换为 UserRole
    if (selectedRoleData.isValid() && selectedRoleData.canConvert<UserRole>())
    {
        UserRole role = selectedRoleData.value<UserRole>();

        // Direct call from C++
        m_controller.registerUser(username, email, password, role);
    }
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
