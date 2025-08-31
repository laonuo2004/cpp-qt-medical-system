#include "patientclient.h"
#include "ui_patientclient.h"
#include "patientinfopage.h"
#include "registerpage.h"
#include "reportpage.h"
#include "drugsearchpage.h"
#include "engine.h"

PatientClient::PatientClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PatientClient)
{
    ui->setupUi(this);
    // Build up stack widget.
    m_patientInfoPage = new PatientInfoPage(this);
    m_registerPage = new RegisterPage(this);
    m_reportPage = new ReportPage(this);
    m_drugSearchPage = new DrugSearchPage(this);
    ui->stackedWidget->addWidget(m_patientInfoPage);
    ui->stackedWidget->addWidget(m_registerPage);
    ui->stackedWidget->addWidget(m_reportPage);
    ui->stackedWidget->addWidget(m_drugSearchPage);

    // Setup side bar button size.
    ui->InfoBtn->setFixedSize(150, 50);
    ui->RegisterBtn->setFixedSize(150, 50);
    ui->ReportBtn->setFixedSize(150, 50);
    ui->DrugSearchBtn->setFixedSize(150, 50);
    ui->LogoutBtn->setFixedSize(150, 50);

    ui->InfoBtn->setProperty("class", "sidebar-btn");
    ui->RegisterBtn->setProperty("class", "sidebar-btn");
    ui->ReportBtn->setProperty("class", "sidebar-btn");
    ui->DrugSearchBtn->setProperty("class", "sidebar-btn");
    ui->LogoutBtn->setProperty("class", "special-btn");

    connect(ui->InfoBtn, &QPushButton::clicked, [this](){ui->stackedWidget->setCurrentIndex(0);});
    connect(ui->RegisterBtn, &QPushButton::clicked, [this](){ui->stackedWidget->setCurrentIndex(1);});
    connect(ui->ReportBtn, &QPushButton::clicked, [this](){ui->stackedWidget->setCurrentIndex(2);});
    connect(ui->DrugSearchBtn, &QPushButton::clicked, [this](){ui->stackedWidget->setCurrentIndex(3);});
    connect(ui->LogoutBtn, &QPushButton::clicked, &Engine::get(), &Engine::Login);
}

PatientClient::~PatientClient()
{
    delete ui;
}
