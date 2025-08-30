#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>

class Engine : public QObject
{
    Q_OBJECT
public:
    explicit Engine(QObject *parent = nullptr);
    static Engine& get()
    {
        static Engine instance;
        return instance;
    }
    void Login();


private:
    class QMainWindow* m_mainWindow;
};

#endif // ENGINE_H
