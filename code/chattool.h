#ifndef CHATTOOL_H
#define CHATTOOL_H

#include <QWidget>

namespace Ui {
class chattool;
}

class chattool : public QWidget
{
    Q_OBJECT

signals:
    void doc() ;
    void patient();

public:
    explicit chattool(QWidget *parent = nullptr);
    ~chattool();

private:
    Ui::chattool *ui;
};

#endif // CHATTOOL_H
