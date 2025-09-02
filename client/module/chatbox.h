#ifndef CHATBOX_H
#define CHATBOX_H

#include <QWidget>
#include <QTimer>
#include <QScrollBar>
#include <QSet>
#include <QHash>
#include <QVariant>
#include <QDateTime>

class UiController;          // 前置声明，避免循环包含
namespace Ui { class ChatBox; }

class ChatBox : public QWidget
{
    Q_OBJECT
public:
    explicit ChatBox(QWidget *parent = nullptr);
    ChatBox(QWidget *parent, int patientid, int doctorid);
    ~ChatBox();

    void setLeftAvatar(const QString& LeftAvatarUrl);
    void setRightAvatar(const QString& RightAvatarUrl);

private slots:
    void sendMsg();
    void loadHistoryMsg();
    void pullNewMessages();

private:
    // 统一入口（本地文本 -> 右/左气泡）
    void newMsg(const QString& msg, bool isSelf);
    // 统一入口（DB行 -> 左气泡，带去重）
    void newMsg(const QVariantMap& row);

    QDateTime parseTimestamp(const QVariant& v) const;

private:
    Ui::ChatBox *ui = nullptr;
    QScrollBar* m_scrollBar = nullptr;

    // 会话双方（约定：m_patientid = 本端用户ID；m_doctorid = 对端用户ID）
    int m_patientid = -1;
    int m_doctorid  = -1;

    QString m_leftAvatarUrl;    // 左侧气泡头像（对方）
    QString m_rightAvatarUrl;   // 右侧气泡头像（自己）

    UiController* m_controller = nullptr;
    QTimer* m_refreshTimer = nullptr;

    // 去重：已上屏的 message_id
    QSet<qint64> m_seenMsgIds;
    // 去重：本地刚发送尚未从DB拉回的消息文本 -> 计数
    QHash<QString, int> m_pendingSelfMsgCounts;
    // 可选：记录目前看到的最大 message_id
    qint64 m_lastMessageId = -1;
};

#endif // CHATBOX_H
