#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include "networkmanager_client.h"

/**
 * @brief 网络客户端测试程序
 * 
 * 用于测试客户端与服务器之间的网络通信
 */
class NetworkClientTest : public QObject
{
    Q_OBJECT

public:
    NetworkClientTest(QObject* parent = nullptr) : QObject(parent)
    {
        m_client = new NetworkManagerClient(this);
        
        // 连接信号
        connect(m_client, &NetworkManagerClient::connectionStatusChanged, 
                this, &NetworkClientTest::onConnectionStatusChanged);
        connect(m_client, &NetworkManagerClient::networkError,
                this, &NetworkClientTest::onNetworkError);
    }

public slots:
    void runTests()
    {
        qInfo() << "=== 开始网络通信测试 ===";
        
        // 连接到服务器
        if (m_client->connectToServer("localhost", 12345)) {
            qInfo() << "正在连接服务器...";
            // 等待连接建立后再进行测试
            QTimer::singleShot(2000, this, &NetworkClientTest::testLogin);
        } else {
            qCritical() << "连接服务器失败";
            QCoreApplication::quit();
        }
    }

private slots:
    void onConnectionStatusChanged(bool connected)
    {
        if (connected) {
            qInfo() << "✅ 成功连接到服务器";
        } else {
            qWarning() << "❌ 服务器连接断开";
        }
    }

    void onNetworkError(const QString& error)
    {
        qWarning() << "网络错误:" << error;
    }

    void testLogin()
    {
        qInfo() << "\n--- 测试用户登录 ---";
        
        // 准备登录参数
        QVariantMap params;
        params["email"] = "test@example.com";
        params["password"] = "testpassword";
        
        // 发送同步请求
        QVariantMap response = m_client->sendRequestSync("login", params, 5000);
        
        if (response.value("success").toBool()) {
            qInfo() << "✅ 登录测试成功";
            qInfo() << "响应数据:" << response;
        } else {
            qWarning() << "❌ 登录测试失败:" << response.value("error").toString();
        }
        
        // 测试异步请求
        QTimer::singleShot(1000, this, &NetworkClientTest::testAsyncRequest);
    }

    void testAsyncRequest()
    {
        qInfo() << "\n--- 测试异步请求 ---";
        
        QVariantMap params;
        params["userId"] = 1;
        
        m_client->sendRequestAsync("getPatientInfo", params, 
                                  [this](bool success, const QVariantMap& response) {
            if (success) {
                qInfo() << "✅ 异步请求成功";
                qInfo() << "响应数据:" << response;
            } else {
                qWarning() << "❌ 异步请求失败:" << response.value("error").toString();
            }
            
            // 测试完成，退出程序
            QTimer::singleShot(1000, []() {
                qInfo() << "\n=== 测试完成 ===";
                QCoreApplication::quit();
            });
        });
    }

private:
    NetworkManagerClient* m_client;
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    // 设置应用程序信息
    app.setApplicationName("Network Client Test");
    app.setApplicationVersion("1.0");
    
    qInfo() << "智慧医疗系统 - 网络客户端测试程序";
    qInfo() << "版本: 1.0";
    
    // 创建测试对象
    NetworkClientTest test;
    
    // 开始测试
    QTimer::singleShot(1000, &test, &NetworkClientTest::runTests);
    
    return app.exec();
}

#include "test_network_client.moc"
