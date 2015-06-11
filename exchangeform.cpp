#include "exchangeform.h"

ExchngeForm::ExchngeForm(QWidget *parent) : QDialog(parent)
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
}

