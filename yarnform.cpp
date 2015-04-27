#include "yarnform.h"
#include <QtSql>
#include <QtNetwork/QTcpSocket>



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
    photoLabel->setToolTip("No Photo");
    photoLabel->setText("No Photo");
    photoLabel->setMinimumSize(100,200);
    photoLabel->setFrameShape(QFrame::NoFrame);
    photoLabel->setCursor(Qt::PointingHandCursor);
    photoLabel->setAlignment(Qt::AlignCenter);
    photoLabel->setStyleSheet("QLabel:hover {background-color: "
                              "qlineargradient(x1:0,y1:0,x2:0,y2:1,"
                              "stop:0 #cfcccc,"
                              "stop:0.5 #333232,"
                              "stop:0.51 #000000,"
                              "stop:1 #585c5f);"
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
    colorComplieter->setCaseSensitivity(Qt::CaseSensitive);
    editColor->setCompleter(colorComplieter);
    QToolButton *addColorButton = new QToolButton;
    QPixmap addPix(":/add.png");
    addColorButton->setIcon(addPix);
    addColorButton->setToolTip(tr("Add new Color"));
    addColorButton->setStyleSheet(styleToolButton);
    QToolButton *seeColorButton = new QToolButton;
    QPixmap seePix(":/see.png");
    seeColorButton->setIcon(seePix);
    seeColorButton->setToolTip(tr("See this Color"));
    seeColorButton->setStyleSheet(styleToolButton);
    QToolButton *listColorButton = new QToolButton;
    QPixmap listPix(":/list.png");
    listColorButton->setIcon(listPix);
    listColorButton->setToolTip(tr("List of Colors"));
    listColorButton->setStyleSheet(styleToolButton);
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
    structureComplieter->setCaseSensitivity(Qt::CaseSensitive);
    editStructure->setCompleter(structureComplieter);
    QToolButton *addStructureButton = new QToolButton;
    addStructureButton->setIcon(addPix);
    addStructureButton->setToolTip(tr("Add new Structure"));
    addStructureButton->setStyleSheet(styleToolButton);
    QToolButton *seeStructureButton = new QToolButton;
    seeStructureButton->setIcon(seePix);
    seeStructureButton->setToolTip(tr("See this Structure"));
    seeStructureButton->setStyleSheet(styleToolButton);
    QToolButton *listStructureButton = new QToolButton;
    listStructureButton->setIcon(listPix);
    listStructureButton->setToolTip(tr("List of Structures"));
    listStructureButton->setStyleSheet(styleToolButton);
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
    firmaComplieter->setCaseSensitivity(Qt::CaseSensitive);
    editFirma->setCompleter(firmaComplieter);
    QToolButton *addFirmaButton = new QToolButton;
    addFirmaButton->setIcon(addPix);
    addFirmaButton->setToolTip(tr("Add new Firma"));
    addFirmaButton->setStyleSheet(styleToolButton);
    QToolButton *seeFirmaButton = new QToolButton;
    seeFirmaButton->setIcon(seePix);
    seeFirmaButton->setToolTip(tr("See this Firma"));
    seeFirmaButton->setStyleSheet(styleToolButton);
    QToolButton *listFirmaButton = new QToolButton;
    listFirmaButton->setIcon(listPix);
    listFirmaButton->setToolTip(tr("List of Firms"));
    listFirmaButton->setStyleSheet(styleToolButton);
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
    QRegExp regExpTh("[x * 0-9 ]{9}");
    editThickness->setValidator(new QRegExpValidator(regExpTh,this));
    labelThickness->setBuddy(editThickness);

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
    mainLayout->addWidget(photoLabel,0,0,8,1);
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
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,7,2);
        editName->selectAll();
    }

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT * FROM yarn WHERE yarnid = ?");
        query.addBindValue(indexTemp);
        query.exec();
        while(query.next()){
            editName->setText(query.value(1).toString());
            editColor->setText(query.value(2).toString());
            editLenght->setText(query.value(3).toString());
            editWeight->setText(query.value(4).toString());
            editStructure->setText(query.value(5).toString());
            editFirma->setText(query.value(6).toString());
            editThickness->setText(query.value(7).toString());
            QSqlQuery queryPhoto;
            queryPhoto.prepare("SELECT photoname FROM photo WHERE photoid = :id");
            queryPhoto.bindValue(":id",indexTemp);
            queryPhoto.exec();
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

}

void YarnForm::editRecord()
{

}

void YarnForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Yarn");
    restoreGeometry(settings.value("Yarn").toByteArray());
}

void YarnForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Yarn");
    restoreGeometry(settings.value("Yarn").toByteArray());
}

void YarnForm::addColorRecord()
{

}

void YarnForm::seeColorRecord()
{

}

void YarnForm::listColorRecord()
{

}

void YarnForm::addStructureRecord()
{

}

void YarnForm::seeStructureRecord()
{

}

void YarnForm::listStructureRecord()
{

}

void YarnForm::addFirmaRecord()
{

}

void YarnForm::seeFirmaRecord()
{

}

void YarnForm::listFirmaRecord()
{

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
