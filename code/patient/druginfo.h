#ifndef DRUGINFO_H
#define DRUGINFO_H

#include <QDialog>

namespace Ui {
class DrugInfo;
}

class DrugInfo : public QDialog
{
    Q_OBJECT

public:
    explicit DrugInfo(QWidget *parent = nullptr);
    ~DrugInfo();

private:
    Ui::DrugInfo *ui;
};

#endif // DRUGINFO_H
