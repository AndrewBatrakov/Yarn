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

    listWidget->setIconSize(QSize(300,400));
    listWidget->setViewMode(QListView::IconMode);
    listWidget->setAlternatingRowColors(true);

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
            editNumber->setText(query.value(2).toString());
            editYear->setText(query.value(3).toString());
            QSqlQuery queryPhoto;
            queryPhoto.prepare("SELECT * FROM journalphoto WHERE journalid = :id");
            queryPhoto.bindValue(":id",indexTemp);
            queryPhoto.exec();

            QByteArray imageByte;
            QImage pixMap;
            QImage re;
            while(queryPhoto.next()){
                QListWidgetItem *listItem = new QListWidgetItem(listWidget);
                QString page = tr("Page ");
                page += queryPhoto.value(3).toString();
                listItem->setText(page);
                imageByte = queryPhoto.value(2).toByteArray();
                pixMap.loadFromData(imageByte);
                re = pixMap.scaled(300,400,Qt::KeepAspectRatio,Qt::SmoothTransformation);
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
    exchangeFile.setFileName("exchange.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::Append);
    }else{
        exchangeFile.open(QFile::Append);
    }
}

void JournalForm::editRecord()
{
    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("UPDATE journal SET journalname = :name, number = :number, year = :year "
                      "WHERE journalid = :id");
        query.bindValue(":name",editName->text());
        query.bindValue(":id",indexTemp);
        query.bindValue(":number",editNumber->text().toInt());
        query.bindValue(":year",editYear->text().toInt());
        query.exec();
    }else{
        QSqlQuery query;
        query.prepare("SELECT * FROM journal WHERE journalname = :name "
                      "AND year = :year AND number = :number");
        query.bindValue(":name",editName->text().simplified());
        query.bindValue(":number",editNumber->text().toInt());
        query.bindValue(":year",editYear->text().toInt());
        query.exec();
        query.next();
        if(!query.isValid()){
            NumPrefix numPrefix(this);
            indexTemp = numPrefix.getPrefix("journal");
            if(indexTemp == ""){
                close();
            }else{
                QSqlQuery query;
                query.prepare("INSERT INTO journal (journalid, journalname, "
                              "number, year) VALUES(:id, :name, :number, :year)");
                query.bindValue(":id",indexTemp);
                query.bindValue(":name",editName->text().simplified());
                query.bindValue(":number",editNumber->text().toInt());
                query.bindValue(":year",editYear->text().toInt());
                query.exec();
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

    forDelete.result();
    forDelete.deleteOnDefault();
    QSqlQuery query;
    query.prepare("DELETE FROM journal WHERE journalid = :id");
    query.bindValue(":id",indexTemp);
    query.exec();
    query.next();
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
    QString tt = listWidget->item(listWidget->currentRow())->text();
    int page = tt.mid(5).toInt();
    PhotoForm photoForm(indexTemp, page,this,false,false);
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
    QDataStream stream(&exchangeFile);

    QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"),"",tr("Images (*.jpg *.gif *.png)"));
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly)){
//        if(file.size() > 300000){
//            int que = QMessageBox::warning(this,tr("Attention!"),
//                                           tr("File size - %1 ,\nAre You sure?").arg(QString::number(file.size())),
//                                           QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
//            if(que == QMessageBox::No){
//                return;
//            }
//        }
        QImage pixMap;
        pixMap.load(fileName);
        //Photo
        QImage re = pixMap.scaled(600,800,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        QByteArray imageByte;
        QBuffer buffer(&imageByte);
        buffer.open(QIODevice::WriteOnly);
        re.save(&buffer, "PNG"); // writes image into ba in PNG format

//        QImage icon = pixMap.scaled(300,400,Qt::KeepAspectRatio,Qt::SmoothTransformation);
//        QByteArray imageByteIcon;
//        QBuffer bufferIcon(&imageByteIcon);
//        bufferIcon.open(QIODevice::WriteOnly);
//        icon.save(&bufferIcon, "PNG"); // writes image into ba in PNG format

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
        //queryPhoto.bindValue(":journalicon",imageByteIcon);
        NumPrefix numPrefix(this);
        QString valID = numPrefix.getPrefix("journalphoto");
        queryPhoto.bindValue(":journalphotoid",valID);
        queryPhoto.bindValue(":page",pageNum);
        queryPhoto.exec();
        stream<<"INSERT INTO journalphoto (journalphotoid, journalid, "
                "journalphoto, journalicon, page) VALUES('";
        stream<<valID;
        stream<<"', '";
        stream<<indexTemp;
        stream<<"', '";
        stream<<imageByte;
        stream<<"', ";
//        stream<<imageByteIcon;
//        stream<<"', '";
        stream<<pageNum;
        stream<<"')";
        stream<<"\r\n";

        QListWidgetItem *listItem = new QListWidgetItem(listWidget);
        QString page = tr("Page ");
        page += QString::number(pageNum);
        listItem->setText(page);
        QImage reToItem = pixMap.scaled(300,400,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        listItem->setIcon(QPixmap::fromImage(reToItem));
        listWidget->repaint();
    }
}

void JournalForm::editPhoto()
{
    QString tt = listWidget->item(listWidget->currentRow())->text();
    int page = tt.mid(5).toInt();
    PhotoForm photoForm(indexTemp, page,this,true,false);
    photoForm.exec();
    listWidget->repaint();
}

void JournalForm::deletePhoto()
{
    int k = QMessageBox::warning(this,tr("Attention!!!"),tr("Really Delete Page Photo?"),
                                 QMessageBox::No|QMessageBox::Yes,QMessageBox::No);
    if(k == QMessageBox::Yes){
        QString tt = listWidget->item(listWidget->currentRow())->text();
        int page = tt.mid(5).toInt();
        QSqlQuery queryPh;
        queryPh.prepare("DELETE FROM journalphoto WHERE (journalid = :journalid AND page = :page)");
        queryPh.bindValue(":page",page);
        queryPh.bindValue(":journalid",indexTemp);
        queryPh.exec();
        queryPh.next();
        //listWidget->clear();
        //listWidget->setFlow(QListView::LeftToRight);
        listWidget->takeItem(listWidget->currentRow());
        //listWidget->repaint();
    }
}

void JournalForm::resizeEvent(QResizeEvent *)
{
    listWidget->setFlow(QListView::LeftToRight);
    listWidget->repaint();
}
