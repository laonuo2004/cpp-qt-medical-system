#include "engine.h"
#include "chooserole.h"
#include "patientclient.h"
#include "doctorclient.h"
#include "backend.h"
#include "loginpanel.h"
#include "uicontroller.h"

#include <QApplication>
#include <QDebug>
#include <QFile>

Engine::Engine(QObject *parent) : QObject(parent)
{
    m_mainWindow = nullptr;

    // 连接 UiController 的登录成功信号到 Engine 的槽函数
    UiController& controller = UiController::get();
    QObject::connect(&controller, &UiController::loginSuccessAdmin,
                     this, &Engine::onLoginSuccessAdmin);
    QObject::connect(&controller, &UiController::loginSuccessDoctor,
                     this, &Engine::onLoginSuccessDoctor);
    QObject::connect(&controller, &UiController::loginSuccessPatient,
                     this, &Engine::onLoginSuccessPatient);
}

void Engine::startEngine()
{
    if (m_mainWindow != nullptr)
    {
        m_mainWindow->close();
        delete m_mainWindow;
        m_mainWindow = nullptr;
    }
    if (m_chooseRole != nullptr)
    {
        delete m_chooseRole;
    }
    m_chooseRole = new ChooseRole;
    m_chooseRole->exec();
}

void Engine::runLoginPanel(int userType)
{
    LoginPanel* loginPanel = new LoginPanel;
    loginPanel->buildByRole(userType);
    if (loginPanel->exec() == QDialog::Accepted)
    {
        m_chooseRole->accept();
        switch(userType)
        {
            case 0:
                m_mainWindow = new PatientClient;
                break;
            case 1:
                m_mainWindow = new DoctorClient;
                break;
            case 2:
                m_mainWindow = new Backend;
                break;
        }
        if (m_mainWindow)
        {
            m_mainWindow->setAttribute(Qt::WA_DeleteOnClose);
            m_mainWindow->show();
        }
    }
}

// Engine 的槽函数：处理管理员登录成功
void Engine::onLoginSuccessAdmin()
{
    qDebug() << "Engine 收到管理员登录成功信号。正在创建 Backend 窗口。";
    if (m_mainWindow) {
        delete m_mainWindow; // 清理旧窗口，以防万一
    }
    m_mainWindow = new Backend;
    // 此时不立即 show()，让 startApplicationFlow() 在 loginPanel 销毁后统一 show。
}

// Engine 的槽函数：处理医生登录成功
void Engine::onLoginSuccessDoctor()
{
    qDebug() << "Engine 收到医生登录成功信号。正在创建 DoctorClient 窗口。";
    if (m_mainWindow) {
        delete m_mainWindow;
    }
    m_mainWindow = new DoctorClient;
}

// Engine 的槽函数：处理患者登录成功
void Engine::onLoginSuccessPatient()
{
    qDebug() << "Engine 收到患者登录成功信号。正在创建 PatientClient 窗口。";
    if (m_mainWindow) {
        delete m_mainWindow;
    }
    m_mainWindow = new PatientClient;
}

void Engine::setGlobalStyleSheet(const QString &qssFilePath)
{
    QString qss;

    // 先检查缓存
    if (m_styleCache.contains(qssFilePath)) {
        qss = m_styleCache[qssFilePath];
    } else {
        QFile file(qssFilePath);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            qWarning() << "Cannot open QSS file:" << qssFilePath;
            return;
        }
        QTextStream stream(&file);
        qss = stream.readAll();
        file.close();

        // 缓存内容
        m_styleCache[qssFilePath] = qss;
    }

    // 应用样式表
    qApp->setStyleSheet(qss);
}

void Engine::setDarkMode(bool isDarkMode)
{
    if (isDarkMode)
    {
        setGlobalStyleSheet(":/style/picture/medicaltheme_dark.qss");
    }
    else
    {
        setGlobalStyleSheet(":/style/picture/medicaltheme.qss");
    }
}

bool Engine::getCurrentMode()
{
    return isDarkMode;
}

void Engine::switchDarkMode()
{
    if (isDarkMode)
    {
        setGlobalStyleSheet(":/style/picture/medicaltheme.qss");
        isDarkMode = false;
    }
    else
    {
        setGlobalStyleSheet(":/style/picture/medicaltheme_dark.qss");
        isDarkMode = true;
    }
}
