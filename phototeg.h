#ifndef PHOTOTEG_H
#define PHOTOTEG_H

#include <QtWidgets>

class PhotoTeg : public QLabel
{
    Q_OBJECT

public:
    PhotoTeg(QWidget *parent = 0);

signals:
    void clicked();

protected:
    void mouseReleaseEvent(QMouseEvent *);
};

#endif // PHOTOTEG_H
