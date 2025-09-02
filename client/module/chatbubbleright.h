#ifndef CHATBUBBLERIGHT_H
#define CHATBUBBLERIGHT_H

#include <QWidget>

namespace Ui {
class ChatBubbleRight;
}

class ChatBubbleRight : public QWidget
{
    Q_OBJECT

public:
    explicit ChatBubbleRight(QWidget *parent = nullptr);
    ~ChatBubbleRight();

    void buildUpBubble(const QString& msg, const QString& avatarUrl = "");

    void setAvatar(const QString& url);

private:
    Ui::ChatBubbleRight *ui;
};

#endif // CHATBUBBLERIGHT_H
