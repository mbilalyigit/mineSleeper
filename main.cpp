#include "minesleeper.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mineSleeper w;
    w.show();
    w.showFullScreen();
    return a.exec();
}
