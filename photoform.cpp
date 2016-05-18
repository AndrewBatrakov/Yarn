#include "photoform.h"
#include <QtSql>
#include "tegtableform.h"

PhotoForm::PhotoForm(QString idJournal, int page, QWidget *parent, bool edit,bool readOnly) : QDialog(parent)
{
    yesEdit = edit;
    journalID = idJournal;
    pageNumber = page;
    photoLabel  = new QLabel;
    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    //photoLabel->setScaledContents(true);

    maxCount();

    QSqlQuery query;
    query.prepare("SELECT journalphoto, journalphotoid FROM journalphoto WHERE (journalid = :journalid AND page = :page)");
    query.bindValue(":journalid",journalID);
    query.bindValue(":page",pageNumber);
    query.exec();
    query.next();

    if(yesEdit && !readOnly){
        pageLabel = new QLabel(tr("Page:"));
        pageEdit = new LineEdit;
        pageEdit->setText(QString::number(pageNumber));

        saveButton = new QPushButton(tr("Save"));
        connect(saveButton,SIGNAL(clicked()),this,SLOT(editRecord()));
        saveButton->setToolTip(tr("Save And Close Button"));

        cancelButton = new QPushButton(tr("Cancel"));
        cancelButton->setDefault(true);
        cancelButton->setStyleSheet("QPushButton:hover {color: red}");
        connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
        cancelButton->setToolTip(tr("Cancel Button"));

        buttonBox = new QDialogButtonBox;
        buttonBox->addButton(cancelButton,QDialogButtonBox::ActionRole);
        buttonBox->addButton(saveButton,QDialogButtonBox::ActionRole);
    }
    //journalPhotoID =query.value(1).toString();
    indexTemp = query.value(1).toString();
    QByteArray imageByte = query.value(0).toByteArray();
    QImage pixMap;
    QImage re;
    pixMap.loadFromData(imageByte);
    int widI = QPixmap::fromImage(pixMap).width();
    int hei = QApplication::desktop()->height()-100;
    re = pixMap.scaled(widI,hei,Qt::KeepAspectRatio,Qt::SmoothTransformation);

    photoLabel->setPixmap(QPixmap::fromImage(re));
    photoLabel->resize(photoLabel->sizeHint());
    qDebug()<<photoLabel->size();
    scrollArea->resize(photoLabel->sizeHint());
    this->resize(photoLabel->sizeHint());

    scrollArea->setWidget(photoLabel);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(scrollArea);
    if(yesEdit && !readOnly){
        mainLayout->addWidget(pageLabel);
        mainLayout->addWidget(pageEdit);
        mainLayout->addWidget(cancelButton);
        mainLayout->addWidget(saveButton);
    }
    setLayout(mainLayout);

    setWindowTitle(tr("Page Of Journal %1").arg(page));
    exchangeFile.setFileName("exchange.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }

    createContextMenu();
    scaledFactor = 1.0;
    photoLabel->setScaledContents(true);
}


void PhotoForm::mousePressEvent(QMouseEvent *mouseEvent)
{
    if(!yesEdit){
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
            QImage re;
            while(query.next()){
                QByteArray imageByte = query.value(0).toByteArray();
                QImage pixMap;
                pixMap.loadFromData(imageByte);
                int widI = QPixmap::fromImage(pixMap).width();
                int hei = QApplication::desktop()->height()-100;
                re = pixMap.scaled(widI,hei,Qt::KeepAspectRatio,Qt::SmoothTransformation);
                photoLabel->setPixmap(QPixmap::fromImage(re));
                photoLabel->resize(photoLabel->sizeHint());
                scrollArea->resize(photoLabel->sizeHint());
                this->resize(photoLabel->sizeHint());
            }
        }
    }
}

void PhotoForm::maxCount()
{
    QSqlQuery query;
    query.prepare("SELECT count(*) FROM journalphoto WHERE journalid = :id");
    query.bindValue(":id",journalID);
    query.exec();
    query.next();
    pageMax = query.value(0).toInt();
}

void PhotoForm::editRecord()
{
    pageNumber = pageEdit->text().toInt();
    QSqlQuery queryPh;
    queryPh.prepare("UPDATE journalphoto SET page = :page WHERE journalid = :journalid");
    queryPh.bindValue(":page",pageNumber);
    queryPh.bindValue(":journalid",journalID);
    queryPh.exec();
    queryPh.next();
    close();
}

void PhotoForm::createContextMenu()
{
    editTagForm = new QAction(tr("Edit Tegs For Search"),this);
    QPixmap pixD(":/edit.png");
    editTagForm->setIcon(pixD);
    connect(editTagForm,SIGNAL(triggered()),this,SLOT(addTag()));

    zoomIn = new QAction(tr("Zoom In"),this);
    QPixmap pixIn(":/up.png");
    zoomIn->setIcon(pixIn);
    connect(zoomIn,SIGNAL(triggered()),this,SLOT(upPictureSlot()));

    zoomOut = new QAction(tr("Zoom Out"),this);
    QPixmap pixOut(":/down.png");
    zoomOut->setIcon(pixOut);
    connect(zoomOut,SIGNAL(triggered()),this,SLOT(downPictureSlot()));

    photoLabel->addAction(zoomIn);
    photoLabel->addAction(editTagForm);
    photoLabel->addAction(zoomOut);
    photoLabel->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void PhotoForm::addTag()
{
    QSqlQuery queryPh;
    queryPh.prepare("SELECT journalphotoid FROM journalphoto WHERE (journalid = :journalid AND page = :page)");
    //"SELECT journalphoto, journalphotoid FROM journalphoto WHERE (journalid = :journalid AND page = :page)
    queryPh.bindValue(":page",pageNumber);
    queryPh.bindValue(":journalid",journalID);
    queryPh.exec();
    queryPh.next();

    QSqlQuery query;
    query.prepare("SELECT tegtableid FROM tegtable WHERE journalphotoid = :journalphotoid");
    query.bindValue(":journalphotoid",indexTemp);
    query.exec();
    query.next();
    TegTableForm openForm(query.value(0).toString(),queryPh.value(0).toString(),this,false);
    openForm.exec();
}

void PhotoForm::scaleImage(double factor)
{
    scaledFactor *= factor;
    photoLabel->resize(scaledFactor * photoLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomIn->setEnabled(scaledFactor < 3.0);
    zoomOut->setEnabled(scaledFactor > 0.333);
}

void PhotoForm::adjustScrollBar(QScrollBar *scrollArea, double factor)
{
    scrollArea->setValue(int(factor * scrollArea->value()
                                 + ((factor - 1) * scrollArea->pageStep()/2)));
}

void PhotoForm::upPictureSlot()
{
    scaleImage(1.25);
}

void PhotoForm::downPictureSlot()
{
    scaleImage(0.8);
}

void PhotoForm::wheelEvent(QWheelEvent *ev)
{
    qDebug()<<ev;
}
