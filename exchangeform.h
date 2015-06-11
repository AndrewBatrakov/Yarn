#ifndef EXCHNGEFORM_H
#define EXCHNGEFORM_H

#include <QtWidgets>

class ExchngeForm : public QDialog
{
    Q_OBJECT
public:
    ExchngeForm(QWidget *parent);

private:
    QPushButton *exchangeButton;
    QDialogButtonBox *buttonBox;
};

#endif // EXCHNGEFORM_H
