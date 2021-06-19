#include "minetable.h"
#include "ui_minetable.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

#include <QDebug>
#include <QRandomGenerator>
#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>

mineTable::mineTable(int lineCount, int columnCount, double mineCount, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mineTable)
{
    ui->setupUi(this);
    QRightClickButton *d2Buttons[lineCount][columnCount];

    QVBoxLayout *lines = new QVBoxLayout();
    lines->setSpacing(2);
    this->setLayout(lines);
    for (int line=0; line<lineCount; line++) {
        QVBoxLayout *lineLayout = new QVBoxLayout();
        lineLayout->setSpacing(2);
        lines->addLayout(lineLayout);
        QHBoxLayout *columnLayout = new QHBoxLayout();
        lineLayout->addLayout(columnLayout);
        for (int column=0; column<columnCount; column++) {
            QRightClickButton *button = new QRightClickButton();
            d2Buttons[line][column] = button;
            columnLayout->addWidget(button);
            prepareButton(button);
            button->setProperty("buttonLoc", QPoint(line, column));
            button->setProperty("mine", false);
            button->setProperty("clicked", false);
            button->setProperty("flagged", false);
            button->setStyleSheet("QPushButton {background-color:rgb(255, 219, 166);}");
            button->setText(" ");
        }
    }

    for (int i=0; i<lineCount; i++) {
        QList<QRightClickButton*> *list = new QList<QRightClickButton*>();
        for (int j=0; j<columnCount; j++) {
            list->append(d2Buttons[i][j]);
        }
        buttons.append(*list);
    }

    QTimer::singleShot(10, this, [=]{
        classInfo.lineCt = lineCount;
        classInfo.columnCt = columnCount;
        classInfo.mineCt = mineCount;

        QRect rec = this->rect();
        int height = rec.height();

        fontSize = height/25;
    });
}

mineTable::~mineTable()
{
    delete ui;
}

void mineTable::prepareButton(QRightClickButton *button)
{
    button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(button, SIGNAL(released(QMouseEvent*)), this, SLOT(buttonReleasedSlot(QMouseEvent*)));
}

void mineTable::paintButton(QPushButton *button, int mineCount)
{
    button->setText(QString::number(mineCount));
    button->setStyleSheet(QString("QPushButton {font-weight:bold; font:%1px; color:%2; background-color:white;}").arg(QString::number(fontSize)).arg(colors.getColorString(mineCount)));

}

void mineTable::openSpace(QPoint loc)
{
    for (int i=loc.x()-1; i<=loc.x()+1; i++)
        for (int j=loc.y()-1; j<=loc.y()+1; j++)
            if(i!=loc.x() || j!=loc.y())
            if(i >= 0 && j >= 0)
            if(i < buttons.count() && j < buttons.at(i).count())
            if(!buttons.at(i).at(j)->property("flagged").toBool())
            if(buttons.at(i).at(j)->property("clicked").toBool()==false){
                buttons.at(i).at(j)->setProperty("clicked",true);
                if(!buttons.at(i).at(j)->property("mine").toBool()){
                    int mineCtr, flagCtr;
                    mineCtr = checkBordersMine(QPoint(i,j));
                    flagCtr = checkBordersFlags(QPoint(i,j));
                    if(mineCtr > 0){
                        paintButton(buttons.at(i).at(j), mineCtr);
                        if(flagCtr == mineCtr){
                            openSpace(QPoint(i,j));
                        }
                    }
                    else{
                        if(!buttons.at(i).at(j)->property("flagged").toBool()){
                            buttons.at(i).at(j)->setStyleSheet("QPushButton {background-color:white;}");
                            openSpace(QPoint(i,j));
                        }
                    }
                }
                else{
                    QMessageBox msgBox;
                    msgBox.setText("Game Over.");
                    msgBox.exec();
                }
            }
}

int mineTable::checkBordersMine(QPoint loc)
{
    int minectr = 0;
    for (int i=loc.x()-1; i<=loc.x()+1; i++) {
        for (int j=loc.y()-1; j<=loc.y()+1; j++) {
            if(i==loc.x() && j==loc.y()){

            }
            else{
                if(i >= 0 && j >= 0){
                    if(i < buttons.count() && j < buttons.at(i).count()){
                        if(buttons.at(i).at(j)->property("mine").toBool()==true){
                            minectr++;
                        }
                    }
                }
            }
        }
    }

    return minectr;
}

int mineTable::checkBordersFlags(QPoint loc)
{
    int flagctr = 0;
    for (int i=loc.x()-1; i<=loc.x()+1; i++) {
        for (int j=loc.y()-1; j<=loc.y()+1; j++) {
            if(i==loc.x() && j==loc.y()){

            }
            else{
                if(i >= 0 && j >= 0){
                    if(i < buttons.count() && j < buttons.at(i).count()){
                        if(buttons.at(i).at(j)->property("flagged").toBool()==true){
                            flagctr++;
                        }
                    }
                }
            }
        }
    }

    return flagctr;
}

void mineTable::buttonReleasedSlot(QMouseEvent *e)
{
    QRightClickButton* buttonSender = qobject_cast<QRightClickButton*>(sender());

    if(e->button()==Qt::LeftButton){
        if(initF == false){
            initF = true;
            for (int i=0; i<classInfo.mineCt; i++) {
                if(i>=classInfo.lineCt*classInfo.columnCt-1){
                    break;
                }
                int x = QRandomGenerator::global()->generateDouble()*classInfo.lineCt;
                int y = QRandomGenerator::global()->generateDouble()*classInfo.columnCt;

                int forbiddenX = buttonSender->property("buttonLoc").toPoint().x();
                int forbiddenY = buttonSender->property("buttonLoc").toPoint().y();

                if(!buttons.at(x).at(y)->property("mine").toBool()){
                    if(x!=forbiddenX || y!=forbiddenY)
                        buttons.at(x).at(y)->setProperty("mine", true);
                    else
                        i--;
                }
                else
                    i--;
            }
        }
        if(!buttonSender->property("flagged").toBool()){
            QPoint loc = buttonSender->property("buttonLoc").toPoint();
            if(buttonSender->property("clicked").toBool()==false){
                buttonSender->setProperty("clicked",true);
                if(buttonSender->property("mine").toBool()){
                    QMessageBox msgBox;
                    msgBox.setText("Game Over.");
                    msgBox.exec();
                }
                else{
                    int minectr = 0;

                    minectr = checkBordersMine(loc);
                    if(minectr == 0){
                        buttons.at(loc.x()).at(loc.y())->setStyleSheet("QPushButton {background-color:white;}");
                        openSpace(loc);
                    }
                    else{
                        paintButton(buttonSender, minectr);
                    }
                }
            }
            else{
                int flagCtr = checkBordersFlags(loc);
                if(flagCtr == buttonSender->text().toInt()){
                    openSpace(loc);
                }
            }
        }
    }
    if(e->button()==Qt::RightButton){
        if(!buttonSender->property("clicked").toBool()){
            if(buttonSender->property("flagged").toBool()){
                buttonSender->setText(" ");
                buttonSender->setStyleSheet(QString("QPushButton {font-weight:bold; font:%1px; background-color:rgb(255, 219, 166);}").arg(QString::number(fontSize)));
                buttonSender->setProperty("flagged", false);
            }
            else{
                buttonSender->setText("!");
                buttonSender->setStyleSheet(QString("QPushButton {font-weight:bold; font:%1px; background-color:rgb(255, 219, 166);}").arg(QString::number(fontSize)));
                buttonSender->setProperty("flagged", true);
            }
        }
    }

    int openedCount = 0;
    for (int i=0; i<buttons.count(); i++) {
        for (int j=0; j<buttons.at(i).count(); j++) {
            QRightClickButton *button = buttons.at(i).at(j);
            if(button->property("clicked").toBool())
                openedCount++;
        }
    }
    if(openedCount+classInfo.mineCt == classInfo.lineCt*classInfo.columnCt){
        QMessageBox msgBox;
        msgBox.setText("Congrats.");
        msgBox.exec();
    }
}
