#include "unitform.h"
#include <QtSql>
#include "numprefix.h"
#include "fordelete.h"

UnitForm::UnitForm(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    readSettings();
    indexTemp = id;

    labelForm = new QLabel(tr("Name:"));
    editForm = new LineEdit;
    editForm->setReadOnly(onlyForRead);
    QRegExp regExp("[\\x0410-\\x044f 0-9 \" -]{150}");
    editForm->setValidator(new QRegExpValidator(regExp,this));
    labelForm->setBuddy(editForm);

    saveButton = new QPushButton(tr("Save"));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(editRecord()));
    saveButton->setToolTip(tr("Save And Close Button"));

    cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setDefault(true);
    cancelButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(accept()));
    cancelButton->setToolTip(tr("Cancel Button"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(saveButton,QDialogButtonBox::ActionRole);

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT unitname FROM unit WHERE unitid = ?");
        query.addBindValue(indexTemp);
        query.exec();
        while(query.next()){
            editForm->setText(query.value(0).toString());
        }
    }else{
        editForm->clear();
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelForm,0,0);
    mainLayout->addWidget(editForm,0,1);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,1,1);
        editForm->selectAll();
    }

    setLayout(mainLayout);

    setWindowTitle(tr("Unit"));
    exchangeFile.setFileName("exchange.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }
}

void UnitForm::editRecord()
{
    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("UPDATE unit SET unitname = :name WHERE unitid = :id");
        query.bindValue(":name",editForm->text());
        query.bindValue(":id",indexTemp);
        query.exec();
    }else{
        QSqlQuery query;
        query.prepare("SELECT * FROM unit WHERE unitname = :name");
        query.bindValue(":name",editForm->text().simplified());
        query.exec();
        query.next();
        if(!query.isValid()){
            NumPrefix numPrefix(this);
            indexTemp = numPrefix.getPrefix("unit");
            if(indexTemp == ""){
                close();
            }else{
                QSqlQuery query;
                query.prepare("INSERT INTO unit (unitid, unitname) VALUES(:id, :name)");
                query.bindValue(":id",indexTemp);
                query.bindValue(":name",editForm->text().simplified());
                query.exec();
            }
        }else{
            QString tempString = editForm->text();
            tempString += QObject::tr(" is availble!");
            QMessageBox::warning(this,QObject::tr("Atention!!!"),tempString);
        }
    }
    emit accept();
    close();
}

void UnitForm::deleteRecord()
{
    ForDelete forDelete(indexTemp,"unit",this);

    forDelete.result();
    forDelete.deleteOnDefault();
    QSqlQuery query;
    query.prepare("DELETE FROM unit WHERE unitid = :id");
    query.bindValue(":id",indexTemp);
    query.exec();
    query.next();
}

void UnitForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void UnitForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Yarn");
    restoreGeometry(settings.value("Unit").toByteArray());
}

void UnitForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Yarn");
    settings.setValue("Unit", saveGeometry());
}
