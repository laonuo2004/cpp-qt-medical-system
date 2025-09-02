#ifndef CHATBUBBLELEFT_H
#define CHATBUBBLELEFT_H

#include <QWidget>

namespace Ui {
class ChatBubbleLeft;
}

class ChatBubbleLeft : public QWidget
{
    Q_OBJECT

public:
    explicit ChatBubbleLeft(QWidget *parent = nullptr);
    ~ChatBubbleLeft();

    void buildUpBubble(const QString& msg, const QString& avatarUrl);

    void setAvatar(const QString& url);

private:
    Ui::ChatBubbleLeft *ui;
};

#endif // CHATBUBBLELEFT_H
