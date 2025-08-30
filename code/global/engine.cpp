#include "engine.h"
#include "loginpanel.h"
#include "patientclient.h"
#include "doctorclient.h"
#include "backend.h"
#include "uicontroller.h"

#include <QApplication>
#include <QMainWindow>
#include <QDialog>
#include <QDebug>


Engine::Engine(QObject *parent) : QObject(parent)
{
    m_mainWindow = nullptr;
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
                m_mainWindow = new Backend;
                break;
        }
        if (m_mainWindow)
        {
            m_mainWindow->setAttribute(Qt::WA_DeleteOnClose);
            m_mainWindow->show();
        }
        else
        {
            qDebug() << "Main window initialization failed.";
        }
    }
    delete loginPanel;
}

