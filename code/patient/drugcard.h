#ifndef DRUGCARD_H
#define DRUGCARD_H

#include <QWidget>

namespace Ui {
class DrugCard;
}

class DrugCard : public QWidget
{
    Q_OBJECT

public:
    explicit DrugCard(QWidget *parent = nullptr);
    ~DrugCard();

protected:
    void showDrugInfo();

private:
    Ui::DrugCard *ui;
};

#endif // DRUGCARD_H
