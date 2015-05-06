#include "photoform.h"
#include <QtSql>

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

}


void PhotoForm::mousePressEvent(QMouseEvent *mouseEvent)
{
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
        this->resize(photoLabel->sizeHint());
    }
}

void PhotoForm::maxCount()
{
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM journalphoto");
    query.next();
    pageMax = query.value(0).toInt();
}
