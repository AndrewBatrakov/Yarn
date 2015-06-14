#include "exchangeform.h"
#include <QtSql>

exchangeForm::exchangeForm(QWidget *parent) : QDialog(parent)
{
    exchangeButton = new QPushButton(tr("Exchange"));
    connect(exchangeButton,SIGNAL(clicked()),this,SLOT(accept()));
    exchangeButton->setDefault(true);


    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(exchangeButton,QDialogButtonBox::ActionRole);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(buttonBox,0,0);
    setLayout(mainLayout);
    setWindowTitle(tr("Exchange Data Base"));
    QFile file("exchange1.txt");
    file.open(QIODevice::ReadOnly);
    QSqlQuery query;
    QTextStream stream(&file);
    while(!stream.atEnd()){
        QString str = stream.readLine();
        query.exec(str);
        qDebug()<<query.lastError().text();
    }
}

