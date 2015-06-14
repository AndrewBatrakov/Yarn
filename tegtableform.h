#ifndef TEGFORM_H
#define TEGFORM_H

#include <QtWidgets>

class TegTableForm : public QDialog
{
    Q_OBJECT

public:
    TegTableForm(QString, QString, QWidget *, bool);
    void done(int result);
    QString rowOut() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void saveRecord();

    void addRecordOfTable();
    void deleteRecordOfTable();
    void editRecordOfTable();

    void readSettings();
    void writeSettings();

    void sortTable(int);

private:

    QPushButton *saveButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;
    QString indexJournalPhoto;

    QPushButton *addRecordButton;
    QPushButton *editRecordButton;
    QPushButton *deleteRecordButton;

    QAction *addAction;
    QAction *deleteAction;
    QAction *editAction;

    QTableWidget *tableWidget;
    QFile exchangeFile;
};

#endif // TEGFORM_H
