#include "doctorattendancewidget.h"
#include "ui_doctorattendancewidget.h"

DoctorAttendanceWidget::DoctorAttendanceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DoctorAttendanceWidget)
{
    ui->setupUi(this);
}

DoctorAttendanceWidget::~DoctorAttendanceWidget()
{
    delete ui;
}
