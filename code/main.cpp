#include "loginpanel.h"
#include "patientclient.h"
#include "doctorclient.h"
#include "manager.h"

#include <QApplication>
#include <QMainWindow>
#include <QDialog>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginPanel* loginPanel = new LoginPanel;
    if (loginPanel->exec() == QDialog::Accepted)
    {
        QMainWindow* NewWindow;
        switch(loginPanel->getMainWindowType())
        {
            case 0:
                NewWindow = new PatientClient;
                break;
            case 1:
                NewWindow = new DoctorClient;
                break;
            case 2:
                NewWindow = new Manager;
                break;
        }
        if (NewWindow)
        {
            NewWindow->show();
        }
    }
    delete loginPanel;
    return a.exec();
}
