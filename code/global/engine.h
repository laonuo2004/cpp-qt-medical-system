#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QMainWindow>
#include "uicontroller.h"

/**
 * @class Engine
 * @brief 引擎管理类
 *
 * 该类负责系统启动、登录流程及主窗口的管理。
 * 提供了一个全局唯一的实例，方便在应用中进行统一控制。
 * 登录成功后，根据选择的用户类型（患者、医生或后台管理员）初始化不同的主窗口。
 */

class Engine : public QObject
{
    Q_OBJECT
public:
    explicit Engine(QObject *parent = nullptr);

    /**
     * @brief 获取单例对象
     *
     * 获取全局单例对象。
     */
    static Engine& get()
    {
        static Engine instance;
        return instance;
    }

    /**
     * @brief 应用程序启动流程
     *
     * 显示登录面板，等待用户输入并验证登录信息。
     * 根据登录后的选择，创建不同类型的主窗口（患者、医生或后台管理员窗口）。
     * 窗口创建后显示，并在关闭时自动删除。
     *
     * 如果登录面板返回接受（登录成功），将创建对应的客户端窗口。
     * 在主界面退出登录后会再次调用该函数，重启登录流程。
     */
    void startApplicationFlow();

private slots:
    // Engine 的槽函数：处理不同角色登录成功
    void onLoginSuccessAdmin();
    void onLoginSuccessDoctor();
    void onLoginSuccessPatient();

private:
    QMainWindow* m_mainWindow;
};

#endif // ENGINE_H