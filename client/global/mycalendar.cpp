// mycalendar.cpp

#include "mycalendar.h"
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QStyle>

MyCalendar::MyCalendar(QWidget *parent)
    : QWidget(parent), m_currentDate(QDate::currentDate()), m_selectedDate(QDate::currentDate()), m_language("zh")
{
    setObjectName("MyCalendar");

    initWidgets();
    arrangeWidgets();
    retranslateUi();
    updateCalendar();
}

MyCalendar::~MyCalendar()
{
}

void MyCalendar::initWidgets()
{
    // ... (导航栏部分代码不变) ...
    m_navWidget = new QWidget(this);
    m_navWidget->setObjectName("navigationBar");
    m_prevMonthBtn = new QPushButton("<", m_navWidget);
    m_prevMonthBtn->setObjectName("prevMonthBtn");
    m_nextMonthBtn = new QPushButton(">", m_navWidget);
    m_nextMonthBtn->setObjectName("nextMonthBtn");
    m_yearMonthLabel = new QLabel(m_navWidget);
    m_yearMonthLabel->setObjectName("yearMonthLabel");
    m_yearMonthLabel->setAlignment(Qt::AlignCenter);

    connect(m_prevMonthBtn, &QPushButton::clicked, this, &MyCalendar::showPreviousMonth);
    connect(m_nextMonthBtn, &QPushButton::clicked, this, &MyCalendar::showNextMonth);


    // 日历主体
    m_calendarBodyWidget = new QWidget(this);
    m_calendarBodyWidget->setObjectName("calendarBody");

    // FIX 2: 初始化左上角占位控件
    m_cornerWidget = new QWidget(m_calendarBodyWidget);
    m_cornerWidget->setObjectName("cornerWidget");

    // ... (星期、周数、日期标签的初始化代码不变) ...
    for (int i = 0; i < 7; ++i) {
        QLabel *label = new QLabel(m_calendarBodyWidget);
        label->setObjectName("weekLabel");
        label->setAlignment(Qt::AlignCenter);
        m_weekLabels.append(label);
    }
    for (int i = 0; i < 6; ++i) {
        QLabel *label = new QLabel(m_calendarBodyWidget);
        label->setObjectName("weekNumLabel");
        label->setAlignment(Qt::AlignCenter);
        m_weekNumLabels.append(label);
    }
    for (int i = 0; i < 42; ++i) {
        QLabel *label = new QLabel(m_calendarBodyWidget);
        label->setObjectName("dayLabel");
        label->setAlignment(Qt::AlignCenter);
        label->setCursor(Qt::PointingHandCursor);
        label->installEventFilter(this);
        m_dayLabels.append(label);
    }
}

void MyCalendar::arrangeWidgets()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(0);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);

    // 1. 导航栏布局
    QHBoxLayout *navLayout = new QHBoxLayout(m_navWidget);
    navLayout->setContentsMargins(5, 5, 5, 5);
    navLayout->addWidget(m_prevMonthBtn);
    navLayout->addWidget(m_yearMonthLabel, 1);
    navLayout->addWidget(m_nextMonthBtn);
    m_mainLayout->addWidget(m_navWidget);

    // 2. 日历主体
    m_calendarLayout = new QGridLayout();
    m_calendarLayout->setSpacing(0);
    m_calendarLayout->setContentsMargins(0, 0, 0, 0);

    // FIX 2: 将占位控件添加到左上角 (0, 0)
    m_calendarLayout->addWidget(m_cornerWidget, 0, 0);

    // 2.1 添加星期栏
    for (int i = 0; i < 7; ++i) {
        m_calendarLayout->addWidget(m_weekLabels[i], 0, i + 1);
    }

    // 2.2 添加周数栏和日期栏
    for (int row = 0; row < 6; ++row) {
        m_calendarLayout->addWidget(m_weekNumLabels[row], row + 1, 0);
        for (int col = 0; col < 7; ++col) {
            m_calendarLayout->addWidget(m_dayLabels[row * 7 + col], row + 1, col + 1);
        }
    }

    // 设置列宽比例，让周数列窄一些
    m_calendarLayout->setColumnStretch(0, 1); // 周数列
    for(int i = 1; i < 8; ++i) {
        m_calendarLayout->setColumnStretch(i, 3); // 日期列
    }
    // 设置行高比例
     for(int i = 0; i < 7; ++i) {
        m_calendarLayout->setRowStretch(i, 1);
    }

    m_calendarBodyWidget->setLayout(m_calendarLayout);
    m_mainLayout->addWidget(m_calendarBodyWidget, 1);
}
void MyCalendar::setLanguage(const QString &lang)
{
    if (lang == "en" || lang == "zh") {
        m_language = lang;
        retranslateUi();
        updateCalendar(); // 刷新月份显示
    }
}

QDate MyCalendar::selectedDate() const
{
    return m_selectedDate;
}

void MyCalendar::setSelectedDate(const QDate &date)
{
    if (date.isValid() && m_selectedDate != date) {
        m_selectedDate = date;
        m_currentDate = QDate(date.year(), date.month(), 1);
        updateCalendar();
        emit clicked(m_selectedDate);
    }
}

void MyCalendar::retranslateUi()
{
    QStringList weekList;
    if (m_language == "zh") {
        weekList << "一" << "二" << "三" << "四" << "五" << "六" << "日";
    } else {
        weekList << "Mon" << "Tue" << "Wed" << "Thu" << "Fri" << "Sat" << "Sun";
    }

    for (int i = 0; i < 7; ++i) {
        m_weekLabels[i]->setText(weekList[i]);
    }
}


void MyCalendar::updateCalendar()
{
    // ... (更新年月标签部分不变) ...
    if (m_language == "zh") {
        m_yearMonthLabel->setText(m_currentDate.toString("yyyy年 MM月"));
    } else {
        m_yearMonthLabel->setText(m_currentDate.toString("MMMM yyyy"));
    }


    QDate firstDayOfMonth = QDate(m_currentDate.year(), m_currentDate.month(), 1);
    int firstDayOfWeek = firstDayOfMonth.dayOfWeek(); // 1=Mon, ..., 7=Sun
    int daysInMonth = m_currentDate.daysInMonth();

    // 清空所有单元格样式和文本
    for (QLabel *label : m_dayLabels) {
        label->clear();
        label->setProperty("type", "");
        label->setProperty("selected", false);
        label->setProperty("isToday", false);
    }
     for (QLabel *label : m_weekNumLabels) {
        label->clear();
    }


    // ... (填充日期和周数部分不变) ...
    for (int i = 0; i < daysInMonth; ++i) {
        int labelIndex = firstDayOfWeek - 1 + i;
        if (labelIndex < m_dayLabels.size()) {
            QDate date = firstDayOfMonth.addDays(i);
            m_dayLabels[labelIndex]->setText(QString::number(date.day()));
            m_dayLabels[labelIndex]->setProperty("date", date);
            m_dayLabels[labelIndex]->setProperty("type", "currentMonth");

            if (date == QDate::currentDate()) {
                 m_dayLabels[labelIndex]->setProperty("isToday", true);
            }
            if (date == m_selectedDate) {
                m_dayLabels[labelIndex]->setProperty("selected", true);
            }
        }
    }

    for(int row = 0; row < 6; ++row) {
        int labelIndex = row * 7 + (8 - firstDayOfWeek) % 7;
        if (labelIndex >= 42) labelIndex = 41; // 防止越界
        if (!m_dayLabels[labelIndex]->text().isEmpty()) {
            QDate dateInWeek = m_dayLabels[labelIndex]->property("date").toDate();
            m_weekNumLabels[row]->setText(QString::number(dateInWeek.weekNumber()));
        }
    }


    // FIX 1: 判断并隐藏多余的第6行
    bool week6Needed = (firstDayOfWeek - 1 + daysInMonth > 35);
    m_weekNumLabels[5]->setVisible(week6Needed);
    for (int i = 35; i < 42; ++i) {
        m_dayLabels[i]->setVisible(week6Needed);
    }


    // 强制刷新QSS样式
    style()->unpolish(this);
    style()->polish(this);
}

void MyCalendar::showPreviousMonth()
{
    m_currentDate = m_currentDate.addMonths(-1);
    updateCalendar();
}

void MyCalendar::showNextMonth()
{
    m_currentDate = m_currentDate.addMonths(1);
    updateCalendar();
}

void MyCalendar::onDateClicked()
{
    QLabel *clickedLabel = qobject_cast<QLabel*>(sender());
    if (clickedLabel && !clickedLabel->text().isEmpty()) {
        QDate date = clickedLabel->property("date").toDate();
        if (date.isValid()) {
            setSelectedDate(date);
        }
    }
}

// 事件过滤器，用于捕获日期标签的点击事件
bool MyCalendar::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
         QLabel *label = qobject_cast<QLabel*>(watched);
         if(label && m_dayLabels.contains(label)) {
             if (!label->text().isEmpty()) {
                 QDate date = label->property("date").toDate();
                 if(date.isValid()){
                     setSelectedDate(date);
                     return true; // 事件已处理
                 }
             }
         }
    }
    return QWidget::eventFilter(watched, event);
}
