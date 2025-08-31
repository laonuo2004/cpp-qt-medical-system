#include "paypage.h"
#include "ui_paypage.h"
#include <QMessageBox>
#include <QDate>
#include <QDebug>

PayPage::PayPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PayPage),
    controller(new UiController(this))  // 初始化列表初始化controller
{
    ui->setupUi(this);
    ui->label_7->hide();
    ui->label->hide();
    ui->yes->hide();
    ui->no->hide();

    setupConnections();
}

PayPage::~PayPage()
{
    delete ui;
    // controller由Qt父对象系统自动管理，无需手动删除
}

void PayPage::setupConnections()
{
    connect(ui->findBtn, &QPushButton::clicked, this, &PayPage::onPaymentSearch);
    // 连接支付确认按钮
    connect(ui->yes, &QPushButton::clicked, this, &PayPage::onPaymentConfirmed);
    // 连接支付取消按钮
    connect(ui->no, &QPushButton::clicked, this, &PayPage::onPaymentCancelled);
}

void PayPage::onPaymentSearch()
{
    QString patientIdText = ui->PatientId->text().trimmed();
    if (patientIdText.isEmpty())
    {
        QMessageBox::warning(const_cast<PayPage*>(this),
            tr("输入错误"),
            tr("患者ID不能为空")
        );
        return ;
    }

    bool conversionOk = false;
    const uint patientId = patientIdText.toUInt(&conversionOk);
    if (!conversionOk || patientId <= 0)
    {
        QMessageBox::warning(const_cast<PayPage*>(this),
            tr("输入错误"),
            tr("请输入有效的患者ID（正整数）")
        );
        return ;
    }

    // 获取并验证日期
    const QDate date = getSelectedDate(conversionOk);
    if (!conversionOk)
    {
        // getSelectedDate内部已经显示了错误消息
        return ;
    }

    // 检查控制器是否有效
    if (!controller)
    {
        QMessageBox::critical(const_cast<PayPage*>(this),
            tr("系统错误"),
            tr("控制器未正确初始化")
        );
        return ;
    }
    try
    {
        // 获取预约列表
        const QVariantList appointments = controller->getPatientAppointments(patientId, date);
        if (appointments.isEmpty())
        {
            QMessageBox::information(const_cast<PayPage*>(this),
                tr("查询结果"),
                tr("未找到指定日期和患者的预约记录")
            );
            return ;
        }
        ui->label_7->show();
        ui->label->show();
        ui->yes->show();
        ui->no->show();
    }
    catch (const std::exception& e)
    {
        QMessageBox::critical(const_cast<PayPage*>(this),
            tr("系统错误"),
            tr("获取预约记录时发生异常: %1").arg(e.what())
        );
        return ;
    }
    catch (...)
    {
        QMessageBox::critical(const_cast<PayPage*>(this),
            tr("系统错误"),
            tr("获取预约记录时发生未知异常")
        );
        return ;
    }
}

void PayPage::onPaymentConfirmed()
{
    updateAppointmentStatus("completed");
}

void PayPage::onPaymentCancelled()
{
    updateAppointmentStatus("canceled");
}

void PayPage::updateAppointmentStatus(const QString& status)
{
    // 检查控制器是否有效
    if (!controller) {
        QMessageBox::critical(this, tr("系统错误"), tr("控制器未正确初始化"));
        return;
    }

    bool ok = false;
    const int appointmentId = getFirstAppointmentId(ok);

    if (ok && appointmentId > 0) {
        try {
            // 尝试更新预约状态
            bool success = controller->updateAppointmentStatus(appointmentId, status);

            if (success) {
                // 显示操作结果消息
                QMessageBox::information(this,
                    tr("支付状态"),
                    status == "completed" ?
                        tr("支付成功完成。") :
                        tr("支付已取消。")
                );
            } else {
                QMessageBox::warning(this,
                    tr("操作失败"),
                    tr("更新预约状态失败，请稍后重试。")
                );
            }
        } catch (const std::exception& e) {
            QMessageBox::critical(this,
                tr("系统错误"),
                tr("更新预约状态时发生异常: %1").arg(e.what())
            );
        } catch (...) {
            QMessageBox::critical(this,
                tr("系统错误"),
                tr("更新预约状态时发生未知异常")
            );
        }
    } else {
        QMessageBox::warning(this,
            tr("错误"),
            tr("未找到有效的预约记录。请检查输入数据。")
        );
    }


}

int PayPage::getFirstAppointmentId(bool& ok) const
{
    ok = false;

    // 获取并验证患者ID
    QString patientIdText = ui->PatientId->text().trimmed();
    if (patientIdText.isEmpty()) {
        QMessageBox::warning(const_cast<PayPage*>(this),
            tr("输入错误"),
            tr("患者ID不能为空")
        );
        return -1;
    }

    bool conversionOk = false;
    const uint patientId = patientIdText.toUInt(&conversionOk);
    if (!conversionOk || patientId <= 0) {
        QMessageBox::warning(const_cast<PayPage*>(this),
            tr("输入错误"),
            tr("请输入有效的患者ID（正整数）")
        );
        return -1;
    }

    // 获取并验证日期
    const QDate date = getSelectedDate(conversionOk);
    if (!conversionOk) {
        // getSelectedDate内部已经显示了错误消息
        return -1;
    }

    // 检查控制器是否有效
    if (!controller) {
        QMessageBox::critical(const_cast<PayPage*>(this),
            tr("系统错误"),
            tr("控制器未正确初始化")
        );
        return -1;
    }

    try {
        // 获取预约列表
        const QVariantList appointments = controller->getPatientAppointments(patientId, date);
        if (appointments.isEmpty()) {
            QMessageBox::information(const_cast<PayPage*>(this),
                tr("查询结果"),
                tr("未找到指定日期和患者的预约记录")
            );
            return -1;
        }

        // 提取第一个预约ID
        const QVariantMap firstAppointment = appointments.first().toMap();
        if (!firstAppointment.contains("appointment_id")) {
            QMessageBox::warning(const_cast<PayPage*>(this),
                tr("数据错误"),
                tr("预约记录数据不完整")
            );
            return -1;
        }

        bool idOk = false;
        int appointmentId = firstAppointment["appointment_id"].toInt(&idOk);

        if (!idOk || appointmentId <= 0) {
            QMessageBox::warning(const_cast<PayPage*>(this),
                tr("数据错误"),
                tr("预约ID无效")
            );
            return -1;
        }

        ok = true;
        return appointmentId;

    } catch (const std::exception& e) {
        QMessageBox::critical(const_cast<PayPage*>(this),
            tr("系统错误"),
            tr("获取预约记录时发生异常: %1").arg(e.what())
        );
        return -1;
    } catch (...) {
        QMessageBox::critical(const_cast<PayPage*>(this),
            tr("系统错误"),
            tr("获取预约记录时发生未知异常")
        );
        return -1;
    }
}

QDate PayPage::getSelectedDate(bool& ok) const
{
    ok = false;

    // 获取并验证年份
    QString yearText = ui->yearEdit->text().trimmed();
    if (yearText.isEmpty()) {
        QMessageBox::warning(const_cast<PayPage*>(this),
            tr("输入错误"),
            tr("年份不能为空")
        );
        return QDate();
    }

    bool conversionOk = false;
    const int year = yearText.toInt(&conversionOk);
    if (!conversionOk || year < 1900 || year > 2100) {
        QMessageBox::warning(const_cast<PayPage*>(this),
            tr("输入错误"),
            tr("请输入有效的年份（1900-2100）")
        );
        return QDate();
    }

    // 获取并验证月份
    QString monthText = ui->monthEdit->text().trimmed();
    if (monthText.isEmpty()) {
        QMessageBox::warning(const_cast<PayPage*>(this),
            tr("输入错误"),
            tr("月份不能为空")
        );
        return QDate();
    }

    const int month = monthText.toInt(&conversionOk);
    if (!conversionOk || month < 1 || month > 12) {
        QMessageBox::warning(const_cast<PayPage*>(this),
            tr("输入错误"),
            tr("请输入有效的月份（1-12）")
        );
        return QDate();
    }

    // 获取并验证日期
    QString dayText = ui->dayEdit->text().trimmed();
    if (dayText.isEmpty()) {
        QMessageBox::warning(const_cast<PayPage*>(this),
            tr("输入错误"),
            tr("日期不能为空")
        );
        return QDate();
    }

    const int day = dayText.toInt(&conversionOk);
    if (!conversionOk || day < 1 || day > 31) {
        QMessageBox::warning(const_cast<PayPage*>(this),
            tr("输入错误"),
            tr("请输入有效的日期（1-31）")
        );
        return QDate();
    }

    // 创建并验证日期
    const QDate date(year, month, day);
    if (!date.isValid()) {
        QMessageBox::warning(const_cast<PayPage*>(this),
            tr("输入错误"),
            tr("输入的日期无效")
        );
        return QDate();
    }

    ok = true;
    return date;
}
