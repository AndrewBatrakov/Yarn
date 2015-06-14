#ifndef TEGINPUTFORM_H
#define TEGINPUTFORM_H

#include <QtWidgets>
#include "lineedit.h"

class TegForm : public QDialog
{
    Q_OBJECT

public:
    TegForm(QString, QWidget *, bool);
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

#endif // TEGINPUTFORM_H
