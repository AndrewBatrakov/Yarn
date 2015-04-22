#ifndef STRUCTUREFORM_H
#define STRUCTUREFORM_H

#include <QtWidgets>
#include <QtSql>
#include "lineedit.h"
#include "photolabel.h"

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

    void photoRead();

private:

    QLabel *labelStructure;
    LineEdit *editStructure;
    PhotoLabel *labelPhoto;

    QPushButton *saveButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;

    QPushButton *addRecordButton;
    QPushButton *editRecordButton;
    QPushButton *deleteRecordButton;
    //QDialogButtonBox *tableButtonBox;

    QAction *addAction;
    QAction *deleteAction;
    QAction *editAction;

    QTableWidget *tableWidget;
    QSqlRelationalTableModel *templateModel;
};

#endif // STRUCTUREFORM_H
