#ifndef BASICFUNC_H
#define BASICFUNC_H

#include <QWidget>
#include <QMainWindow>
#include "doctorchattool.h"
#include "patientchattool.h"
#include "payinterface.h"
#include "chatinterface.h"
#include "apiget.h"

namespace Ui {
class BasicFunc;
}

class BasicFunc : public QWidget
{
    Q_OBJECT

public:
    explicit BasicFunc(QWidget *parent = nullptr);
    ~BasicFunc();

private slots:
    void onShowPayInterface();  // 显示PayFunc窗口的槽函数
    void onShowChatInterface() ;  // to show chatfunc
    void onShowLineChartFunc() ; //to show linechart

private:
    Ui::BasicFunc *ui;
    PayInterface *PF;
    ChatInterface *CF;
    APIGet *AF;
};

#endif // BASICFUNC_H
