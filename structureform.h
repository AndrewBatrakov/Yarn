#ifndef STRUCTUREFORM_H
#define STRUCTUREFORM_H

#include <QtWidgets>
#include <QtSql>
#include "lineedit.h"

class StructureForm : public QDialog
{
    Q_OBJECT

public:
    StructureForm(QString,QWidget *,bool);
    void done(int result);
    QString rowOut() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();

    void saveRecord();
    void cancelRecord();

    void addRecordOfTable();
    void deleteRecordOfTable();
    void editRecordOfTable();

    void readSettings();
    void writeSettings();

    void sortTable(int);

private:

    QLabel *labelStructure;
    LineEdit *editStructure;

    QPushButton *saveButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;
    QString structureTableId;

    QPushButton *addRecordButton;
    QPushButton *editRecordButton;
    QPushButton *deleteRecordButton;

    QAction *addAction;
    QAction *deleteAction;
    QAction *editAction;

    QTableWidget *tableWidget;
};

#endif // STRUCTUREFORM_H
