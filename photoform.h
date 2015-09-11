#ifndef PHOTOFORM_H
#define PHOTOFORM_H

#include <QtWidgets>
#include "lineedit.h"

class PhotoForm : public QDialog
{
    Q_OBJECT
public:
    PhotoForm(QString, int, QWidget *, bool,bool);

private slots:
    void maxCount();
    void editRecord();
    void createContextMenu();
    void addTag();

private:
    QAction *editTagForm;
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
    //QString journalPhotoID;
    QFile exchangeFile;

protected:
    void mousePressEvent(QMouseEvent *);
};

#endif // PHOTOFORM_H
