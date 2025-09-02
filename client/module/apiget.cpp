#include "apiget.h"
#include "ui_apiget.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QRegularExpression>

APIGet::APIGet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::APIGet)
{
    ui->setupUi(this);
    ui->label_light->setStyleSheet("color:rgb(255,0,0)");

    QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
    ui->comboBox_port->clear();
    for(int i=0; i<list.size(); i++)
    {
        ui->comboBox_port->addItem(list.at(i).portName());
    }

    serial = new QSerialPort;

    QObject::connect(serial,&QSerialPort::readyRead,this,&APIGet::ReadData);

    // 初始化折线图组件
    lineChartWidget = new LineChartWidget(this);
    // 将折线图添加到界面布局中
    ui->chartSlot->addWidget(lineChartWidget);
}

APIGet::~APIGet()
{
    delete ui;
}

void APIGet::on_Button_openserial_clicked()
{
    if(ui->Button_openserial->text() == "打开端口")
    {
        serial->setPortName(ui->comboBox_port->currentText());//ttySx
        switch (ui->comboBox_baud->currentIndex())
        {
            case 0: serial->setBaudRate(QSerialPort::Baud115200); break;
            case 1: serial->setBaudRate(QSerialPort::Baud9600);   break;
            default: break;
        }
        switch (ui->comboBox_databit->currentIndex())
        {
            case 0:serial->setDataBits(QSerialPort::Data8); break;
            case 1:serial->setDataBits(QSerialPort::Data6); break;
            default: break;
        }
        switch (ui->comboBox_parity->currentIndex())
        {
            case 0:serial->setParity(QSerialPort::NoParity);   break;
            case 1:serial->setParity(QSerialPort::EvenParity); break;
            case 2:serial->setParity(QSerialPort::OddParity);  break;
            default: break;
        }
        switch (ui->comboBox_stopbit->currentIndex())
        {
            case 0:serial->setStopBits(QSerialPort::OneStop); break;
            case 1:serial->setStopBits(QSerialPort::OneAndHalfStop); break;
            case 2:serial->setStopBits(QSerialPort::TwoStop); break;
            default: break;
        }
        serial->setFlowControl(QSerialPort::NoFlowControl);
        bool info = serial->open(QIODevice::ReadWrite);
        if(info == true)
        {

            ui->label_light->setText("已开启...");
            ui->label_light->setStyleSheet("color:rgb(0,255,0)");
            ui->comboBox_port->setEnabled(false);
            ui->comboBox_baud->setEnabled(false);
            ui->comboBox_databit->setEnabled(false);
            ui->comboBox_parity->setEnabled(false);
            ui->comboBox_stopbit->setEnabled(false);
            ui->Button_openserial->setText(tr("closePort"));

            // 清空之前的数据
            receivedData.clear();
            dataCount = 0;
        }
        else
        {
            qDebug()<<"fail";
        }
    }
    else
    {
        serial->clear();
        serial->close();
        ui->label_light->setText("关闭");
        ui->label_light->setStyleSheet("color:rgb(255,0,0)");
        ui->comboBox_port->setEnabled(true);
        ui->comboBox_baud->setEnabled(true);
        ui->comboBox_databit->setEnabled(true);
        ui->comboBox_parity->setEnabled(true);
        ui->comboBox_stopbit->setEnabled(true);
        ui->Button_openserial->setText(tr("打开端口"));
    }
}

void APIGet::updateTheme(bool isDarkMode)
{
    lineChartWidget->applyTheme(isDarkMode);
}

void APIGet::ReadData()
{
    QByteArray buf = serial->readAll();
    ui->textBrowser->append(buf);

    // 处理接收到的数据
    processReceivedData(buf);
}

void APIGet::processReceivedData(const QByteArray &data)
{
    // 将数据转换为字符串
    QString dataStr = QString::fromUtf8(data);

    // 处理包含多个数字的字符串
    processMultiValueString(dataStr);
}

// 新增函数：处理包含多个值的字符串
void APIGet::processMultiValueString(const QString &dataStr)
{
    // 使用正则表达式匹配所有数字（包括浮点数）
    QRegularExpression re("\\b\\d+(?:\\.\\d+)?\\b");
    QRegularExpressionMatchIterator i = re.globalMatch(dataStr);

    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        QString numberStr = match.captured(0);

        // 尝试将字符串转换为数字
        bool ok;
        double value = numberStr.toDouble(&ok);

        if (ok && value >= 0.0 && value <= 1.0)
        {
            // 将有效数据添加到数组中
            receivedData.append(value);
            dataCount++;

            qDebug() << "Extracted value:" << value;
            qDebug() << "datacount:" << dataCount;

            // 如果收集到5个数据点，更新图表
            if (receivedData.size() >= 5)
            {
                updateChart();

                // 清空数据，准备接收下一组
                receivedData.clear();
                break; // 处理完5个数据后跳出循环
            }
        }
        else
        {
            qDebug() << "Invalid data:" << numberStr;
        }
    }
}

void APIGet::updateChart()
{
    // 确保有数据点
    if (receivedData.isEmpty())
        return;

    // 创建数据点列表
    QList<QPointF> points;
    for (int i = 0; i < receivedData.size(); i++)
    {
        // x坐标为数据点索引，y坐标为接收到的值
        points.append(QPointF(i*0.2, receivedData[i]));
    }

    // 将数据添加到折线图
    // 使用第一个系列（索引0）
    lineChartWidget->addSeriesData(0, points);
}

void APIGet::on_Button_openFile_clicked()
{
    // 打开文件对话框
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Text File"), "", tr("Text Files (*.txt)"));

    if (!fileName.isEmpty())
    {
        processFileData(fileName);
    }
}

// 修改后的处理文件数据的函数
void APIGet::processFileData(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "Cannot open file: " + filePath);
        return;
    }

    // 读取整个文件内容
    QTextStream in(&file);
    QString fileContent = in.readAll();
    file.close();

    // 处理文件内容中的多个值
    processMultiValueString(fileContent);
}

// 新增公共方法：处理字符串数据
void APIGet::processStringData(const QString &data)
{
    processMultiValueString(data);
}
