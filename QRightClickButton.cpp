#include "QRightClickButton.h"

QRightClickButton::QRightClickButton(QWidget *parent) :
    QPushButton(parent)
{
}

void QRightClickButton::mousePressEvent(QMouseEvent *e)
{
    emit clicked(e);
}

void QRightClickButton::mouseReleaseEvent(QMouseEvent *e)
{
    emit released(e);
}
