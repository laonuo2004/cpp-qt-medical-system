#include "patientclient.h"
#include "ui_patientclient.h"
#include "patientinfopage.h"
#include "registerpage.h"
#include "reportpage.h"
#include "drugsearchpage.h"
#include "engine.h"
#include "apiget.h"

#include <QButtonGroup>

PatientClient::PatientClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PatientClient)
{
    ui->setupUi(this);

    m_patientId = 19;

    QVector<QWidget*> pages =
    {
        new PatientInfoPage(this, m_patientId),
        new RegisterPage(this, m_patientId),
        new ReportPage(this, m_patientId),
        new DrugSearchPage(this),
        new APIGet(this)
    };

    QVector<QPushButton*> navButtons =
    {
        ui->InfoBtn,
        ui->RegisterBtn,
        ui->ReportBtn,
        ui->DrugSearchBtn,
        ui->ChartBtn
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

PatientClient::~PatientClient()
{
    delete ui;
}
