#include "uicontroller.h"
#include <QDebug>
#include <QDateTime> // 用于验证码的过期时间（可选，但推荐）

UiController::UiController(QObject *parent) : QObject(parent)
{
    // 确保数据库连接已初始化并创建了表
    if (!DatabaseManager::instance().isConnected()) {
        qCritical() << "Database not connected!";
        // 可以发出一个全局错误信号或者在UI上显示错误
    }
}
// --- 1. 认证和用户管理后端 ---
// 登录验证方法
void UiController::login(const QString &email, const QString &password)
{
    if (email.isEmpty() || password.isEmpty()) {
        emit loginFailed("邮箱或密码不能为空。");
        return;
    }

    if (!DatabaseManager::instance().isConnected()) {
        emit loginFailed("数据库未连接。");
        return;
    }

    QString sql = QString("SELECT username, password_hash, role FROM users WHERE email = '%1'").arg(email);
    DatabaseManager::ResultSet result = DatabaseManager::instance().query(sql);

    if (result.empty()) {
        emit loginFailed("邮箱未注册，请先注册。");
        return;
    }

    // 找到了用户
    DatabaseManager::DataRow userRow = result[0];
    QString storedPasswordHash = userRow["password_hash"].toString();
    int roleInt = userRow["role"].toInt(); // 从数据库获取角色整数值
    UserRole userRole = static_cast<UserRole>(roleInt);

    if (storedPasswordHash == hashPassword(password)) {
        // 登录成功
        qDebug() << "User" << email << "logged in successfully as" << static_cast<int>(userRole);
        switch (userRole) {
            case UserRole::Admin:
                emit loginSuccessAdmin();
                break;
            case UserRole::Doctor:
                emit loginSuccessDoctor();
                break;
            case UserRole::Patient:
                emit loginSuccessPatient();
                break;
            default:
                emit loginFailed("未知用户角色。"); // 处理未定义的角色
                break;
        }
    } else {
        emit loginFailed("密码不正确。");
    }
}

// 注册方法
void UiController::registerUser(const QString &username, const QString &email, const QString &password, UserRole role)
{
    if (username.isEmpty() || email.isEmpty() || password.isEmpty()) {
        emit registrationFailed("用户名、邮箱或密码不能为空。");
        return;
    }

    if (!DatabaseManager::instance().isConnected()) {
        emit registrationFailed("数据库未连接。");
        return;
    }

    if (isEmailRegistered(email)) {
        emit registrationFailed("该邮箱已被注册。");
        return;
    }

    if (!isPasswordComplex(password)) {
        emit registrationFailed("密码不符合复杂度要求：至少包含8位，大小写字母，数字和特殊字符。");
        return;
    }

    DatabaseManager::DataRow userData;
    userData["username"] = username;
    userData["email"] = email;
    userData["password_hash"] = hashPassword(password);
    userData["role"] = static_cast<int>(role); // 将枚举转换为整数存储

    if (DatabaseManager::instance().insert("users", userData)) {
        qDebug() << "User" << email << "registered successfully.";
        emit registrationSuccess();
    } else {
        qCritical() << "Failed to register user:" << DatabaseManager::instance().lastError();
        emit registrationFailed("注册失败：" + DatabaseManager::instance().lastError());
    }
}

// 检查邮箱是否已注册
bool UiController::isEmailRegistered(const QString &email)
{
    if (!DatabaseManager::instance().isConnected()) {
        qWarning() << "Database not connected for email check.";
        return false; // 如果数据库未连接，暂时认为未注册
    }

    QString sql = QString("SELECT COUNT(*) FROM users WHERE email = '%1'").arg(email);
    DatabaseManager::ResultSet result = DatabaseManager::instance().query(sql);

    if (result.empty()) {
        qWarning() << "isEmailRegistered query returned empty result.";
        return false;
    }

    // 获取第一行第一列的值（COUNT(*)）
    return result[0].begin()->second.toInt() > 0;
}

// 检查密码复杂度
bool UiController::isPasswordComplex(const QString &password)
{
    // 至少8位
    if (password.length() < 8) return false;

    bool hasLower = false;
    bool hasUpper = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    // 检查大小写字母、数字、特殊字符
    for (QChar c : password) {
        if (c.isLower()) hasLower = true;
        else if (c.isUpper()) hasUpper = true;
        else if (c.isDigit()) hasDigit = true;
        else if (!c.isLetterOrNumber()) hasSpecial = true; // 认为非字母数字即为特殊字符
    }

    return hasLower && hasUpper && hasDigit && hasSpecial;
}

// 找回密码请求
void UiController::forgotPassword(const QString &email)
{
    if (email.isEmpty()) {
        emit forgotPasswordRequestFailed("邮箱不能为空。");
        return;
    }

    if (!DatabaseManager::instance().isConnected()) {
        emit forgotPasswordRequestFailed("数据库未连接。");
        return;
    }

    if (!isEmailRegistered(email)) {
        emit forgotPasswordRequestFailed("该邮箱未注册。");
        return;
    }

    // 模拟发送验证码到邮箱
    QString verificationCode = generateVerificationCode();
    // 存储验证码，可以考虑存储一个时间戳，用于判断验证码是否过期
    m_verificationCodes.insert(email, verificationCode);

    qDebug() << "Verification code for" << email << ":" << verificationCode;
    // 实际应用中，这里会调用邮件发送服务
    emit forgotPasswordRequestSuccess(email);
}

// 验证验证码并重置密码
void UiController::resetPassword(const QString &email, const QString &verificationCode, const QString &newPassword)
{
    if (email.isEmpty() || verificationCode.isEmpty() || newPassword.isEmpty()) {
        emit passwordResetFailed("邮箱、验证码或新密码不能为空。");
        return;
    }

    if (!DatabaseManager::instance().isConnected()) {
        emit passwordResetFailed("数据库未连接。");
        return;
    }

    if (!m_verificationCodes.contains(email) || m_verificationCodes.value(email) != verificationCode) {
        emit passwordResetFailed("验证码不正确或已过期。");
        return;
    }

    if (!isPasswordComplex(newPassword)) {
        emit passwordResetFailed("新密码不符合复杂度要求：至少包含8位，大小写字母，数字和特殊字符。");
        return;
    }

    DatabaseManager::DataRow updateData;
    updateData["password_hash"] = hashPassword(newPassword);
    QString whereClause = QString("email = '%1'").arg(email);

    if (DatabaseManager::instance().update("users", updateData, whereClause)) {
        m_verificationCodes.remove(email); // 移除已使用的验证码
        qDebug() << "Password for" << email << "reset successfully.";
        emit passwordResetSuccess();
    } else {
        qCritical() << "Failed to reset password:" << DatabaseManager::instance().lastError();
        emit passwordResetFailed("密码重置失败：" + DatabaseManager::instance().lastError());
    }
}

// 用于生成和验证密码哈希
QString UiController::hashPassword(const QString &password)
{
    // 同样，生产环境中请使用更安全的哈希算法和盐
    QByteArray passwordBytes = password.toUtf8();
    return QString(QCryptographicHash::hash(passwordBytes, QCryptographicHash::Sha256).toHex());
}

// 生成随机验证码
QString UiController::generateVerificationCode()
{
    const QString possibleCharacters("0123456789"); // 验证码通常是数字
    const int codeLength = 6;
    QString randomString;
    for (int i = 0; i < codeLength; ++i) {
        int index = QRandomGenerator::global()->bounded(possibleCharacters.length());
        randomString.append(possibleCharacters.at(index));
    }
    return randomString;
}
// --- 2. 个人信息后端 ---
QVariantMap UiController::getPatientInfo(int userId)
{
    if (!DatabaseManager::instance().isConnected()) {
        qCritical() << "Get patient info failed: Database not connected.";
        return {};
    }

    // 使用 LEFT JOIN 联合查询，确保即使用户还没有详细信息也能查出用户名
    QString sql = QString(
        "SELECT u.username, pd.* "
        "FROM users u "
        "LEFT JOIN patient_details pd ON u.id = pd.user_id "
        "WHERE u.id = %1"
    ).arg(userId);

    DatabaseManager::ResultSet result = DatabaseManager::instance().query(sql);

    if (result.empty()) {
        qWarning() << "No user found with ID:" << userId;
        return {}; // 没有找到用户
    }

    // 将 std::map 转换为 QVariantMap 以便在QML中使用
    QVariantMap patientInfo;
    // 使用 result.front() 或 result[0]
    for(const auto& pair : result.front()) {
        patientInfo[pair.first] = pair.second;
    }

    return patientInfo;
}

bool UiController::updatePatientInfo(int userId, const QVariantMap &details)
{
    if (!DatabaseManager::instance().isConnected()) {
        qCritical() << "Update patient info failed: Database not connected.";
        return false;
    }

    // 开启事务，确保数据一致性
    if (!DatabaseManager::instance().beginTransaction()) {
        qCritical() << "Failed to begin transaction:" << DatabaseManager::instance().lastError();
        return false;
    }

    // 1. 更新 users 表中的姓名 (username)
    if (details.contains("username")) {
        DatabaseManager::DataRow userData;
        userData["username"] = details["username"];
        QString userWhereClause = QString("id = %1").arg(userId);
        if (!DatabaseManager::instance().update("users", userData, userWhereClause)) {
            qCritical() << "Failed to update username:" << DatabaseManager::instance().lastError();
            DatabaseManager::instance().rollbackTransaction();
            return false;
        }
    }

    // 2. 更新或插入 patient_details 表
    DatabaseManager::DataRow patientDetailsData;
    // 遍历传入的数据，填充到 DataRow 中
    for(auto it = details.constBegin(); it != details.constEnd(); ++it) {
        // "username" 字段不属于 patient_details 表，跳过
        if (it.key() != "username") {
            patientDetailsData[it.key()] = it.value();
        }
    }

    // 检查 patient_details 中是否已存在该用户的记录
    QString checkSql = QString("SELECT COUNT(*) as count FROM patient_details WHERE user_id = %1").arg(userId);
    DatabaseManager::ResultSet result = DatabaseManager::instance().query(checkSql);
    bool detailsExist = !result.empty() && result.front()["count"].toInt() > 0;

    bool success;
    if (detailsExist) {
        // 记录存在，执行 UPDATE
        QString patientWhereClause = QString("user_id = %1").arg(userId);
        success = DatabaseManager::instance().update("patient_details", patientDetailsData, patientWhereClause);
    } else {
        // 记录不存在，执行 INSERT
        patientDetailsData["user_id"] = userId; // 必须插入 user_id
        success = DatabaseManager::instance().insert("patient_details", patientDetailsData);
    }

    if (!success) {
        qCritical() << "Failed to upsert patient_details:" << DatabaseManager::instance().lastError();
        DatabaseManager::instance().rollbackTransaction();
        return false;
    }

    // 所有操作成功，提交事务
    qInfo() << "Patient info for user" << userId << "updated successfully.";
    return DatabaseManager::instance().commitTransaction();
}
    // --- 3. 挂号预约后端 ---
