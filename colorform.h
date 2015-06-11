#ifndef COLOR_H
#define COLOR_H

#include <QtWidgets>
#include "lineedit.h"

class ColorForm : public QDialog
{
    Q_OBJECT

public:
    ColorForm(QString, QWidget *, bool);
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

#endif // COLOR_H
