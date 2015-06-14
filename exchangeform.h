#ifndef exchangeFORM_H
#define exchangeFORM_H

#include <QtWidgets>

class exchangeForm : public QDialog
{
    Q_OBJECT
public:
    exchangeForm(QWidget *parent);

private:
    QPushButton *exchangeButton;
    QDialogButtonBox *buttonBox;
};

#endif // exchangeFORM_H
