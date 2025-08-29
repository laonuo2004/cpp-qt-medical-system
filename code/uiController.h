#ifndef UICONTROLLER_H
#define UICONTROLLER_H

#include <QString>
#include <QCryptographicHash> // 用于密码加密
#include <QVariant> // 用于数据库操作
#include <QRandomGenerator> // 用于生成随机验证码

#include "databasemanager.h" // 包含数据库管理类

// 定义用户身份枚举 (与数据库中的角色对应)
enum class UserRole {
    Admin = 0,      // 管理员
    Doctor = 1,     // 医生
    Patient = 2     // 患者
};

class UiController : public QObject
{
    Q_OBJECT
public:
    explicit UiController(QObject *parent = nullptr);

    // --- 1. 认证和用户管理后端 ---
    Q_INVOKABLE void login(const QString &email, const QString &password);
    Q_INVOKABLE void registerUser(const QString &username, const QString &email, const QString &password, UserRole role);
    Q_INVOKABLE void forgotPassword(const QString &email);
    Q_INVOKABLE void resetPassword(const QString &email, const QString &verificationCode, const QString &newPassword);
    bool isEmailRegistered(const QString &email);
    bool isPasswordComplex(const QString &password);
    // --- 2. 个人信息后端 ---
    Q_INVOKABLE QVariantMap getPatientInfo(int userId);
    Q_INVOKABLE bool updatePatientInfo(int userId, const QVariantMap &details);
    // --- 3. 挂号预约后端 ---

    // --- 4. 病历/医嘱后端 ---

    // --- 5. 预约提醒后端 ---
private:
    // 用于生成和验证密码哈希
    QString hashPassword(const QString &password);

    // 生成随机验证码
    QString generateVerificationCode();

    // 存储邮箱和对应的验证码 (模拟，实际应用中会通过邮件发送并有过期机制)
    QMap<QString, QString> m_verificationCodes; // 仍然需要这个来模拟验证码的临时存储

signals:
    // 认证信号
    void loginSuccessAdmin();
    void loginSuccessDoctor();
    void loginSuccessPatient();
    void loginFailed(const QString &reason);
    void registrationSuccess();
    void registrationFailed(const QString &reason);
    void forgotPasswordRequestSuccess(const QString &email);
    void forgotPasswordRequestFailed(const QString &reason);
    void passwordResetSuccess();
    void passwordResetFailed(const QString &reason);

};

#endif // UICONTROLLER_H
