#include "chatbubbleright.h"
#include "ui_chatbubbleright.h"

ChatBubbleRight::ChatBubbleRight(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatBubbleRight)
{
    ui->setupUi(this);
}

ChatBubbleRight::~ChatBubbleRight()
{
    delete ui;
}

void ChatBubbleRight::buildUpBubble(const QString& msg, const QString& avatarUrl)
{
    ui->msg->setText(msg);
    setAvatar(avatarUrl);
}

void ChatBubbleRight::setAvatar(const QString& url)
{
    QString avatarUrl = url;
    if (avatarUrl == "")
    {
        avatarUrl = ":/img/picture/doctorAvatar.png";
    }
    QPixmap pixmap(avatarUrl);
    QPixmap scaledPixmap = pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->avatar->setPixmap(scaledPixmap);
}
