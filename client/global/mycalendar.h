// mycalendar.h

#ifndef MYCALENDAR_H
#define MYCALENDAR_H

#include <QWidget>
#include <QDate>
#include <QVector>
#include <QVBoxLayout>

class QLabel;
class QPushButton;
class QGridLayout;

class MyCalendar : public QWidget
{
    Q_OBJECT

public:
    explicit MyCalendar(QWidget *parent = nullptr);
    ~MyCalendar();

    void setLanguage(const QString& lang);
    QDate selectedDate() const;
    void setSelectedDate(const QDate& date);

signals:
    void clicked(const QDate& date);

private slots:
    void showPreviousMonth();
    void showNextMonth();
    void onDateClicked();

private:
    void initWidgets();
    void arrangeWidgets();
    void updateCalendar();
    void retranslateUi();

    // 事件过滤器
    bool eventFilter(QObject *watched, QEvent *event) override;

    // 内部私有成员
    QDate m_currentDate;
    QDate m_selectedDate;

    // 布局
    QVBoxLayout *m_mainLayout; // 使用垂直布局，更简洁
    QGridLayout *m_calendarLayout;

    // 导航栏
    QWidget* m_navWidget;
    QPushButton *m_prevMonthBtn;
    QPushButton *m_nextMonthBtn;
    QLabel *m_yearMonthLabel;

    // 日历主体
    QWidget* m_calendarBodyWidget;
    QWidget* m_cornerWidget; // <-- FIX 2: 新增左上角占位控件
    QVector<QLabel*> m_weekLabels;
    QVector<QLabel*> m_dayLabels;
    QVector<QLabel*> m_weekNumLabels;

    QString m_language;
};

#endif // MYCALENDAR_H
