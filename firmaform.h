#ifndef FIRMAFORM_H
#define FIRMAFORM_H

#include <QtWidgets>
#include "lineedit.h"

class FirmaForm : public QDialog
{
    Q_OBJECT

public:
    FirmaForm(QString, QWidget *, bool);
    void done(int result);
    QString rowOut() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();
    void readSettings();
    void writeSettings();

private:

    QLabel *labelForm;
    LineEdit *editForm;

    QPushButton *saveButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;
    QFile exchangeFile;
};

#endif // FIRMAFORM_H
