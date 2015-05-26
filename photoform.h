#ifndef PHOTOFORM_H
#define PHOTOFORM_H

#include <QtWidgets>
#include "phototeg.h"
#include "lineedit.h"

class PhotoForm : public QDialog
{
    Q_OBJECT
public:
    PhotoForm(QString, int, QWidget *, bool);

private slots:
    void maxCount();
    void editRecord();

private:
    QLabel *pageLabel;
    LineEdit *pageEdit;
    bool yesEdit;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;

    QLabel *photoLabel;
    QString journalID;
    int pageNumber;
    int pageMax;
    QAction *editTag;
    QString indexTemp;

protected:
    void mousePressEvent(QMouseEvent *);
};

#endif // PHOTOFORM_H
