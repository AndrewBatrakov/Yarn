#ifndef PHOTOFORM_H
#define PHOTOFORM_H

#include <QtWidgets>

class PhotoForm : public QDialog
{
    Q_OBJECT
public:
    PhotoForm(QString, int, QWidget *);

private slots:
    void maxCount();

private:
    QLabel *photoLabel;
    QString journalID;
    int pageNumber;
    int pageMax;

protected:
    void mousePressEvent(QMouseEvent *);
};

#endif // PHOTOFORM_H
