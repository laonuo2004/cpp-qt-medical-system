#ifndef APIGET_H
#define APIGET_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include "linechartwidget.h"  // 包含折线图组件的头文件

namespace Ui {
class APIGet;
}

class APIGet : public QWidget
{
    Q_OBJECT

public:
    explicit APIGet(QWidget *parent = nullptr);
    ~APIGet();

public slots:
    void on_Button_openserial_clicked();

        void ReadData();

        void on_Button_send_clicked();

private:
    Ui::APIGet *ui;
    QSerialPort  *serial;
    LineChartWidget *lineChartWidget;  // 折线图组件

    QVector<double> receivedData;
    int dataCount;

    // 数据处理方法
    void processReceivedData(const QByteArray &data);
    void updateChart();
};

#endif // APIGET_H
