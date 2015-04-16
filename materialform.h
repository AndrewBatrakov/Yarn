#ifndef MATERIALFORM_H
#define MATERIALFORM_H

#include <QtWidgets>
#include <lineedit.h>

class MaterialForm : public QDialog
{
    Q_OBJECT
public:
    MaterialForm(QString,QWidget *,bool);
    void done(int result);
    QString rowOut() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();
    void readSettings();
    void writeSettings();
    void correctPercent(QString);

private:

    QLabel *labelMaterial;
    LineEdit *editMaterial;
    QLabel *labelPersent;
    LineEdit *editPersent;

    QPushButton *saveButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;
};

#endif // MATERIALFORM_H
