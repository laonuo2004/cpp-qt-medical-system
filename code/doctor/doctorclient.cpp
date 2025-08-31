#include "doctorclient.h"
#include "ui_doctorclient.h"
#include "doctorinfopage.h"
#include "patientcardspage.h"
#include "attendancepage.h"
#include "engine.h"

DoctorClient::DoctorClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DoctorClient)
{
    ui->setupUi(this);
    // 创建子页面，添加到stack控件
    m_doctorInfoPage = new DoctorInfoPage(this);
    m_patientCardsPage = new PatientCardsPage(this);
    m_AttendancePage = new AttendancePage(this);
    ui->stackedWidget->addWidget(m_doctorInfoPage);
    ui->stackedWidget->addWidget(m_patientCardsPage);
    ui->stackedWidget->addWidget(m_AttendancePage);

    // 设置侧边导航栏按钮大小
    ui->InfoBtn->setFixedSize(150, 50);
    ui->PatientBtn->setFixedSize(150, 50);
    ui->AttendanceBtn->setFixedSize(150, 50);
    ui->LogoutBtn->setFixedSize(150, 50);

    ui->InfoBtn->setProperty("class", "sidebar-btn");
    ui->PatientBtn->setProperty("class", "sidebar-btn");
    ui->AttendanceBtn->setProperty("class", "sidebar-btn");
    ui->LogoutBtn->setProperty("class", "special-btn");

    connect(ui->InfoBtn, &QPushButton::clicked, [this](){ui->stackedWidget->setCurrentIndex(0);});
    connect(ui->PatientBtn, &QPushButton::clicked, [this](){ui->stackedWidget->setCurrentIndex(1);});
    connect(ui->AttendanceBtn, &QPushButton::clicked, [this](){ui->stackedWidget->setCurrentIndex(2);});
    connect(ui->LogoutBtn, &QPushButton::clicked, &Engine::get(), &Engine::Login);
}

DoctorClient::~DoctorClient()
{
    delete ui;
}
