#include "linechartwidget.h"
#include <QtCharts/QValueAxis>
#include <QFont>

const ChartTheme lightTheme = {
    QColor("#FFFFFF"),     // backgroundColor
    QColor("#F7F9FC"),     // plotAreaColor
    QColor("#D4D8E1"),     // borderColor
    QColor("#357ABD"),     // titleColor
    QColor("#3E4A59"),     // legendLabelColor
    QColor("#D4D8E1"),     // axisLineColor
    QColor("#3E4A59"),     // axisLabelColor
    QColor("#357ABD"),     // axisTitleColor
    QColor("#E6F2FF"),     // gridLineColor
    QColor("#F0F4F7"),     // minorGridLineColor
    { QColor("#FF6B6B"), QColor("#357ABD"), QColor("#45B7D1") } // series colors
};

const ChartTheme darkTheme = {
    QColor("#121212"),     // backgroundColor
    QColor("#1E1E1E"),     // plotAreaColor
    QColor("#2D3B45"),     // borderColor
    QColor("#20C997"),     // titleColor
    QColor("#E0E0E0"),     // legendLabelColor
    QColor("#2D3B45"),     // axisLineColor
    QColor("#CCCCCC"),     // axisLabelColor
    QColor("#20C997"),     // axisTitleColor
    QColor("#333333"),     // gridLineColor
    QColor("#1E1E1E"),     // minorGridLineColor
    {
        QColor("#20C997"), // series 1
        QColor("#FF6B81"), // series 2
        QColor("#FFD93D"), // series 3
    }
};



LineChartWidget::LineChartWidget(QWidget *parent)
    : QChartView(parent)
    , m_chart(new QChart())
{    
    // 初始化系列
    initSeries();

    // 初始化图表
    initChart();

    // 设置渲染提示
    setRenderHint(QPainter::Antialiasing);
}

LineChartWidget::~LineChartWidget()
{
    // 清理资源
    qDeleteAll(m_series);
    delete m_chart;
}

void LineChartWidget::applyTheme(bool isDarkTheme)
{
    ChartTheme theme = lightTheme;
    if (isDarkTheme)
    {
        theme = darkTheme;
    }
    // 背景 & 绘图区
    m_chart->setBackgroundBrush(QBrush(theme.backgroundColor));
    m_chart->setPlotAreaBackgroundVisible(true);
    m_chart->setPlotAreaBackgroundBrush(QBrush(theme.plotAreaColor));
    m_chart->setBackgroundPen(QPen(theme.borderColor, 1));
    m_chart->setBackgroundRoundness(8);

    // 标题
    m_chart->setTitleBrush(QBrush(theme.titleColor));

    // 图例
    QLegend* legend = m_chart->legend();
    legend->setLabelColor(theme.legendLabelColor);

    // 坐标轴
    for (QAbstractAxis* axis : m_chart->axes()) {
        if (QValueAxis* vAxis = qobject_cast<QValueAxis*>(axis)) {
            vAxis->setLabelsColor(theme.axisLabelColor);
            vAxis->setTitleBrush(QBrush(theme.axisTitleColor));
            vAxis->setLinePen(QPen(theme.axisLineColor, 1));
            vAxis->setGridLineColor(theme.gridLineColor);
            vAxis->setMinorGridLineColor(theme.minorGridLineColor);
        }
    }

    // 系列颜色
    int i = 0;
    for (QLineSeries* series : m_series) {
        if (i < theme.seriesColors.size()) {
            QPen pen(theme.seriesColors[i]);
            pen.setWidth(2);
            series->setPen(pen);
        }
        i++;
    }
}



void LineChartWidget::initSeries()
{
    // 创建两个系列
    QLineSeries *series1 = new QLineSeries();
    QLineSeries *series2 = new QLineSeries();

    m_series.append(series1);
    m_series.append(series2);

    // 设置线条名称
    series1->setName(QString("数据 1"));
    series2->setName(QString("数据 2"));

    // 设置线条颜色（主题色）
    series1->setColor(QColor("#007BFF"));   // 主题蓝
    series2->setColor(QColor("#6C757D"));   // 辅助灰蓝

    // 设置是否线条可视
    series1->setVisible(true);
    series2->setVisible(true);

    // 点标签是否可视
    series1->setPointLabelsVisible(true);
    series2->setPointLabelsVisible(true);

    // 点标签颜色（统一用主文字色）
    series1->setPointLabelsColor(QColor("#3E4A59"));
    series2->setPointLabelsColor(QColor("#3E4A59"));

    // 点标签字体
    QFont labelFont("Microsoft YaHei", 10);
    series1->setPointLabelsFont(labelFont);
    series2->setPointLabelsFont(labelFont);

    // 设置点标签显示格式
    series1->setPointLabelsFormat("(@xPoint,@yPoint)");
    series2->setPointLabelsFormat("(@xPoint,@yPoint)");

    // 是否切割边缘点标签
    series1->setPointLabelsClipping(false);
    series2->setPointLabelsClipping(true);

    // 设置点是否可视
    series1->setPointsVisible(true);
    series2->setPointsVisible(true);
}

void LineChartWidget::initChart()
{
    // 标题字体
    QFont titleFont("Microsoft YaHei", 16, QFont::Bold);
    m_chart->setTitleFont(titleFont);

    // 图例
    QLegend* legend = m_chart->legend();
    legend->setVisible(true);
    legend->setAlignment(Qt::AlignBottom);
    legend->setBackgroundVisible(false);
    legend->setBorderColor(Qt::transparent);
    legend->setFont(QFont("Microsoft YaHei", 12));

    // 坐标轴字体（颜色等由 applyTheme 控制）
    for (QAbstractAxis* axis : m_chart->axes()) {
        if (QValueAxis* vAxis = qobject_cast<QValueAxis*>(axis)) {
            QFont axisFont("Microsoft YaHei", 12);
            vAxis->setLabelsFont(axisFont);
            vAxis->setTitleFont(axisFont);
            vAxis->setLabelFormat("%.1f");
        }
    }

    m_chart->setMargins(QMargins(30, 30, 30, 30));

    // 动画效果
    m_chart->setAnimationOptions(QChart::AllAnimations);

    // 本地化数字
    m_chart->setLocalizeNumbers(true);

    // 添加系列到图表
    for (QLineSeries *series : m_series) {
        m_chart->addSeries(series);
    }

    // 创建默认坐标轴
    m_chart->createDefaultAxes();

    // 设置标题
    m_chart->setTitle("健康状态分析");

    applyTheme(false);

    // 应用图表
    setChart(m_chart);
}

void LineChartWidget::addSeriesData(int seriesIndex, const QList<QPointF> &points)
{
    if (seriesIndex >= 0 && seriesIndex < m_series.size()) {
        m_series[seriesIndex]->append(points);
    }
}

void LineChartWidget::addSeriesData(int seriesIndex, qreal x, qreal y)
{
    if (seriesIndex >= 0 && seriesIndex < m_series.size()) {
        m_series[seriesIndex]->append(x, y);
    }
}

QLineSeries* LineChartWidget::getSeries(int index) const
{
    if (index >= 0 && index < m_series.size()) {
        return m_series[index];
    }
    return nullptr;
}

int LineChartWidget::seriesCount() const
{
    return m_series.size();
}

void LineChartWidget::addNewSeries(const QString &name, const QColor &color)
{
    QLineSeries *series = new QLineSeries();

    if (!name.isEmpty()) {
        series->setName(name);
    } else {
        series->setName(QString("折线 " + QString::number(m_series.count() + 1)));
    }

    if (color.isValid()) {
        series->setColor(color);
    }

    m_series.append(series);
    m_chart->addSeries(series);
    m_chart->createDefaultAxes();
}

void LineChartWidget::setChartTitle(const QString &title)
{
    m_chart->setTitle(title);
}

void LineChartWidget::setChartTheme(QChart::ChartTheme theme)
{
    m_chart->setTheme(theme);
}

void LineChartWidget::setAnimationOptions(QChart::AnimationOptions options)
{
    m_chart->setAnimationOptions(options);
}
