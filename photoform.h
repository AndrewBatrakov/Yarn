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
    void addTag();

    void upPictureSlot();
    void downPictureSlot();
    void createContextMenu();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

private:
    QAction *editTagForm;
    QAction *zoomIn;
    QAction *zoomOut;

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

    double scaledFactor;
    QScrollArea *scrollArea;

protected:
    void mousePressEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
};

#endif // PHOTOFORM_H
