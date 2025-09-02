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

    // 扫描可用串口
    refreshSerialPorts();
    
    // 如果没有找到串口，显示提示信息
    if(ui->comboBox_port->count() == 0) {
        ui->textBrowser->append("警告: 未检测到串口设备，请连接设备后重新启动应用程序");
    } else {
        ui->textBrowser->append(QString("检测到 %1 个可用串口").arg(ui->comboBox_port->count()));
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
    if(ui->Button_openserial->text() == "打开串口")
    {
        // 检查是否有可用串口
        if(ui->comboBox_port->count() == 0) {
            ui->textBrowser->append("错误: 没有找到可用的串口设备");
            return;
        }
        
        QString portName = ui->comboBox_port->currentText();
        if(portName.isEmpty()) {
            ui->textBrowser->append("错误: 请选择一个有效的串口");
            return;
        }
        
        ui->textBrowser->append(QString("尝试打开串口: %1").arg(portName));
        
        // 检查串口详细信息
        QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
        QSerialPortInfo selectedPortInfo;
        bool portFound = false;
        
        for(const QSerialPortInfo &portInfo : availablePorts) {
            if(portInfo.portName() == portName) {
                selectedPortInfo = portInfo;
                portFound = true;
                break;
            }
        }
        
        if(portFound) {
            if(selectedPortInfo.isBusy()) {
                ui->textBrowser->append(QString("警告: 串口 %1 当前被其他程序占用").arg(portName));
            }
            ui->textBrowser->append(QString("串口描述: %1").arg(selectedPortInfo.description()));
            ui->textBrowser->append(QString("制造商: %1").arg(selectedPortInfo.manufacturer()));
            
            // 测试串口访问性
            ui->textBrowser->append("正在测试串口访问性...");
            if(!testSerialPortAccess(portName)) {
                ui->textBrowser->append("串口访问测试失败，可能被其他程序占用");
                return;  // 提前返回，避免尝试打开已知不可访问的串口
            } else {
                ui->textBrowser->append("串口访问测试通过");
            }
        } else {
            ui->textBrowser->append(QString("警告: 串口 %1 在可用串口列表中未找到").arg(portName));
        }
        
        serial->setPortName(portName);//ttySx
        switch (ui->comboBox_baud->currentIndex())
        {
            case 0: serial->setBaudRate(QSerialPort::Baud9600);   break;
            case 1: serial->setBaudRate(QSerialPort::Baud115200); break;
            default: break;
        }
        switch (ui->comboBox_databit->currentIndex())
        {
            case 0:serial->setDataBits(QSerialPort::Data8); break;
            case 1:serial->setDataBits(QSerialPort::Data7); break;
            case 2:serial->setDataBits(QSerialPort::Data6); break;
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
            QString errorMsg = QString("串口打开失败: %1").arg(serial->errorString());
            qDebug() << errorMsg;
            ui->textBrowser->append(errorMsg);
            
            // 显示更详细的错误信息
            QString detailMsg;
            QStringList suggestions;
            
            switch(serial->error()) {
                case QSerialPort::DeviceNotFoundError:
                    detailMsg = "设备未找到，请检查串口连接";
                    suggestions << "检查设备是否正确连接到电脑";
                    suggestions << "检查设备驱动是否已正确安装";
                    break;
                case QSerialPort::PermissionError:
                    detailMsg = "权限不足或串口被占用";
                    suggestions << "关闭可能占用串口的程序 (Arduino IDE、串口调试工具、Hyper Terminal等)";
                    suggestions << "在设备管理器中禁用并重新启用该串口";
                    suggestions << "检查串口在设备管理器中是否显示为正常状态";
                    suggestions << "尝试断开设备重新连接";
                    suggestions << "如果是USB转串口设备，尝试换一个USB串口";
                    suggestions << "重启应用程序";
                    break;
                case QSerialPort::OpenError:
                    detailMsg = "串口已被其他程序占用";
                    suggestions << "关闭可能使用该串口的其他程序";
                    suggestions << "检查是否有串口调试工具在运行";
                    break;
                case QSerialPort::ResourceError:
                    detailMsg = "资源错误，设备可能已断开";
                    suggestions << "重新连接设备";
                    suggestions << "检查USB线缆是否正常";
                    break;
                default:
                    detailMsg = QString("未知错误 (错误码: %1)").arg(serial->error());
                    suggestions << "尝试重新连接设备";
                    break;
            }
            
            ui->textBrowser->append(QString("错误详情: %1").arg(detailMsg));
            ui->textBrowser->append("建议解决方案:");
            for(const QString &suggestion : suggestions) {
                ui->textBrowser->append(QString("  • %1").arg(suggestion));
            }
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
        ui->Button_openserial->setText(tr("打开串口"));
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

void APIGet::refreshSerialPorts()
{
    QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
    ui->comboBox_port->clear();
    
    ui->textBrowser->append("=== 串口扫描结果 ===");
    
    for(int i=0; i<list.size(); i++)
    {
        QSerialPortInfo portInfo = list.at(i);
        QString portName = portInfo.portName();
        ui->comboBox_port->addItem(portName);
        
        // 显示详细的串口信息
        QString info = QString("串口: %1").arg(portName);
        if(!portInfo.description().isEmpty()) {
            info += QString(" - %1").arg(portInfo.description());
        }
        if(!portInfo.manufacturer().isEmpty()) {
            info += QString(" (制造商: %1)").arg(portInfo.manufacturer());
        }
        if(portInfo.isBusy()) {
            info += " [占用中]";
        }
        ui->textBrowser->append(info);
    }
    
    if(list.size() == 0) {
        ui->textBrowser->append("未找到任何串口设备");
    }
    
    ui->textBrowser->append("==================");
}

bool APIGet::testSerialPortAccess(const QString &portName)
{
    // 创建一个临时的串口对象进行测试
    QSerialPort testSerial;
    testSerial.setPortName(portName);
    testSerial.setBaudRate(QSerialPort::Baud9600);  // 使用一个标准波特率进行测试
    testSerial.setDataBits(QSerialPort::Data8);
    testSerial.setParity(QSerialPort::NoParity);
    testSerial.setStopBits(QSerialPort::OneStop);
    testSerial.setFlowControl(QSerialPort::NoFlowControl);
    
    // 尝试快速打开和关闭串口
    bool success = testSerial.open(QIODevice::ReadWrite);
    if(success) {
        testSerial.close();
        return true;
    } else {
        QString errorDetail;
        switch(testSerial.error()) {
            case QSerialPort::DeviceNotFoundError:
                errorDetail = "设备未找到";
                break;
            case QSerialPort::PermissionError:
                errorDetail = "权限不足或被占用";
                break;
            case QSerialPort::OpenError:
                errorDetail = "串口被占用";
                break;
            case QSerialPort::ResourceError:
                errorDetail = "资源错误";
                break;
            default:
                errorDetail = QString("未知错误(%1)").arg(testSerial.error());
                break;
        }
        ui->textBrowser->append(QString("测试失败原因: %1").arg(errorDetail));
        return false;
    }
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
