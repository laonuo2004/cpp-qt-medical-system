#include "doctorcard.h"
#include "ui_doctorcard.h"
#include "patientinfo.h"
#include "doctorinfo.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

DoctorCard::DoctorCard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DoctorCard)
{
    ui->setupUi(this);
    ui->Photo->setFixedSize(120, 150);
    connect(ui->DetailBtn, &QPushButton::clicked, this, &DoctorCard::openDetailPanel);
}

DoctorCard::~DoctorCard()
{
    delete ui;
}

void DoctorCard::openDetailPanel()
{
    DoctorInfo* detailPanel = new DoctorInfo(this);
    detailPanel->buildUpDoctorInfo(m_doctorInfo);
    detailPanel->exec();
    delete detailPanel;
}

void DoctorCard::buildUpDoctorCard(QVariantMap& doctorInfo)
{
    m_doctorInfo = doctorInfo;

    ui->nameInput->setText(doctorInfo.value("full_name").toString());
    ui->jobInput->setText(doctorInfo.value("title").toString());
    ui->idInput->setText(doctorInfo.value("doctor_id").toString());

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
                    pixmap = pixmap.scaled(ui->Photo->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    ui->Photo->setPixmap(pixmap);
                    ui->Photo->setAlignment(Qt::AlignCenter);
                } else {
                    // 加载失败时显示默认图片或清除显示
                    ui->Photo->clear();
                }
                reply->deleteLater();
                manager->deleteLater();
            });
        }
    }
}

