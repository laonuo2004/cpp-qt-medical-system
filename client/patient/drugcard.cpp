#include "drugcard.h"
#include "ui_drugcard.h"
#include "druginfo.h"

#include <QVariantMap>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

DrugCard::DrugCard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrugCard)
{
    ui->setupUi(this);
    ui->Photo->setFixedSize(200, 200);
    connect(ui->DetailBtn, &QPushButton::clicked, this, &DrugCard::showDrugInfo);
}

DrugCard::~DrugCard()
{
    delete ui;
}

void DrugCard::showDrugInfo()
{
    DrugInfo* drugInfo = new DrugInfo(this);
    drugInfo->buildUpDrugInfo(m_drugInfo);
    drugInfo->exec();
    delete drugInfo;
}

void DrugCard::buildUpDrugCard(QVariantMap& drugInfo)
{
    m_drugInfo = drugInfo;

    ui->DetailBtn->setText(drugInfo.value("drug_name").toString());

    // 从网络链接获取并显示图片
    if (drugInfo.contains("image_url"))
    {
        QString drugUrl = drugInfo.find("image_url")->toString();
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
