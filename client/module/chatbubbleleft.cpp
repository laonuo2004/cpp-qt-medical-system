#include "chatbubbleleft.h"
#include "ui_chatbubbleleft.h"

ChatBubbleLeft::ChatBubbleLeft(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatBubbleLeft)
{
    ui->setupUi(this);
}

ChatBubbleLeft::~ChatBubbleLeft()
{
    delete ui;
}

void ChatBubbleLeft::buildUpBubble(const QString& msg, const QString& avatarUrl)
{
    ui->msg->setText(msg);
    setAvatar(avatarUrl);
}

void ChatBubbleLeft::setAvatar(const QString& url)
{
    QString avatarUrl = url;
    if (avatarUrl == "")
    {
        avatarUrl = ":/img/picture/patientAvatar.png";
    }
    QPixmap pixmap(avatarUrl);
    QPixmap scaledPixmap = pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->avatar->setPixmap(scaledPixmap);
}
