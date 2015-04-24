#ifndef YARNFORM_H
#define YARNFORM_H

#include <QtWidgets>
#include "lineedit.h"
#include "photolabel.h"

class YarnForm : public QDialog
{
    Q_OBJECT
public:
    YarnForm(QString, QWidget *, bool);
    void done(int result);
    QString rowOut() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();
    void readSettings();
    void writeSettings();

    void addColorRecord();
    void seeColorRecord();
    void listColorRecord();
    void addStructureRecord();
    void seeStructureRecord();
    void listStructureRecord();
    void addFirmaRecord();
    void seeFirmaRecord();
    void listFirmaRecord();

    void photoRead();

private:
    QLabel *labelName;
    LineEdit *editName;
    QLabel *labelColor;
    LineEdit *editColor;
    QLabel *labelLenght;
    LineEdit *editLenght;
    QLabel *labelWeight;
    LineEdit *editWeight;
    QLabel *labelStructure;
    LineEdit *editStructure;
    QLabel *labelFirma;
    LineEdit *editFirma;
    QLabel *labelThickness;
    LineEdit *editThickness;
    PhotoLabel *photoLabel;

    QPushButton *saveButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;
    QString indexTemp;
};

#endif // YARNFORM_H
