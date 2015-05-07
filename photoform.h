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
    void createContextMenu();

private:
    QLabel *photoLabel;
    QString journalID;
    int pageNumber;
    int pageMax;
    QAction *editTag;

protected:
    void mousePressEvent(QMouseEvent *);
};

#endif // PHOTOFORM_H
