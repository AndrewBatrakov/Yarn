#include "materialform.h"
#include <QtSql>
#include "fordelete.h"
#include "numprefix.h"

MaterialForm::MaterialForm(QString id, QWidget *parent, bool onlyForRead) : QDialog(parent)
{
    readSettings();
    indexTemp = id;

    labelMaterial = new QLabel(tr("Name:"));
    editMaterial = new LineEdit;
    editMaterial->setReadOnly(onlyForRead);
    QRegExp regExp("[\\x0410-\\x044f 0-9 \" -]{150}");
    editMaterial->setValidator(new QRegExpValidator(regExp,this));
    labelMaterial->setBuddy(editMaterial);

    labelPersent = new QLabel(tr("Percent:"));
    editPersent = new LineEdit;
    editPersent->setReadOnly(onlyForRead);
    QRegExp regExpP("[0-9]{3}");
    editPersent->setValidator(new QRegExpValidator(regExpP,this));
    labelPersent->setBuddy(editPersent);
    connect(editPersent,SIGNAL(textChanged(QString)),this,SLOT(correctPercent(QString)));

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
        query.prepare("SELECT materialname, persent FROM material WHERE materialid = ?");
        query.addBindValue(indexTemp);
        query.exec();
        while(query.next()){
            editMaterial->setText(query.value(0).toString());
            editPersent->setText(query.value(1).toString());
        }
    }else{
        editMaterial->clear();
        editPersent->clear();
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelMaterial,0,0);
    mainLayout->addWidget(editMaterial,0,1);
    mainLayout->addWidget(labelPersent,1,0);
    mainLayout->addWidget(editPersent,1,1);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,2,1);
        editMaterial->selectAll();
        editPersent->selectAll();
    }

    setLayout(mainLayout);

    setWindowTitle(tr("Material"));
}

void MaterialForm::deleteRecord()
{
    ForDelete forDelete(indexTemp,"material",this);

    int tt = forDelete.result();
    //if(forDelete.result() != 0){
    //    forDelete.exec();
    //int k = QMessageBox::warning(this,tr("Attention!!!"),tr("Delete item with the replacement for default value?"),
    //                     QMessageBox::No|QMessageBox::Yes,QMessageBox::No);
    //if(k == QMessageBox::Yes){
    forDelete.deleteOnDefault();
    if(indexTemp != "OWN000000001"){
        QSqlQuery query;
        query.prepare("DELETE FROM material WHERE materialid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();
    }else{
        QMessageBox::warning(this,QObject::tr("Attention"),QObject::tr("You dont may delete default value!"));
    }
    // }
    // }
    /*else{
           if(indexTemp != "OWN000000001"){
               QSqlQuery query;
               query.prepare("DELETE FROM organization WHERE organizationid = :id");
               query.bindValue(":id",indexTemp);
               query.exec();
               query.next();
           }else{
               QMessageBox::warning(this,QObject::tr("Attention"),QObject::tr("You dont may delete default value!"));
           }
       }*/

}

void MaterialForm::editRecord()
{
    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("UPDATE material SET materialname = :name, persent = :persent WHERE materialid = :id");
        query.bindValue(":name",editMaterial->text());
        query.bindValue(":persent",editPersent->text().toInt());
        query.bindValue(":id",indexTemp);
        query.exec();
        //nameTemp = editForm->text();
    }else{
        QSqlQuery query;
        query.prepare("SELECT * FROM material WHERE (materialname = :name AND persent = :persent)");
        query.bindValue(":name",editMaterial->text().simplified());
        query.bindValue(":persent",editPersent->text().toInt());
        query.exec();
        query.next();
        if(!query.isValid()){
            NumPrefix numPrefix(this);
            indexTemp = numPrefix.getPrefix("material");
            if(indexTemp == ""){
                close();
            }else{
                QSqlQuery query;
                query.prepare("INSERT INTO material (materialid, materialname, persent)"
                              " VALUES(:id, :name, :persent)");
                query.bindValue(":id",indexTemp);
                query.bindValue(":name",editMaterial->text().simplified());
                query.bindValue(":persent",editPersent->text().toInt());
                query.exec();
                //nameTemp = editOrganization->text();
            }
        }else{
            QString tempString = editMaterial->text();
            tempString += " ";
            tempString += editPersent->text();
            tempString += QObject::tr("% is availble!");
            QMessageBox::warning(this,QObject::tr("Atention!!!"),tempString);
        }
    }
    emit accept();
    close();
}

void MaterialForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void MaterialForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Yarn");
    restoreGeometry(settings.value("Material").toByteArray());
}

void MaterialForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Yarn");
    settings.setValue("Material", saveGeometry());
}

void MaterialForm::correctPercent(QString str)
{
    if(str.toInt() > 100)
        editPersent->setText("100");
}
