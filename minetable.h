#ifndef MINETABLE_H
#define MINETABLE_H

#include <QWidget>
#include <QPushButton>

#include "QRightClickButton.h"

namespace Ui {
class mineTable;
}

typedef struct {
    QString color;
} STRUCT_COLOR_NO;

typedef  enum {
    MINE_NONE=0,
    MINE_ONE,
    MINE_TWO,
    MINE_THREE,
    MINE_FOUR,
    MINE_FIVE,
    MINE_SIX,
    MINE_SEVEN,
    MINE_EIGHT,

    MINE_COUNT,
} ENUM_MINECOUNT;

typedef struct STRUCT_COLORS{
    STRUCT_COLOR_NO     number[MINE_COUNT];

    void defaultInit(){
        number[MINE_NONE] = {""};
        number[MINE_ONE] = {"rgb(0,0,254)"};
        number[MINE_TWO] = {"rgb(0,128,1)"};
        number[MINE_THREE] = {"rgb(254,0,0)"};
        number[MINE_FOUR] = {"rgb(1,0,128)"};
        number[MINE_FIVE] = {"rgb(254,0,0)"};
        number[MINE_SIX] = {"rgb(128,0,0)"};
        number[MINE_SEVEN] = {"rgb(255,89,100)"};
        number[MINE_EIGHT] = {"rgb(113,113,113)"};
    };
    STRUCT_COLORS(){defaultInit();};
    QString getColorString(int No){return number[No].color;};
} STRUCT_COLORS;

class mineTable : public QWidget
{
    Q_OBJECT

public:
    explicit mineTable(int lineCount, int columnCount, double mineCount,QWidget *parent = nullptr);
    ~mineTable();

    void openSpace(QPoint loc);
    int checkBordersMine(QPoint loc);
    int checkBordersFlags(QPoint loc);

private:
    Ui::mineTable *ui;
    int mineHitCtr = 0;
    STRUCT_COLORS colors;
    bool initF = false;
    struct classInfo{
        int lineCt = 0;
        int columnCt = 0;
        int mineCt = 0;
    }classInfo;

    int fontSize = 20;

    void prepareButton(QRightClickButton *button);
    void paintButton(QPushButton *button, int mineCount);

    QList<QList<QRightClickButton*>> buttons;

private slots:
    void buttonReleasedSlot(QMouseEvent *e);

};

#endif // MINETABLE_H
