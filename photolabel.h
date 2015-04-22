#ifndef PHOTOLABEL_H
#define PHOTOLABEL_H

#include <QtWidgets>

class PhotoLabel : public QLabel
{
    Q_OBJECT

public:
    PhotoLabel(QWidget *parent = 0);

signals:
    void clicked();

protected:
    void mouseReleaseEvent(QMouseEvent *);
};

#endif // PHOTOLABEL_H
