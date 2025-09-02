#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ChatDialog(QWidget *parent , int patientid , int doctorid);
    ~ChatDialog();

private:
    Ui::ChatDialog *ui;
    int m_patientid;
    int m_doctorid;
};

#endif // CHATDIALOG_H
