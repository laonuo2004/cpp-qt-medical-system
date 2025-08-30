#include "doctorclient.h"
#include "ui_doctorclient.h"
#include "doctorinformationwidget.h"
#include "doctorpatientprofilewidget.h"
#include "doctorattendancewidget.h"

DoctorClient::DoctorClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DoctorClient)
{
    ui->setupUi(this);
    // Build up stack widget.
    m_doctorInformationWidget = new DoctorInformationWidget(this);
    m_doctorPatientProfileWidget = new DoctorPatientProfileWidget(this);
    m_doctorAttendanceWidget = new DoctorAttendanceWidget(this);
    ui->stackedWidget->addWidget(m_doctorInformationWidget);
    ui->stackedWidget->addWidget(m_doctorPatientProfileWidget);
    ui->stackedWidget->addWidget(m_doctorAttendanceWidget);
    connect(ui->InfoBtn, &QPushButton::clicked, [this](){ui->stackedWidget->setCurrentIndex(0);});
    connect(ui->PatientBtn, &QPushButton::clicked, [this](){ui->stackedWidget->setCurrentIndex(1);});
    connect(ui->AttendanceBtn, &QPushButton::clicked, [this](){ui->stackedWidget->setCurrentIndex(2);});

    // Setup side bar button size.
    ui->InfoBtn->setFixedSize(150, 50);
    ui->PatientBtn->setFixedSize(150, 50);
    ui->AttendanceBtn->setFixedSize(150, 50);
}

DoctorClient::~DoctorClient()
{
    delete ui;
}
