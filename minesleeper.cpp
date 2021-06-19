#include "minesleeper.h"
#include "ui_minesleeper.h"


#include <QTimer>
#include <QFrame>
#include <QLabel>
#include <QMessageBox>

mineSleeper::mineSleeper(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mineSleeper)
{
    ui->setupUi(this);

    mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);

    QFrame *menuFrame = new QFrame();
    mainLayout->addWidget( menuFrame );
    menuFrame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

    menuLayout = new QHBoxLayout();
    menuFrame->setLayout(menuLayout);


    QPushButton *startButton = new QPushButton("Start");
    menuLayout->addWidget(startButton);

    QVBoxLayout *xSelectorLayout = new QVBoxLayout();
    menuLayout->addLayout(xSelectorLayout);
    QLabel * xLabel = new QLabel("X:");
    xSelectorLayout->addWidget(xLabel);
    xEdit = new QLineEdit();
    xSelectorLayout->addWidget(xEdit);

    QVBoxLayout *ySelectorLayout = new QVBoxLayout();
    menuLayout->addLayout(ySelectorLayout);
    QLabel * yLabel = new QLabel("Y:");
    ySelectorLayout->addWidget(yLabel);
    yEdit = new QLineEdit();
    ySelectorLayout->addWidget(yEdit);


    QVBoxLayout *mineSelectorLayout = new QVBoxLayout();
    menuLayout->addLayout(mineSelectorLayout);
    QLabel * mineLabel = new QLabel("Mine:");
    mineSelectorLayout->addWidget(mineLabel);
    mineEdit = new QLineEdit();
    mineSelectorLayout->addWidget(mineEdit);

    connect(startButton, SIGNAL(released()), this, SLOT(startSlot()));


    xEdit->setText(QString::number(20));
    yEdit->setText(QString::number(20));
    mineEdit->setText(QString::number(50));

    xEdit->setProperty("name", QString("X"));
    yEdit->setProperty("name", QString("Y"));
    mineEdit->setProperty("name", QString("Mine"));

    startButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    xEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    yEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    mineEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

mineSleeper::~mineSleeper()
{
    delete ui;
}

int mineSleeper::checkLineEdits()
{
    auto result = [=](QLineEdit *lineEdit){
        int retval = 0;
        if(lineEdit->text().isEmpty()){
            // text is empty
            QMessageBox(QMessageBox::NoIcon,"",QString("%1 Setting is empty").arg(lineEdit->property("name").toString())).exec();
            retval++;
        }
        else{
            int x;
            try {
                x = lineEdit->text().toInt();
            }  catch (...) {
                // not integer
                QMessageBox(QMessageBox::NoIcon,"",QString("Can't conver %1 Setting to integer").arg(lineEdit->property("name").toString())).exec();
                retval++;
            }
            if(x < 1){
                // should be bigger than 0
                QMessageBox(QMessageBox::NoIcon,"",QString("%1 Setting should be more than 0").arg(lineEdit->property("name").toString())).exec();
                retval++;
            }

        }
        return retval;
    };

    int status = 0;
    status += result(xEdit);
    status += result(yEdit);
    status += result(mineEdit);

    return status;

}

void mineSleeper::startSlot()
{
    if(tableExistF){
        delete table;
    }

    int status = checkLineEdits();

    if(status == 0){
        table = new mineTable(xEdit->text().toInt(),yEdit->text().toInt(),mineEdit->text().toInt());
        mainLayout->addWidget(table);
        tableExistF = true;
    }
}

