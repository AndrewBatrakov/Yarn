#include "journalform.h"
#include <QtSql>
#include "numprefix.h"
#include "fordelete.h"
#include "photoform.h"

JournalForm::JournalForm(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    readSettings();
    indexTemp = id;

    labelName = new QLabel(tr("Name:"));
    editName = new LineEdit;
    editName->setReadOnly(onlyForRead);
    QRegExp regExp("[\\x0410-\\x044f 0-9 \" -]{150}");
    editName->setValidator(new QRegExpValidator(regExp,this));
    labelName->setBuddy(editName);

    labelNumber = new QLabel(tr("Number:"));
    editNumber = new LineEdit;
    editNumber->setReadOnly(onlyForRead);
    QRegExp regExpNum("[0-9]{2}");
    editNumber->setValidator(new QRegExpValidator(regExpNum,this));
    labelNumber->setBuddy(editNumber);

    labelYear = new QLabel(tr("Year:"));
    editYear = new LineEdit;
    editYear->setReadOnly(onlyForRead);
    QRegExp regExpY("[0-9]{4}");
    editYear->setValidator(new QRegExpValidator(regExpY,this));
    labelYear->setBuddy(editYear);

    saveButton = new QPushButton(tr("Save"));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(editRecord()));
    saveButton->setToolTip(tr("Save And Close Button"));

    listWidget = new QListWidget(this);

    listWidget->setIconSize(QSize(128,128));
    listWidget->setViewMode(QListView::IconMode);



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
        query.prepare("SELECT * FROM journal WHERE journalid = ?");
        query.addBindValue(indexTemp);
        query.exec();
        while(query.next()){
            editName->setText(query.value(1).toString());
            QSqlQuery queryPhoto;
            queryPhoto.prepare("SELECT * FROM journalphoto WHERE journalid = :id");
            queryPhoto.bindValue(":id",indexTemp);
            queryPhoto.exec();
            while(queryPhoto.next()){
                QListWidgetItem *listItem = new QListWidgetItem(listWidget);
                QString page = tr("Page ");
                page += queryPhoto.value(3).toString();
                listItem->setText(page);
                QByteArray imageByte = queryPhoto.value(2).toByteArray();
                QImage pixMap;
                pixMap.loadFromData(imageByte);
                QImage re = pixMap.scaled(100,200,Qt::KeepAspectRatio,Qt::SmoothTransformation);
                //photoLabel->setPixmap(QPixmap::fromImage(re));
                listItem->setIcon(QPixmap::fromImage(re));
            }
        }
    }else{
        editName->clear();
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelName,0,0);
    mainLayout->addWidget(editName,0,1);
    mainLayout->addWidget(labelNumber,1,0);
    mainLayout->addWidget(editNumber,1,1);
    mainLayout->addWidget(labelYear,2,0);
    mainLayout->addWidget(editYear,2,1);
    mainLayout->addWidget(listWidget,3,0,4,2);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,7,1);
        editName->selectAll();
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
        query.bindValue(":name",editName->text());
        query.bindValue(":id",indexTemp);
        query.exec();
    }else{
        QSqlQuery query;
        query.prepare("SELECT * FROM journal WHERE journalname = :name");
        query.bindValue(":name",editName->text().simplified());
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
                query.bindValue(":name",editName->text().simplified());
                query.exec();
                //nameTemp = editOrganization->text();
            }
        }else{
            QString tempString = editName->text();
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
    PhotoForm photoForm(indexTemp, (listWidget->currentRow() + 1),this);
    photoForm.exec();
}

void JournalForm::createContextMenu()
{
    addAction = new QAction(tr("Add Record"),this);
    QPixmap pixAdd(":/add.png");
    addAction->setIcon(pixAdd);
    connect(addAction,SIGNAL(triggered()),this,SLOT(addPhoto()));

    QPixmap pixDelete(":/delete.png");
    deleteAction = new QAction(tr("Delete Record"),this);
    deleteAction->setIcon(pixDelete);
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(deletePhoto()));

    QPixmap pixEdit(":/edit.png");
    editAction = new QAction(tr("Edit Record"),this);
    editAction->setIcon(pixEdit);
    connect(editAction,SIGNAL(triggered()),this,SLOT(editPhoto()));

    listWidget->addAction(addAction);
    listWidget->addAction(deleteAction);
    listWidget->addAction(editAction);
    listWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void JournalForm::addPhoto()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"),"",tr("Images (*.jpg *.gif *.png)"));
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly)){
        if(file.size() > 300000){
            int que = QMessageBox::warning(this,tr("Attention!"),
                                           tr("File size - %1 ,\nAre You sure?").arg(QString::number(file.size())),
                                           QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
            if(que == QMessageBox::No){
                return;
            }
        }
        QImage pixMap;
        pixMap.load(fileName);
        QImage re = pixMap.scaled(600,800,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        QByteArray imageByte;
        QBuffer buffer(&imageByte);
        buffer.open(QIODevice::WriteOnly);
        re.save(&buffer, "PNG"); // writes image into ba in PNG format

        QSqlQuery queryPhotoControl;
        queryPhotoControl.prepare("SELECT COUNT(*) FROM journalphoto WHERE journalid = :id");
        queryPhotoControl.bindValue(":id",indexTemp);
        queryPhotoControl.exec();
        queryPhotoControl.next();
        int pageNum = queryPhotoControl.value(0).toInt();
        ++pageNum;
        QSqlQuery queryPhoto;
        queryPhoto.prepare("INSERT INTO journalphoto (journalphotoid, journalid, "
                           "journalphoto, page) VALUES("
                           ":journalphotoid, :journalid, :journalphoto, :page)"
                           );
        queryPhoto.bindValue(":journalid",indexTemp);


        queryPhoto.bindValue(":journalphoto",imageByte);
        NumPrefix numPrefix(this);
        QString valID = numPrefix.getPrefix("journalphoto");
        queryPhoto.bindValue(":journalphotoid",valID);
        queryPhoto.bindValue(":page",pageNum);
        queryPhoto.exec();

        QListWidgetItem *listItem = new QListWidgetItem(listWidget);
        QString page = tr("Page ");
        page += QString::number(pageNum);
        listItem->setText(page);
        QImage reToItem = pixMap.scaled(100,200,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        listItem->setIcon(QPixmap::fromImage(reToItem));
        listWidget->repaint();
    }
}

void JournalForm::editPhoto()
{

}

void JournalForm::deletePhoto()
{

}
