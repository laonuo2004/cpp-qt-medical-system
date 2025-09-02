#include "druginfo.h"
#include "ui_druginfo.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

DrugInfo::DrugInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DrugInfo)
{
    ui->setupUi(this);
    setFixedSize(1200, 800);
    ui->Photo->setFixedSize(300, 300);
}

DrugInfo::~DrugInfo()
{
    delete ui;
}

void DrugInfo::buildUpDrugInfo(const QVariantMap& drugInfo)
{
    ui->nameInput->setText(drugInfo.value("drug_name").toString());
    ui->priceInput->setText(drugInfo.value("drug_price").toString());
    ui->usageInput->setText(drugInfo.value("usage").toString());
    ui->precautions->setText(drugInfo.value("precautions").toString());
    ui->descriptionInput->setText(drugInfo.value("description").toString());

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
