#include "yarnform.h"
#include <QtSql>
#include "colorform.h"
#include "structureform.h"
#include "firmaform.h"
#include "viewlisttable.h"
#include "numprefix.h"
#include "unitform.h"

YarnForm::YarnForm(QString iD, QWidget *parent, bool onlyForRead)
    : QDialog(parent)
{
    readSettings();
    indexTemp = iD;
//    QFile file("ToolButtonStyle.txt");
//    file.open(QIODevice::ReadOnly);  QLatin1String(file.readAll());
    QString styleToolButton = "QToolButton {"
            "border: 1px solid black;"
            "max-height: 18px;"
            "max-width: 18px;"
            "min-height: 18px;"
            "min-width: 18px;"
            "padding: 1px;"
            "border-top-right-radius: 5px;"
            "border-top-left-radius: 5px;"
            "border-bottom-right-radius: 5px;"
            "border-bottom-left-radius: 5px;"
            "background-color: "
            "qlineargradient(x1:0, y1:0, x2:0, y2:1,"
            "stop:0 #d3d3d3,"
            "stop:0.5 #bebebe,"
            "stop:0.51 #bebebe,"
            "stop:1 #848484);"
            "color: #231A4C;"
            "font: bold}"

            "QToolButton:hover {background-color: "
            "qlineargradient(x1:0, y1:0, x2:0, y2:1,"
            "stop:0 #cfcccc,"
            "stop:0.5 #333232,"
            "stop:0.51 #000000,"
            "stop:1 #585c5f);"
            "color: #00cc00}";

    photoLabel = new PhotoLabel;

    photoLabel->setText("No Photo");
    photoLabel->setMinimumSize(100,200);
    photoLabel->setFrameShape(QFrame::NoFrame);
    photoLabel->setCursor(Qt::PointingHandCursor);
    photoLabel->setAlignment(Qt::AlignCenter);
    photoLabel->setStyleSheet("QLabel:hover {background-color: "
                              "qlineargradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,"
                              "stop:0 #cfcccc,"
                              "stop:1 #ffffff);"
                              "color: #00cc00;"
                              "font: bold;}");

    connect(photoLabel,SIGNAL(clicked()),this,SLOT(photoRead()));

    labelName = new QLabel(tr("Name:"));
    editName = new LineEdit;
    editName->setReadOnly(onlyForRead);
    QRegExp regExp("[\\x0410-\\x044f 0-9 \" -]{150}");
    editName->setValidator(new QRegExpValidator(regExp,this));
    labelName->setBuddy(editName);

    labelColor = new QLabel(tr("Color:"));
    editColor = new LineEdit;
    editColor->setValidator(new QRegExpValidator(regExp,this));
    QSqlQueryModel *colorModel = new QSqlQueryModel;
    colorModel->setQuery("SELECT colorname FROM color");
    QCompleter *colorComplieter = new QCompleter(colorModel);
    colorComplieter->setCompletionMode(QCompleter::InlineCompletion);
    colorComplieter->setCompletionMode(QCompleter::PopupCompletion);
    colorComplieter->setCaseSensitivity(Qt::CaseInsensitive);
    editColor->setCompleter(colorComplieter);
    QToolButton *addColorButton = new QToolButton;
    QPixmap addPix(":/add.png");
    addColorButton->setIcon(addPix);
    addColorButton->setToolTip(tr("Add new Color"));
    addColorButton->setStyleSheet(styleToolButton);
    connect(addColorButton,SIGNAL(clicked()),this,SLOT(addColorRecord()));
    QToolButton *seeColorButton = new QToolButton;
    QPixmap seePix(":/see.png");
    seeColorButton->setIcon(seePix);
    seeColorButton->setToolTip(tr("See this Color"));
    seeColorButton->setStyleSheet(styleToolButton);
    connect(seeColorButton,SIGNAL(clicked()),this,SLOT(seeColorRecord()));
    QToolButton *listColorButton = new QToolButton;
    QPixmap listPix(":/list.png");
    listColorButton->setIcon(listPix);
    listColorButton->setToolTip(tr("List of Colors"));
    listColorButton->setStyleSheet(styleToolButton);
    connect(listColorButton,SIGNAL(clicked()),this,SLOT(listColorRecord()));
    QHBoxLayout *colorLayout = new QHBoxLayout;
    //colorLayout->addWidget(labelColor);
    colorLayout->addWidget(editColor);
    colorLayout->addWidget(addColorButton);
    colorLayout->addWidget(seeColorButton);
    colorLayout->addWidget(listColorButton);
    colorLayout->addStretch();

    labelLenght = new QLabel(tr("Length:"));
    editLenght = new LineEdit;
    editLenght->setReadOnly(onlyForRead);
    QRegExp regExpNum("[0-9]{3}");
    editLenght->setValidator(new QRegExpValidator(regExpNum,this));
    labelLenght->setBuddy(editLenght);

    labelWeight = new QLabel(tr("Weight:"));
    editWeight = new LineEdit;
    editWeight->setReadOnly(onlyForRead);
    editWeight->setValidator(new QRegExpValidator(regExpNum,this));
    labelWeight->setBuddy(editWeight);

    labelStructure = new QLabel(tr("Structure:"));
    editStructure = new LineEdit;
    editStructure->setValidator(new QRegExpValidator(regExp,this));
    QSqlQueryModel *structureModel = new QSqlQueryModel;
    structureModel->setQuery("SELECT structurename FROM structure");
    QCompleter *structureComplieter = new QCompleter(structureModel);
    structureComplieter->setCompletionMode(QCompleter::InlineCompletion);
    structureComplieter->setCompletionMode(QCompleter::PopupCompletion);
    structureComplieter->setCaseSensitivity(Qt::CaseInsensitive);
    editStructure->setCompleter(structureComplieter);
    QToolButton *addStructureButton = new QToolButton;
    addStructureButton->setIcon(addPix);
    addStructureButton->setToolTip(tr("Add new Structure"));
    addStructureButton->setStyleSheet(styleToolButton);
    connect(addStructureButton,SIGNAL(clicked()),this,SLOT(addStructureRecord()));
    QToolButton *seeStructureButton = new QToolButton;
    seeStructureButton->setIcon(seePix);
    seeStructureButton->setToolTip(tr("See this Structure"));
    seeStructureButton->setStyleSheet(styleToolButton);
    connect(seeStructureButton,SIGNAL(clicked()),this,SLOT(seeStructureRecord()));
    QToolButton *listStructureButton = new QToolButton;
    listStructureButton->setIcon(listPix);
    listStructureButton->setToolTip(tr("List of Structures"));
    listStructureButton->setStyleSheet(styleToolButton);
    connect(listStructureButton,SIGNAL(clicked()),this,SLOT(listStructureRecord()));
    QHBoxLayout *structureLayout = new QHBoxLayout;
    //structureLayout->addWidget(labelStructure);
    structureLayout->addWidget(editStructure);
    structureLayout->addWidget(addStructureButton);
    structureLayout->addWidget(seeStructureButton);
    structureLayout->addWidget(listStructureButton);
    structureLayout->addStretch();

    labelFirma = new QLabel(tr("Firma:"));
    editFirma = new LineEdit;
    editFirma->setValidator(new QRegExpValidator(regExp,this));
    QSqlQueryModel *firmaModel = new QSqlQueryModel;
    firmaModel->setQuery("SELECT firmaname FROM firma");
    QCompleter *firmaComplieter = new QCompleter(firmaModel);
    firmaComplieter->setCompletionMode(QCompleter::InlineCompletion);
    firmaComplieter->setCompletionMode(QCompleter::PopupCompletion);
    firmaComplieter->setCaseSensitivity(Qt::CaseInsensitive);
    editFirma->setCompleter(firmaComplieter);
    QToolButton *addFirmaButton = new QToolButton;
    addFirmaButton->setIcon(addPix);
    addFirmaButton->setToolTip(tr("Add new Firma"));
    addFirmaButton->setStyleSheet(styleToolButton);
    connect(addFirmaButton,SIGNAL(clicked()),this,SLOT(addFirmaRecord()));
    QToolButton *seeFirmaButton = new QToolButton;
    seeFirmaButton->setIcon(seePix);
    seeFirmaButton->setToolTip(tr("See this Firma"));
    seeFirmaButton->setStyleSheet(styleToolButton);
    connect(seeFirmaButton,SIGNAL(clicked()),this,SLOT(seeFirmaRecord()));
    QToolButton *listFirmaButton = new QToolButton;
    listFirmaButton->setIcon(listPix);
    listFirmaButton->setToolTip(tr("List of Firms"));
    listFirmaButton->setStyleSheet(styleToolButton);
    connect(listFirmaButton,SIGNAL(clicked()),this,SLOT(listFirmaRecord()));
    QHBoxLayout *firmaLayout = new QHBoxLayout;
   // firmaLayout->addWidget(labelFirma);
    firmaLayout->addWidget(editFirma);
    firmaLayout->addWidget(addFirmaButton);
    firmaLayout->addWidget(seeFirmaButton);
    firmaLayout->addWidget(listFirmaButton);
    firmaLayout->addStretch();

    labelThickness = new QLabel(tr("Thickness:"));
    editThickness = new LineEdit;
    editThickness->setReadOnly(onlyForRead);
    QRegExp regExpTh("[x * 0-9 /]{9}");
    editThickness->setValidator(new QRegExpValidator(regExpTh,this));
    labelThickness->setBuddy(editThickness);

    labelUnit = new QLabel(tr("Unit:"));
    editUnit = new LineEdit;
    editUnit->setValidator(new QRegExpValidator(regExp,this));
    QSqlQueryModel *unitModel = new QSqlQueryModel;
    unitModel->setQuery("SELECT unitname FROM unit");
    QCompleter *unitComplieter = new QCompleter(unitModel);
    unitComplieter->setCompletionMode(QCompleter::InlineCompletion);
    unitComplieter->setCompletionMode(QCompleter::PopupCompletion);
    unitComplieter->setCaseSensitivity(Qt::CaseInsensitive);
    editUnit->setCompleter(unitComplieter);
    QToolButton *addUnitButton = new QToolButton;
    addUnitButton->setIcon(addPix);
    addUnitButton->setToolTip(tr("Add new Unit"));
    addUnitButton->setStyleSheet(styleToolButton);
    connect(addUnitButton,SIGNAL(clicked()),this,SLOT(addUnitRecord()));
    QToolButton *seeUnitButton = new QToolButton;
    seeUnitButton->setIcon(seePix);
    seeUnitButton->setToolTip(tr("See this Unit"));
    seeUnitButton->setStyleSheet(styleToolButton);
    connect(seeUnitButton,SIGNAL(clicked()),this,SLOT(seeUnitRecord()));
    QToolButton *listUnitButton = new QToolButton;
    listUnitButton->setIcon(listPix);
    listUnitButton->setToolTip(tr("List of Units"));
    listUnitButton->setStyleSheet(styleToolButton);
    connect(listUnitButton,SIGNAL(clicked()),this,SLOT(listUnitRecord()));
    QHBoxLayout *unitLayout = new QHBoxLayout;
   // firmaLayout->addWidget(labelFirma);
    unitLayout->addWidget(editUnit);
    unitLayout->addWidget(addUnitButton);
    unitLayout->addWidget(seeUnitButton);
    unitLayout->addWidget(listUnitButton);
    unitLayout->addStretch();

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

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(photoLabel,0,0,9,1);
    mainLayout->addWidget(labelName,0,1);
    mainLayout->addWidget(editName,0,2);
    mainLayout->addWidget(labelColor,1,1);
    mainLayout->addLayout(colorLayout,1,2,1,2);
    mainLayout->addWidget(labelLenght,2,1);
    mainLayout->addWidget(editLenght,2,2);
    mainLayout->addWidget(labelWeight,3,1);
    mainLayout->addWidget(editWeight,3,2);
    mainLayout->addWidget(labelStructure,4,1);
    mainLayout->addLayout(structureLayout,4,2,1,2);
    mainLayout->addWidget(labelFirma,5,1);
    mainLayout->addLayout(firmaLayout,5,2,1,2);
    mainLayout->addWidget(labelThickness,6,1);
    mainLayout->addWidget(editThickness,6,2);
    mainLayout->addWidget(labelUnit,7,1);
    mainLayout->addLayout(unitLayout,7,2);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,8,2);
        editName->selectAll();
    }

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT * FROM yarn WHERE yarnid = ?");
        query.addBindValue(indexTemp);
        query.exec();
        while(query.next()){
            editName->setText(query.value(1).toString());
            QSqlQuery queryCol;
            queryCol.prepare("SELECT colorname FROM color WHERE colorid = :colorid");
            queryCol.bindValue(":colorid",query.value(2).toString());
            queryCol.exec();
            queryCol.next();
            editColor->setText(queryCol.value(0).toString());
            editLenght->setText(query.value(3).toString());
            editWeight->setText(query.value(4).toString());
            QSqlQuery querySt;
            querySt.prepare("SELECT structurename FROM structure WHERE structureid = :structureid");
            querySt.bindValue(":structureid",query.value(5).toString());
            querySt.exec();
            querySt.next();
            editStructure->setText(querySt.value(0).toString());
            QSqlQuery queryF;
            queryF.prepare("SELECT firmaname FROM firma WHERE firmaid = :firmaid");
            queryF.bindValue(":firmaid",query.value(6).toString());
            queryF.exec();
            queryF.next();
            editFirma->setText(queryF.value(0).toString());
            editThickness->setText(query.value(7).toString());
            QSqlQuery queryU;
            queryU.prepare("SELECT unitname FROM unit WHERE unitid = :unitid");
            queryU.bindValue(":unitid",query.value(8).toString());
            queryU.exec();
            queryU.next();
            editUnit->setText(queryU.value(0).toString());
            QSqlQuery queryPhoto;
            queryPhoto.prepare("SELECT photoname FROM photo WHERE photoid = :id");
            queryPhoto.bindValue(":id",indexTemp);
            queryPhoto.exec();
            if(!queryPhoto.isActive()){
                photoLabel->setToolTip("No Photo");
            }else{
                photoLabel->setToolTip("Photo Of Yarn");
            }
            while(queryPhoto.next()){
                QByteArray imageByte = queryPhoto.value(0).toByteArray();
                QImage pixMap;
                pixMap.loadFromData(imageByte);
                QImage re = pixMap.scaled(100,200,Qt::KeepAspectRatio,Qt::SmoothTransformation);
                photoLabel->setPixmap(QPixmap::fromImage(re));
                QSettings settings("AO_Batrakov_Inc.", "Yarn");
                settings.setValue("indexPhoto",indexTemp);
            }
        }
    }

    setLayout(mainLayout);

    setWindowTitle(tr("Yarn"));
}

void YarnForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void YarnForm::deleteRecord()
{
    QSqlQuery query;
    query.prepare("DELETE FROM yarn WHERE yarnid = :id");
    query.bindValue(":id",indexTemp);
    query.exec();
    query.next();
}

void YarnForm::editRecord()
{
    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("UPDATE yarn SET "
                      "yarnname = :name, "
                      "colorid = (SELECT colorid FROM color WHERE colorname = :colorname), "
                      "lenght = :lenght, "
                      "weight = :weight, "
                      "structureid = (SELECT structureid FROM structure WHERE structurename = :structurename), "
                      "firmaid = (SELECT firmaid FROM firma WHERE firmaname = :firmaname), "
                      "thickness = :thickness, "
                      "unitid = (SELECT unitid FROM unit WHERE unitname = :unitname) "
                      "WHERE yarnid = :id");
        query.bindValue(":name",editName->text());
        query.bindValue(":colorname",editColor->text());
        query.bindValue(":lenght",editLenght->text().toInt());
        query.bindValue(":weight",editWeight->text().toInt());
        query.bindValue(":structurename",editStructure->text());
        query.bindValue(":firmaname",editFirma->text());
        query.bindValue(":thickness",editThickness->text());
        query.bindValue(":unitname",editUnit->text());
        query.bindValue(":id",indexTemp);
        query.exec();
    }else{
        QSqlQuery query;
        query.prepare("SELECT * FROM yarn WHERE yarnname = :name");
        query.bindValue(":name",editName->text().simplified());
        query.exec();
        query.next();
        if(!query.isValid()){
            NumPrefix numPrefix(this);
            indexTemp = numPrefix.getPrefix("yarn");
            qDebug()<<indexTemp;
            if(indexTemp == ""){
                close();
            }else{
                QSqlQuery query;
                query.prepare("INSERT INTO yarn (yarnid, "
                              "yarnname, "
                              "colorid, "
                              "lenght, "
                              "weight, "
                              "structureid, "
                              "firmaid, "
                              "thickness, "
                              "unitid) "
                              " VALUES(:id, "
                              ":name, "
                              "(SELECT colorid FROM color WHERE colorname = :colorname), "
                              ":lenght, "
                              ":weight, "
                              "(SELECT structureid FROM structure WHERE structurename = :structurename), "
                              "(SELECT firmaid FROM firma WHERE firmaname = :firmaname), "
                              ":thickness, "
                              "(SELECT unitid FROM unit WHERE unitname = :unitname))");
                query.bindValue(":id",indexTemp);
                query.bindValue(":name",editName->text().simplified());
                query.bindValue(":colorname",editColor->text());
                query.bindValue(":lenght",editLenght->text().toInt());
                query.bindValue(":weight",editWeight->text().toInt());
                query.bindValue(":structurename",editStructure->text());
                query.bindValue(":firmaname",editFirma->text());
                query.bindValue(":thickness",editThickness->text());
                query.bindValue(":unitname",editUnit->text());
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


void YarnForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Yarn");
    restoreGeometry(settings.value("YarnForm").toByteArray());
}

void YarnForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Yarn");
    settings.setValue("YarnForm", saveGeometry());
}

void YarnForm::addColorRecord()
{
    ColorForm formOpen("",this,false);
    formOpen.exec();
    if(formOpen.rowOut() != ""){
        QSqlQuery query;
        query.prepare("SELECT colorname FROM color WHERE "
                      "colorid = :id");
        query.bindValue(":id",formOpen.rowOut());
        query.exec();
        query.next();
        editColor->setText(query.value(0).toString());
    }
}

void YarnForm::seeColorRecord()
{
    QSqlQuery query;
    query.prepare("SELECT colorid FROM color WHERE colorname = :name");
    query.bindValue(":name",editColor->text());
    query.exec();
    while(query.next()){
        ColorForm formOpen(query.value(0).toString(),this,true);
        formOpen.exec();
    }
}

void YarnForm::listColorRecord()
{
    QSqlQuery query;
    query.prepare("SELECT colorid FROM color WHERE colorname = :name");
    query.bindValue(":name",editColor->text());
    query.exec();
    query.next();
    ViewListTable listOpen(query.value(0).toString(),"color",this);
    listOpen.exec();
    if(listOpen.rowOut() != ""){
        QSqlQuery queryL;
        queryL.prepare("SELECT colorname FROM color WHERE "
                      "colorid = :id");
        queryL.bindValue(":id",listOpen.rowOut());
        queryL.exec();
        queryL.next();
        editColor->setText(queryL.value(0).toString());
    }
}

void YarnForm::addStructureRecord()
{
    StructureForm formOpen("",this,false);
    formOpen.exec();
    if(formOpen.rowOut() != ""){
        QSqlQuery query;
        query.prepare("SELECT structurename FROM structure WHERE "
                      "structureid = :id");
        query.bindValue(":id",formOpen.rowOut());
        query.exec();
        query.next();
        editStructure->setText(query.value(0).toString());
    }
}

void YarnForm::seeStructureRecord()
{
    QSqlQuery query;
    query.prepare("SELECT structureid FROM structure WHERE structurename = :name");
    query.bindValue(":name",editStructure->text());
    query.exec();
    while(query.next()){
        StructureForm formOpen(query.value(0).toString(),this,true);
        formOpen.exec();
    }
}

void YarnForm::listStructureRecord()
{
    QSqlQuery query;
    query.prepare("SELECT structureid FROM structure WHERE structurename = :name");
    query.bindValue(":name",editStructure->text());
    query.exec();
    query.next();
    ViewListTable listOpen(query.value(0).toString(),"structure",this);
    listOpen.exec();
    if(listOpen.rowOut() != ""){
        QSqlQuery queryL;
        queryL.prepare("SELECT structurename FROM structure WHERE "
                      "structureid = :id");
        queryL.bindValue(":id",listOpen.rowOut());
        queryL.exec();
        queryL.next();
        editStructure->setText(queryL.value(0).toString());
    }
}

void YarnForm::addFirmaRecord()
{
    FirmaForm formOpen("",this,false);
    formOpen.exec();
    if(formOpen.rowOut() != ""){
        QSqlQuery query;
        query.prepare("SELECT firmaname FROM firma WHERE "
                      "firmaid = :id");
        query.bindValue(":id",formOpen.rowOut());
        query.exec();
        query.next();
        editFirma->setText(query.value(0).toString());
    }
}

void YarnForm::seeFirmaRecord()
{
    QSqlQuery query;
    query.prepare("SELECT firmaid FROM firma WHERE firmaname = :name");
    query.bindValue(":name",editFirma->text());
    query.exec();
    while(query.next()){
        FirmaForm formOpen(query.value(0).toString(),this,true);
        formOpen.exec();
    }
}

void YarnForm::listFirmaRecord()
{
    QSqlQuery query;
    query.prepare("SELECT firmaid FROM firma WHERE firmaname = :name");
    query.bindValue(":name",editFirma->text());
    query.exec();
    query.next();
    ViewListTable listOpen(query.value(0).toString(),"firma",this);
    listOpen.exec();
    if(listOpen.rowOut() != ""){
        QSqlQuery queryL;
        queryL.prepare("SELECT firmaname FROM firma WHERE "
                      "firmaid = :id");
        queryL.bindValue(":id",listOpen.rowOut());
        queryL.exec();
        queryL.next();
        editFirma->setText(queryL.value(0).toString());
    }
}

void YarnForm::addUnitRecord()
{
    UnitForm formOpen("",this,false);
    formOpen.exec();
    if(formOpen.rowOut() != ""){
        QSqlQuery query;
        query.prepare("SELECT unitname FROM unit WHERE "
                      "unitid = :id");
        query.bindValue(":id",formOpen.rowOut());
        query.exec();
        query.next();
        editUnit->setText(query.value(0).toString());
    }
}

void YarnForm::seeUnitRecord()
{
    QSqlQuery query;
    query.prepare("SELECT unitid FROM unit WHERE unitname = :name");
    query.bindValue(":name",editUnit->text());
    query.exec();
    while(query.next()){
        UnitForm formOpen(query.value(0).toString(),this,true);
        formOpen.exec();
    }
}

void YarnForm::listUnitRecord()
{
    QSqlQuery query;
    query.prepare("SELECT unitid FROM unit WHERE unitname = :name");
    query.bindValue(":name",editUnit->text());
    query.exec();
    query.next();
    ViewListTable listOpen(query.value(0).toString(),"unit",this);
    listOpen.exec();
    if(listOpen.rowOut() != ""){
        QSqlQuery queryL;
        queryL.prepare("SELECT unitname FROM unit WHERE "
                      "unitid = :id");
        queryL.bindValue(":id",listOpen.rowOut());
        queryL.exec();
        queryL.next();
        editUnit->setText(queryL.value(0).toString());
    }
}

void YarnForm::photoRead()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"),"",tr("Images (*.jpg *.gif *.png)"));
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly)){
        if(file.size() > 600000){
            int que = QMessageBox::warning(this,tr("Attention!"),
                                 tr("File size - %1 ,\nAre You sure?").arg(QString::number(file.size())),
                                           QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
            if(que == QMessageBox::No){
                return;
            }
        }
        QImage pixMap;
        pixMap.load(fileName);
        QImage re = pixMap.scaled(100,200,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        photoLabel->setPixmap(QPixmap::fromImage(re));
        QSqlQuery queryPhotoControl;
        queryPhotoControl.prepare("SELECT photoid FROM photo WHERE photoid = :id");
        queryPhotoControl.bindValue(":id",indexTemp);
        queryPhotoControl.exec();
        queryPhotoControl.next();
       if(queryPhotoControl.value(0).toString() == ""){
            QSqlQuery queryPhoto;
            queryPhoto.prepare("INSERT INTO photo (photoid, photoname) VALUES("
                               ":photoid, :photoname)"
                               );
            queryPhoto.bindValue(":photoid",indexTemp);
            QByteArray imageByte = file.readAll();
            queryPhoto.bindValue(":photoname",imageByte);
            queryPhoto.exec();
        }else{
            QSqlQuery queryPhoto;
            queryPhoto.prepare("UPDATE photo SET photoname = :name "
                               "WHERE photoid = :id)");
            queryPhoto.bindValue(":id",indexTemp);
            QByteArray imageByte = file.readAll();
            queryPhoto.bindValue(":name",imageByte);
            queryPhoto.exec();
        }
    }
}
