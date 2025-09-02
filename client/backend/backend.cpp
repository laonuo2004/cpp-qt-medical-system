#include "backend.h"
#include "ui_backend.h"
#include "engine.h"
#include "adduser.h"
#include "adddrug.h"
#include "adddepartment.h"

#include <QMessageBox>
#include <QTableWidgetItem>
#include<QDebug>

Backend::Backend(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Backend)
{
    ui->setupUi(this);
    // 点击“退出登录”按钮，关闭主窗口，打开登录界面
    connect(ui->LogoutBtn, &QPushButton::clicked, &Engine::get(), &Engine::startEngine);
    connect(ui->addPatientAction, &QAction::triggered, [this](){ addUser(0); });
    connect(ui->addDoctorAction, &QAction::triggered, [this](){ addUser(1); });
    connect(ui->addAdminAction, &QAction::triggered, [this](){ addUser(2); });
    connect(ui->addDrug, &QAction::triggered, [this](){ openDialog(new AddDrug, "添加药品", "药品添加成功！"); });
    connect(ui->addDepartment, &QAction::triggered, [this](){ openDialog(new AddDepartment, "添加部门", "部门添加成功！"); });
}

Backend::~Backend()
{
    delete ui;
}

void Backend::addUser(int userRole)
{
    AddUser* newAddUserPanel = new AddUser(this);
    newAddUserPanel->buildByRole(userRole);
    if (newAddUserPanel->exec() == QDialog::Accepted)
    {
        QMessageBox::information(this, tr("添加用户"), tr("新用户添加成功！"));
    }
    delete newAddUserPanel;
}

void Backend::openDialog(QDialog* dialog, const QString& msgTitle, const QString& msgContent)
{
    if (dialog->exec() == QDialog::Accepted)
    {
        QMessageBox::information(this, msgTitle, msgContent);
    }
    delete dialog;
}

void Backend::on_pushButton_clicked()
{
    QString uiRoleString = ui->comboBox->currentText(); // 从界面或某个输入获取的字符串

    if (uiRoleString == "所有用户") {
        DatabaseManager::ResultSet Data=UiController::get().getAllInfo();
        displayPatientsInListWidget(Data);
    } else if (uiRoleString == "医生") {
        DatabaseManager::ResultSet Data=UiController::get().getAllDoctorInfo();
        displayPatientsInListWidget(Data);
    } else if (uiRoleString == "病患") {
        DatabaseManager::ResultSet Data=UiController::get().getAllPatientInfo();
        displayPatientsInListWidget(Data);
    }else if (uiRoleString == "药品") {
        DatabaseManager::ResultSet Data=UiController::get().getAllDrugInfo();
        displayDrug(Data);
    }


}
void Backend::displayPatientsInListWidget(const DatabaseManager::ResultSet& patientsData)
{
    ui->tableWidget->clearContents(); // 清空所有单元格内容
    ui->tableWidget->setRowCount(0);   // 重置行数，清空所有行

    // 定义表头
    QStringList headerLabels;
    headerLabels << "ID" << "用户名" << "邮箱" << "角色" ;

    // 设置列数
    ui->tableWidget->setColumnCount(headerLabels.size());
    // 设置水平表头
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);

    // 如果没有数据，显示提示
    if (patientsData.empty()) {
        ui->tableWidget->setRowCount(1); // 添加一行用于提示信息
        QTableWidgetItem *noData = new QTableWidgetItem("没有找到任何患者记录。");
        // 设置文本居中，提高显示效果（可选）
        noData->setTextAlignment(Qt::AlignCenter);
        // 将 item 设置到 (0, 0) 位置
        ui->tableWidget->setItem(0, 0, noData);
        ui->tableWidget->setSpan(0, 0, 1, headerLabels.size());
        return;
    }

    // 设置行数
    ui->tableWidget->setRowCount(patientsData.size());

    // 遍历 ResultSet 并填充 QTableWidget
    int rowIdx = 0;
    for (const DatabaseManager::DataRow& row : patientsData) {
        auto getColumnValue = [&](const QString& key) {
                    auto it = row.find(key);
                    if (it != row.end()) {
                        return it->second.toString();
                    }
                    return QString(); // 键不存在时返回空字符串
                };
        // ID
        QTableWidgetItem *idItem = new QTableWidgetItem(getColumnValue("user_id"));
        ui->tableWidget->setItem(rowIdx, 0, idItem);

        // 用户名
        QTableWidgetItem *nameItem = new QTableWidgetItem(getColumnValue("user_name"));
        ui->tableWidget->setItem(rowIdx, 1, nameItem);

        // 邮箱
        QTableWidgetItem *emailItem = new QTableWidgetItem(getColumnValue("email"));
        ui->tableWidget->setItem(rowIdx, 2, emailItem);

        // 角色
        QTableWidgetItem *roleItem = new QTableWidgetItem(getColumnValue("role"));
        ui->tableWidget->setItem(rowIdx, 3, roleItem);

        rowIdx++; // 移动到下一行
    }

    // 可选：调整列宽以适应内容
    //ui->tableWidget->resizeColumnsToContents();

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    qDebug() << "Patient list updated in QTableWidget. Total rows:" << ui->tableWidget->rowCount();
}

void Backend::displayDrug(const DatabaseManager::ResultSet& patientsData)
{
    ui->tableWidget->clearContents(); // 清空所有单元格内容
    ui->tableWidget->setRowCount(0);   // 重置行数，清空所有行

    // 定义表头
    QStringList headerLabels;
    headerLabels << "药品ID" << "药品名" << "药品说明" << "使用剂量"<<"注意事项"<<"价格"<<"图片url"<<"单位" ;
    // 设置列数
    ui->tableWidget->setColumnCount(headerLabels.size());
    // 设置水平表头
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);

    // 如果没有数据，显示提示
    if (patientsData.empty()) {
        ui->tableWidget->setRowCount(1); // 添加一行用于提示信息
        QTableWidgetItem *noData = new QTableWidgetItem("没有找到任何Drug记录。");
        // 设置文本居中，提高显示效果（可选）
        noData->setTextAlignment(Qt::AlignCenter);
        // 将 item 设置到 (0, 0) 位置
        ui->tableWidget->setItem(0, 0, noData);
        ui->tableWidget->setSpan(0, 0, 1, headerLabels.size());
        return;
    }

    // 设置行数
    ui->tableWidget->setRowCount(patientsData.size());

    // 遍历 ResultSet 并填充 QTableWidget
    int rowIdx = 0;
    for (const DatabaseManager::DataRow& row : patientsData) {
        auto getColumnValue = [&](const QString& key) {
                    auto it = row.find(key);
                    if (it != row.end()) {
                        return it->second.toString();
                    }
                    return QString(); // 键不存在时返回空字符串
                };
        // ID
        QTableWidgetItem *idItem = new QTableWidgetItem(getColumnValue("drug_id"));
        ui->tableWidget->setItem(rowIdx, 0, idItem);


        QTableWidgetItem *nameItem = new QTableWidgetItem(getColumnValue("drug_name"));
        ui->tableWidget->setItem(rowIdx, 1, nameItem);


        QTableWidgetItem *emailItem = new QTableWidgetItem(getColumnValue("description"));
        ui->tableWidget->setItem(rowIdx, 2, emailItem);


        QTableWidgetItem *roleItem = new QTableWidgetItem(getColumnValue("usage"));
        ui->tableWidget->setItem(rowIdx, 3, roleItem);

        QTableWidgetItem *preItem = new QTableWidgetItem(getColumnValue("precautions"));
        ui->tableWidget->setItem(rowIdx, 4, preItem);


        QTableWidgetItem *priceItem = new QTableWidgetItem(getColumnValue("drug_price"));
        ui->tableWidget->setItem(rowIdx, 5, priceItem);


        QTableWidgetItem *imageItem = new QTableWidgetItem(getColumnValue("image_url"));
        ui->tableWidget->setItem(rowIdx,6, imageItem);


        QTableWidgetItem *unitItem = new QTableWidgetItem(getColumnValue("unit"));
        ui->tableWidget->setItem(rowIdx, 7, unitItem);
        rowIdx++; // 移动到下一行
    }

    // 可选：调整列宽以适应内容
    //ui->tableWidget->resizeColumnsToContents();

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    qDebug() << "Drug updated in QTableWidget. Total rows:" << ui->tableWidget->rowCount();
}
