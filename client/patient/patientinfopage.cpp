#include "patientinfopage.h"
#include "ui_patientinfopage.h"
#include "editpatientinfo.h"
#include "uicontroller.h"
#include <QTimer>
#include "mycalendar.h"

#include <QTextCharFormat>

PatientInfoPage::PatientInfoPage(QWidget *parent, int patientId) :
    QWidget(parent),
    ui(new Ui::PatientInfoPage),
    m_patientId(patientId)
{
    ui->setupUi(this);
    ui->PatientPhoto->setFixedSize(200, 250);
    connect(ui->SaveBtn, &QPushButton::clicked, this, &PatientInfoPage::editPatientInfo);

    MyCalendar* calendar = new MyCalendar(this);
    ui->rightPanel->addWidget(calendar);

    QVariantMap patientInfo = UiController::get().getPatientInfo(m_patientId);
    ui->nameLabel->setText(patientInfo.value("full_name").toString());
    ui->idLabel->setText(patientInfo.value("id_card_no").toString());
    ui->genderLabel->setText(patientInfo.value("sex").toString());
    ui->birthdayLabel->setText(patientInfo.value("birth_date").toString());
    ui->ageLabel->setText(patientInfo.value("age").toString());
    ui->contactLabel->setText(patientInfo.value("phone_no").toString());
    ui->emailLabel->setText(patientInfo.value("username").toString());
    ui->emerNameLabel->setText(patientInfo.value("sos_name").toString());
    ui->emerContactLabel->setText(patientInfo.value("sos_phone_no").toString());
    // ui->historyLabel->setText(patientInfo.value("history").toString()); 既往病史， 在数据库中没有对应条目

    // 创建定时器
    QTimer *timer = new QTimer(this);
    timer->start(1000);  // 1000 毫秒更新一次
    // 连接信号和槽，每秒触发一次
    connect(timer, &QTimer::timeout, this, &PatientInfoPage::updateTimeAndGreeting);
}

PatientInfoPage::~PatientInfoPage()
{
    delete ui;
}

void PatientInfoPage::editPatientInfo()
{
    EditPatientInfo* editPanel = new EditPatientInfo(this);
    if (editPanel->exec() == QDialog::Accepted)
    {
        QList<QString> InfoSet;
        editPanel->getInfoSet(InfoSet);
        ui->nameLabel->setText(InfoSet[0]);
        ui->idLabel->setText(InfoSet[1]);
        ui->genderLabel->setText(InfoSet[2]);
        ui->birthdayLabel->setText(InfoSet[3]);
        ui->ageLabel->setText(InfoSet[4]);
        ui->contactLabel->setText(InfoSet[5]);
        ui->emailLabel->setText(InfoSet[6]);
        ui->emerNameLabel->setText(InfoSet[7]);
        ui->emerContactLabel->setText(InfoSet[8]);
        // ui->historyLabel->setText(InfoSet[9]);
        ui->PatientPhoto->setPixmap(editPanel->getImage());


        QVariantMap patientInfo;
        patientInfo["full_name"] = InfoSet[0];
        patientInfo["id_card_no"] = InfoSet[1];
        patientInfo["sex"] = InfoSet[2];
        patientInfo["birth_date"] = InfoSet[3];
        patientInfo["age"] = InfoSet[4];
        patientInfo["phone_no"] = InfoSet[5];
        patientInfo["username"] = InfoSet[6];
        patientInfo["sos_name"] = InfoSet[7];
        patientInfo["sos_phone_no"] = InfoSet[8];
        // patientInfo["history"] = InfoSet[9]; 既往病史， 在数据库中没有对应条目
        UiController::get().updatePatientInfo(m_patientId, patientInfo);
    }
    delete editPanel;
}

void PatientInfoPage::updateTimeAndGreeting()
{
    const QTime now = QTime::currentTime();
    ui->timeDisplay->setText(now.toString("HH:mm"));
    ui->title->setText(greetingForHour(now.hour()));
}

QString PatientInfoPage::greetingForHour(int h) const
{
    if (h >= 5  && h <= 8)  return QStringLiteral("早上好");
    if (h > 8  && h <= 12) return QStringLiteral("上午好");
    if (h > 12 && h <= 14) return QStringLiteral("中午好");
    if (h > 14 && h <= 18) return QStringLiteral("下午好");
    if (h > 18 && h <= 22) return QStringLiteral("晚上好");
    return QStringLiteral("夜深了，注意休息");
}
