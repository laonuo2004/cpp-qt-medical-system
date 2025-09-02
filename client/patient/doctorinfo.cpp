#include "doctorinfo.h"
#include "ui_doctorinfo.h"
#include "registerinfo.h"
#include "patientchattool.h"
#include "uicontroller.h"

#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

DoctorInfo::DoctorInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DoctorInfo)
{
    ui->setupUi(this);
    ui->DoctorPhoto->setFixedSize(200, 250);
    connect(ui->confirmBtn, &QPushButton::clicked, this, &DoctorInfo::patientRegister);
    connect(ui->chatBtn, &QPushButton::clicked, this, &DoctorInfo::startChat);
}

DoctorInfo::~DoctorInfo()
{
    delete ui;
}

void DoctorInfo::patientRegister()
{
    RegisterInfo* registerPanel = new RegisterInfo(this);
    if (registerPanel->exec() == QDialog::Accepted)
    {
        QMessageBox::information(this, tr("挂号"), tr("挂号成功。"));
    }
    delete registerPanel;
}

void DoctorInfo::startChat()
{
    PatientChatTool* chatDialog = new PatientChatTool(this);
    if (chatDialog->exec() == QDialog::Accepted)
    {

    }
    delete chatDialog;
}

void DoctorInfo::buildUpDoctorInfo(QVariantMap doctorInfo)
{
    // 加载基本的医生信息
    ui->nameInput->setText(doctorInfo.value("full_name").toString());
    ui->idInput->setText(doctorInfo.value("doctor_id").toString());
    ui->ageInput->setText(doctorInfo.value("age").toString());
    ui->departmentInput->setText(doctorInfo.value("department").toString());
    ui->titleInput->setText(doctorInfo.value("title").toString());
    ui->workTimeInput->setText(doctorInfo.value("doc_start").toString());
    ui->priceInput->setText(doctorInfo.value("registration_fee").toString());
    ui->maxPatientInput->setText(doctorInfo.value("patient_limit").toString());

    // 从网络链接获取并显示图片
    if (doctorInfo.contains("image_url"))
    {
        QString drugUrl = doctorInfo.find("image_url")->toString();
        if (!drugUrl.isEmpty())
        {
            // 从URL加载图片
            QNetworkAccessManager *manager = new QNetworkAccessManager(this);
            QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(drugUrl)));

            // 使用lambda处理异步响应
            QObject::connect(reply, &QNetworkReply::finished, [=]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QByteArray imageData = reply->readAll();
                    QPixmap pixmap;
                    pixmap.loadFromData(imageData);

                    // 缩放图片以适应QLabel大小，同时保持比例
                    pixmap = pixmap.scaled(ui->DoctorPhoto->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    ui->DoctorPhoto->setPixmap(pixmap);
                    ui->DoctorPhoto->setAlignment(Qt::AlignCenter);
                } else {
                    // 加载失败时显示默认图片或清除显示
                    ui->DoctorPhoto->clear();
                }
                reply->deleteLater();
                manager->deleteLater();
            });
        }
    }

    // 加载医生可用时段

}

