#ifndef MINESLEEPER_H
#define MINESLEEPER_H

#include <QWidget>
#include "minetable.h"

#include <QVBoxLayout>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class mineSleeper; }
QT_END_NAMESPACE

class mineSleeper : public QWidget
{
    Q_OBJECT

public:
    mineSleeper(QWidget *parent = nullptr);
    ~mineSleeper();

    mineTable *table;

private:
    Ui::mineSleeper *ui;

    QVBoxLayout *mainLayout;
    QHBoxLayout *menuLayout;

    QLineEdit *xEdit;
    QLineEdit *yEdit;
    QLineEdit *mineEdit;

    bool tableExistF = false;
    int checkLineEdits();

private slots:
    void startSlot();
};
#endif // MINESLEEPER_H
