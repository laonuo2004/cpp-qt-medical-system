#include "doctorclient.h"
#include "ui_doctorclient.h"
#include "doctorinfopage.h"
#include "patientcardspage.h"
#include "attendancepage.h"
#include "engine.h"

#include <QButtonGroup>

DoctorClient::DoctorClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DoctorClient)
{
    ui->setupUi(this);

    QVector<QWidget*> pages =
    {
        new DoctorInfoPage(this),
        new PatientCardsPage(this),
        new AttendancePage(this)
    };

    QVector<QPushButton*> navButtons =
    {
        ui->InfoBtn,
        ui->PatientBtn,
        ui->AttendanceBtn
    };

    for (int i = 0; i < pages.size(); ++i)
    {
        ui->stackedWidget->addWidget(pages[i]);
        navButtons[i]->setFixedSize(150, 50);
        navButtons[i]->setProperty("class", "sidebar-btn");
        navButtons[i]->setCheckable(true);
    }

    // 退出按钮单独设置
    ui->LogoutBtn->setFixedSize(150, 50);
    ui->LogoutBtn->setProperty("class", "special-btn");

    m_navButtonGroup = new QButtonGroup(this);
    for (int i = 0; i < navButtons.size(); ++i)
    {
        m_navButtonGroup->addButton(navButtons[i], i);
    }

    connect(m_navButtonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            [this](QAbstractButton *button) {
                ui->stackedWidget->setCurrentIndex(m_navButtonGroup->id(button));
            });

    // 连接退出按钮
    connect(ui->LogoutBtn, &QPushButton::clicked, &Engine::get(), &Engine::startEngine);

    // 设置初始状态
    ui->InfoBtn->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);
}

DoctorClient::~DoctorClient()
{
    delete ui;
}
