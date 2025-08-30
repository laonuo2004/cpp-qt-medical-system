#include "engine.h"
#include "loginpanel.h"
#include "patientclient.h"
#include "doctorclient.h"
#include "manager.h"

#include <QApplication>
#include <QMainWindow>
#include <QDialog>
#include <QDebug>


Engine::Engine(QObject *parent) : QObject(parent)
{
    m_mainWindow = nullptr;
}

Engine *Engine::m_engine = nullptr;

Engine *Engine::instance()
{
    if (!m_engine)
    {
        m_engine = new Engine;
    }
    return m_engine;
}

void Engine::destroy()
{
    if (m_engine)
    {
        delete m_engine;
        m_engine = nullptr;
    }
}

void Engine::Login()
{
    if (m_mainWindow != nullptr)
    {
        delete m_mainWindow;
    }

    LoginPanel* loginPanel = new LoginPanel;
    if (loginPanel->exec() == QDialog::Accepted)
    {
        switch(loginPanel->getMainWindowType())
        {
            case 0:
                m_mainWindow = new PatientClient;
                break;
            case 1:
                m_mainWindow = new DoctorClient;
                break;
            case 2:
                m_mainWindow = new Manager;
                break;
        }
        if (m_mainWindow)
        {
            m_mainWindow->show();
        }
        else
        {
            qDebug() << "Main window initialization failed.";
        }
    }
    delete loginPanel;
}
