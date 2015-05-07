#include "photoform.h"
#include <QtSql>
#include "tegform.h"

PhotoForm::PhotoForm(QString idJournal, int page, QWidget *parent) : QDialog(parent)
{
    journalID = idJournal;
    pageNumber = page;
    photoLabel  = new QLabel;

    maxCount();

    QSqlQuery query;
    query.prepare("SELECT journalphoto FROM journalphoto WHERE (journalid = :journalid AND page = :page)");
    query.bindValue(":journalid",journalID);
    query.bindValue(":page",pageNumber);
    query.exec();
    query.next();

    QByteArray imageByte = query.value(0).toByteArray();
    QImage pixMap;
    pixMap.loadFromData(imageByte);
    //QImage re = pixMap.scaled(100,200,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    photoLabel->setPixmap(QPixmap::fromImage(pixMap));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(photoLabel);
    setLayout(mainLayout);

    setWindowTitle(tr("Pages Of Journal"));
    //createContextMenu();
}


void PhotoForm::mousePressEvent(QMouseEvent *mouseEvent)
{
    if(mouseEvent->button() == Qt::LeftButton){
        if(mouseEvent->localPos().x() < size().width()/2){
            if(--pageNumber == 0)
                pageNumber = pageMax;
        }else{
            if(++pageNumber > pageMax)
                pageNumber = 1;
        }

        QSqlQuery query;
        query.prepare("SELECT journalphoto FROM journalphoto WHERE (journalid = :journalid AND page = :page)");
        query.bindValue(":journalid",journalID);
        query.bindValue(":page",pageNumber);
        query.exec();
        while(query.next()){
            QByteArray imageByte = query.value(0).toByteArray();
            QImage pixMap;
            pixMap.loadFromData(imageByte);
            photoLabel->setPixmap(QPixmap::fromImage(pixMap));
            int hei = QApplication::desktop()->height();
            if(hei < photoLabel->height()){
                this->setMaximumHeight(hei);
            }else{
                this->resize(photoLabel->sizeHint());
            }
        }
    }else if(mouseEvent->button() == Qt::RightButton){
        QMenu menu(photoLabel);
        QPixmap pixD(":/edit.png");
        menu.addAction(pixD,tr("Edit Tags For Search"));
        menu.setContextMenuPolicy(Qt::ActionsContextMenu);
        QAction *m = menu.exec(mouseEvent->globalPos());
        if(m){
            TegForm tegForm(this);
            tegForm.exec();
        }
    }
}

void PhotoForm::maxCount()
{
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM journalphoto");
    query.next();
    pageMax = query.value(0).toInt();
}

void PhotoForm::createContextMenu()
{
//    editTag = new QAction(tr("Edit Tags For Search"),photoLabel);
//    photoLabel->addAction(editTag);
//    photoLabel->setContextMenuPolicy(Qt::ActionsContextMenu);

}
