#include "tegform.h"
#include <QtSql>
#include "numprefix.h"
#include "fordelete.h"

TegForm::TegForm(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    readSettings();
    indexTemp = id;

    labelForm = new QLabel(tr("Name:"));
    editForm = new LineEdit;
    editForm->setReadOnly(onlyForRead);
    //QRegExp regExp("[\\x0410-\\x044f 0-9 \" -]{150}");
    //editForm->setValidator(new QRegExpValidator(regExp,this));
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
        query.prepare("SELECT tegname FROM teg WHERE tegid = ?");
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

    setWindowTitle(tr("Teg"));
    exchangeFile.setFileName("exchange.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }
}

void TegForm::editRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }
    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("UPDATE teg SET tegname = :name, teglowname = :teglowname WHERE tegid = :id");
        query.bindValue(":name",editForm->text().simplified());
        query.bindValue(":id",indexTemp);
        query.bindValue(":teglowname",editForm->text().toLower().simplified());
        query.exec();
        line += "UPDATE teg SET tegname = '";
        line += editForm->text();
        line += "', teglowname = '";
        line += editForm->text().toLower().simplified();
        line += "' WHERE tegid = '";
        line += indexTemp;
        line += "'";
        line += "\r\n";
        stream<<line;
    }else{
        QSqlQuery query;
        query.prepare("SELECT * FROM teg WHERE teglowname = :name");
        query.bindValue(":name",editForm->text().toLower().simplified());
        query.exec();
        query.next();
        if(!query.isValid()){
            NumPrefix numPrefix(this);
            indexTemp = numPrefix.getPrefix("teg");
            if(indexTemp == ""){
                close();
            }else{
                QSqlQuery query;
                query.prepare("INSERT INTO teg (tegid, tegname, teglowname) VALUES(:id, :name, :teglowname)");
                query.bindValue(":id",indexTemp);
                query.bindValue(":name",editForm->text().simplified());
                query.bindValue(":teglowname",editForm->text().toLower().simplified());
                query.exec();
                line += "INSERT INTO teg (tegid, tegname, teglowname) VALUES('";
                line += indexTemp;
                line += "', '";
                line += editForm->text();
                line += "', '";
                line += editForm->text().toLower().simplified();
                line += "')";
                line += "\r\n";
                stream<<line;
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

void TegForm::deleteRecord()
{
    ForDelete forDelete(indexTemp,"teg",this);

    forDelete.result();
    forDelete.deleteOnDefault();
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }
    QSqlQuery query;
    query.prepare("DELETE FROM teg WHERE tegid = :id");
    query.bindValue(":id",indexTemp);
    query.exec();
    query.next();
    line += "DELETE FROM teg WHERE tegid = '";
    line += indexTemp;
    line += "'";
    line += "\r\n";
    stream<<line;
}

void TegForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void TegForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Yarn");
    restoreGeometry(settings.value("TegForm").toByteArray());
}

void TegForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Yarn");
    settings.setValue("TegForm", saveGeometry());
}
