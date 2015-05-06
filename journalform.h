#ifndef JOURNALFORM_H
#define JOURNALFORM_H

#include <QtWidgets>
#include <QtGui>
#include "lineedit.h"

class JournalForm : public QDialog
{
    Q_OBJECT

public:
    JournalForm(QString, QWidget *, bool);
    void done(int result);
    QString rowOut() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();
    void readSettings();
    void writeSettings();

    void itemClicked();
    void createContextMenu();

    void addPhoto();
    void editPhoto();
    void deletePhoto();

private:
    QLabel *labelName;
    LineEdit *editName;
    QLabel *labelNumber;
    LineEdit *editNumber;
    QLabel *labelYear;
    LineEdit *editYear;

    QListWidget *listWidget;

    QAction *addAction;
    QAction *deleteAction;
    QAction *editAction;

    QPushButton *saveButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;
};

#endif // JOURNALFORM_H
