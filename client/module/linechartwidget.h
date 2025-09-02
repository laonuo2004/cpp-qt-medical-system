#ifndef LINECHARTWIDGET_H
#define LINECHARTWIDGET_H

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QDoubleSpinBox>

QT_CHARTS_USE_NAMESPACE

struct ChartTheme {
    QColor backgroundColor;       // 图表背景
    QColor plotAreaColor;         // 绘图区背景
    QColor borderColor;           // 边框
    QColor titleColor;            // 标题
    QColor legendLabelColor;      // 图例文字
    QColor axisLineColor;         // 坐标轴线
    QColor axisLabelColor;        // 坐标轴标签
    QColor axisTitleColor;        // 坐标轴标题
    QColor gridLineColor;         // 主网格线
    QColor minorGridLineColor;    // 次网格线
    QList<QColor> seriesColors;   // 各个系列的颜色
};



class LineChartWidget : public QChartView
{
    Q_OBJECT

public:
    explicit LineChartWidget(QWidget *parent = nullptr);
    ~LineChartWidget();

    // 添加数据点的方法
    void addSeriesData(int seriesIndex, const QList<QPointF> &points);
    void addSeriesData(int seriesIndex, qreal x, qreal y);

    // 系列管理方法
    QLineSeries* getSeries(int index) const;
    int seriesCount() const;
    void addNewSeries(const QString &name = QString(), const QColor &color = QColor());

    // 图表配置方法
    void setChartTitle(const QString &title);
    void setChartTheme(QChart::ChartTheme theme);
    void setAnimationOptions(QChart::AnimationOptions options);

    void applyTheme(bool isDarkTheme);

private:
    void initChart();
    void initSeries();

    QChart *m_chart;
    QList<QLineSeries *> m_series;
};

#endif // LINECHARTWIDGET_H
