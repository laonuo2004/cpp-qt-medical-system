#include "linechartwidget.h"
#include <QtCharts/QValueAxis>
#include <QFont>

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

void LineChartWidget::initSeries()
{
    // 创建两个系列
    QLineSeries *series1 = new QLineSeries();
    QLineSeries *series2 = new QLineSeries();

    m_series.append(series1);
    m_series.append(series2);

    // 设置线条名称
    series1->setName(QString("line " + QString::number(1)));
    series2->setName(QString("line " + QString::number(2)));

    // 设置线条颜色
    series1->setColor(QColor(255, 0, 255));
    series2->setColor(QColor(0, 255, 255));

    // 设置是否线条可视
    series1->setVisible(true);
    series2->setVisible(true);

    // 点标签是否可视
    series1->setPointLabelsVisible(true);
    series2->setPointLabelsVisible(true);

    // 点标签颜色
    series1->setPointLabelsColor(QColor(255, 255, 255));
    series2->setPointLabelsColor(QColor(255, 255, 255));

    // 点标签字体
    series1->setPointLabelsFont(QFont("微软雅黑"));
    series2->setPointLabelsFont(QFont("微软雅黑"));

    // 设置点标签显示格式
    series1->setPointLabelsFormat("(@xPoint,@yPoint)");
    series2->setPointLabelsFormat("(@xPoint,@yPoint)");

    // 是否切割边缘点标签
    series1->setPointLabelsClipping(false);
    series2->setPointLabelsClipping(true);

    // 设置点标签是否可视
    series1->setPointsVisible(true);
    series2->setPointsVisible(true);
}

void LineChartWidget::initChart()
{
    // 设置图表主题
    m_chart->setTheme(QChart::ChartThemeBlueCerulean);

    // 设置动画选项
    m_chart->setAnimationOptions(QChart::AllAnimations);

    // 数字是否本地化
    m_chart->setLocalizeNumbers(true);

    // 添加系列到图表
    for (QLineSeries *series : m_series) {
        m_chart->addSeries(series);
    }

    // 创建默认轴
    m_chart->createDefaultAxes();

    // 设置标题
    m_chart->setTitle("Simple line chart example");
    m_chart->setTitleBrush(QBrush(QColor(255, 170, 255)));
    m_chart->setTitleFont(QFont("微软雅黑"));

    // 配置图例
    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignBottom);
    m_chart->legend()->setBackgroundVisible(true);
    m_chart->legend()->setLabelColor(QColor(255, 128, 255));
    m_chart->legend()->setBorderColor(QColor(255, 255, 170, 185));

    QFont font = m_chart->legend()->font();
    font.setItalic(!font.italic());
    font.setPointSizeF(12);
    m_chart->legend()->setFont(font);
    m_chart->legend()->setFont(QFont("微软雅黑"));

    // 设置图表
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
        series->setName(QString("line " + QString::number(m_series.count() + 1)));
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
