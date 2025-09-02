#ifndef CHATBOX_H
#define CHATBOX_H

#include <QWidget>

namespace Ui {
class ChatBox;
}

/**
 * @class ChatBox
 * @brief 聊天界面控件
 *
 * ChatBox 基于 QWidget，内部集成了消息气泡显示、输入框和发送按钮。
 * 通过 newMsg() 可以添加新消息，区分自己（右气泡）和对方（左气泡）。
 * 同时支持设置左右头像，以及加载历史消息。
 *
 * 使用说明： 该控件应该内嵌到其它窗口或对话框中，只负责以聊天形式显示对话，
 * 不处理具体的网络逻辑。
 */
class ChatBox : public QWidget
{
    Q_OBJECT

public:
    explicit ChatBox(QWidget *parent = nullptr);
    ~ChatBox();

    /**
     * @brief 添加一条新的聊天消息
     *
     * 调用该函数，会根据传入的字符串生成一个气泡，显示在聊天记录中。
     *
     * @param msg 消息内容
     * @param isSelf 是否为自己发送的消息（true：右侧气泡，false：左侧气泡）
     */
    void newMsg(const QString& msg, bool isSelf);

    /**
     * @brief 加载历史消息
     *
     * 在此函数中从数据库或文件中读取历史消息，并调用 newMsg 添加到界面。
     */
    void loadHistoryMsg();

    /**
     * @brief 发送消息
     *
     * 从输入框中读取消息，生成气泡，添加到聊天列表，并清空输入框。
     * 同时会自动将滚动条移动到底部。
     */
    void sendMsg();

    /**
     * @brief 设置左侧头像
     * @param LeftAvatarUrl 左侧头像图片路径或 URL
     */
    void setLeftAvatar(const QString& LeftAvatarUrl);

    /**
     * @brief 设置右侧头像
     * @param RightAvatarUrl 右侧头像图片路径或 URL
     */
    void setRightAvatar(const QString& RightAvatarUrl);

private:
    Ui::ChatBox *ui;
    QString m_leftAvatarUrl = ""; ///< 左侧头像路径或 URL
    QString m_rightAvatarUrl = "";///< 右侧头像路径或 URL

    class QScrollBar* m_scrollBar;
};

#endif // CHATBOX_H
