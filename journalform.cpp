#include "journalform.h"
#include <QtSql>
#include "numprefix.h"
#include "fordelete.h"

JournalForm::JournalForm(QString id, QWidget *parent, bool onlyForRead) :
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

    listWidget = new QListWidget(this);

    listWidget->setIconSize(QSize(128,128));
    listWidget->setViewMode(QListView::IconMode);

    QListWidgetItem *listItem = new QListWidgetItem(listWidget);

    listItem->setText("Add Button");
    listItem->setIcon(QPixmap(":/add.png"));

    connect(listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(itemClicked()));
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
        query.prepare("SELECT journalname FROM journal WHERE journalid = ?");
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
    mainLayout->addWidget(listWidget,1,0,3,2);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,4,1);
        editForm->selectAll();
    }

    setLayout(mainLayout);
    setWindowTitle(tr("Journal"));

    createContextMenu();
}

void JournalForm::editRecord()
{
    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("UPDATE journal SET journalname = :name WHERE journalid = :id");
        query.bindValue(":name",editForm->text());
        query.bindValue(":id",indexTemp);
        query.exec();
        //nameTemp = editForm->text();
    }else{
        QSqlQuery query;
        query.prepare("SELECT * FROM journal WHERE journalname = :name");
        query.bindValue(":name",editForm->text().simplified());
        query.exec();
        query.next();
        if(!query.isValid()){
            NumPrefix numPrefix(this);
            indexTemp = numPrefix.getPrefix("journal");
            if(indexTemp == ""){
                close();
            }else{
                QSqlQuery query;
                query.prepare("INSERT INTO journal (journalid, journalname) VALUES(:id, :name)");
                query.bindValue(":id",indexTemp);
                query.bindValue(":name",editForm->text().simplified());
                query.exec();
                //nameTemp = editOrganization->text();
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

void JournalForm::deleteRecord()
{
    ForDelete forDelete(indexTemp,"journal",this);

    int tt = forDelete.result();
    //if(forDelete.result() != 0){
    //    forDelete.exec();
    //int k = QMessageBox::warning(this,tr("Attention!!!"),tr("Delete item with the replacement for default value?"),
    //                     QMessageBox::No|QMessageBox::Yes,QMessageBox::No);
    //if(k == QMessageBox::Yes){
    forDelete.deleteOnDefault();
    if(indexTemp != "OWN000000001"){
        QSqlQuery query;
        query.prepare("DELETE FROM journal WHERE journalid = :id");
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

void JournalForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void JournalForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Yarn");
    restoreGeometry(settings.value("JournalForm").toByteArray());
}

void JournalForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Yarn");
    settings.setValue("JournalForm", saveGeometry());
}

void JournalForm::itemClicked()
{
    qDebug()<<listWidget->currentIndex();
}

void JournalForm::createContextMenu()
{
    addAction = new QAction(tr("Add Record"),this);
    QPixmap pixAdd(":/add.png");
    addAction->setIcon(pixAdd);
    //connect(addAction,SIGNAL(triggered()),this,SLOT(addRecordOfTable()));

    QPixmap pixDelete(":/delete.png");
    deleteAction = new QAction(tr("Delete Record"),this);
    deleteAction->setIcon(pixDelete);
    //connect(deleteAction,SIGNAL(triggered()),this,SLOT(deleteRecordOfTable()));

    QPixmap pixEdit(":/edit.png");
    editAction = new QAction(tr("Edit Record"),this);
    editAction->setIcon(pixEdit);
    //connect(editAction,SIGNAL(triggered()),this,SLOT(editRecordOfTable()));

    listWidget->addAction(addAction);
    listWidget->addAction(deleteAction);
    listWidget->addAction(editAction);
    listWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
}
