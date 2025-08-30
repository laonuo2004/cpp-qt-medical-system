#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>

class Engine : public QObject
{
    Q_OBJECT
public:
    explicit Engine(QObject *parent = nullptr);
    static Engine* instance();
    static void destroy();
    void Login();

private:
    static Engine* m_engine;
    class QMainWindow* m_mainWindow;
};

#endif // ENGINE_H
