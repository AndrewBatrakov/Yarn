#include "mainwindow.h"
#include "colorform.h"
#include "materialform.h"
#include "structureform.h"
#include "yarnform.h"
#include "putbase.h"
#include "unitform.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QPixmap pixLogo(":/logo.png");
    setWindowIcon(pixLogo);

    QWidget::setStyleSheet("MainWindow, QMessageBox, QDialog, QMenu, QAction, QMenuBar {background-color: "
                           "#DDD6FF}"
                           "QMenu {"
                           "font: bold}"
                           "QMenu::item:selected {background-color: "
                           "qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                           "stop:0 #cfcccc,"
                           "stop:0.5 #333232,"
                           "stop:0.51 #000000,"
                           "stop:1 #585c5f);"
                           "color: #00cc00}"

                           "QMenuBar::item:selected {background-color: "
                           "qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                           "stop:0 #cfcccc,"
                           "stop:0.5 #333232,"
                           "stop:0.51 #000000,"
                           "stop:1 #585c5f);"
                           "color: #00cc00}"

                           "QPushButton {"
                           "border: 1px solid black;"
                           "min-height: 20px;"
                           "min-width: 70px;"
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

                           "QPushButton:hover {background-color: "
                           "qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                           "stop:0 #cfcccc,"
                           "stop:0.5 #333232,"
                           "stop:0.51 #000000,"
                           "stop:1 #585c5f);"
                           "color: #00cc00}"

                           "QPushButton:focus {border-color: yellow}"
                           "LineEdit:hover {background-color: #FFFF99}"
                           "QComboBox:hover {background-color: #FFFF99}"
                           "QDateEdit:hover {background-color: #FFFF99}"
                           "LineEdit:focus {background-color: #FFFFCC}"
                           "QComboBox:focus {background-color: #FFFFCC}"
                           "QDateEdit:focus {background-color: #FFFFCC}"

                           "QProgressBar {"
                           "border: 1px solid black;"
                           "text-align: center;"
                           "color: #00B600;"
                           "font: bold;"
                           "padding: 1px;"
                           "border-top-right-radius: 5px;"
                           "border-top-left-radius: 5px;"
                           "border-bottom-right-radius: 5px;"
                           "border-bottom-left-radius: 5px;"
                           "background: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1,"
                           "stop: 0 #fff,"
                           "stop: 0.4999 #eee,"
                           "stop: 0.5 #ddd,"
                           "stop: 1 #eee );"
                           "width: 15px;"
                           "}"

                           "QProgressBar::chunk {"
                           "background: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1,"
                           "stop: 0 #78d,"
                           "stop: 0.4999 #46a,"
                           "stop: 0.5 #45a,"
                           "stop: 1 #238 );"
                           "border-top-right-radius: 5px;"
                           "border-top-left-radius: 5px;"
                           "border-bottom-right-radius: 5px;"
                           "border-bottom-left-radius: 5px;"
                           "border: 1px solid black;}"

                           "QTabWidget::pane {"
                           "border: 1px solid #A3A3FF;"
                           "border-top-right-radius: 5px;"
                           "border-top-left-radius: 5px;"
                           "border-bottom-right-radius: 5px;"
                           "border-bottom-left-radius: 5px;"
                           "top: -0.5em}"

                           "QTabWidget::tab-bar {"
                           "left: 5px;}"

                           "QTabBar::tab {background-color: "
                           "qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                           "stop:0 #DDD6FF,"
                           "stop:0.5 #A3A3FF,"
                           "stop:0.51 #A3A3FF,"
                           "stop:1 #DDD6FF);"
                           "border: 1px solid #A3A3FF;"
                           "padding: 1px;"
                           "min-width: 90px;"
                           "border-top-right-radius: 5px;"
                           "border-top-left-radius: 5px;"
                           "font: bold;}"

                           "QTabBar::tab:!selected {background-color: "
                           "#DDD6FF;"
                           "margin-top: 2px;"
                           "font: normal;}"

                           "QTabBar::tab:hover {background-color: "
                           "qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                           "stop:0 #cfcccc,"
                           "stop:0.5 #333232,"
                           "stop:0.51 #000000,"
                           "stop:1 #585c5f);"
                           "color: #00cc00;"
                           "font: bold;}"

                           "QDoubleSpinBox {padding-left: 15px; padding-right: 15px; border: 1px;}"

                           "QDoubleSpinBox::up-button {subcontrol-position: right; subcontrol-origin: border; "
                           "min-width: 15px; min-height: 15px;"
                           "background-color: "
                           "qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                           "stop:0 #d3d3d3,"
                           "stop:0.5 #bebebe,"
                           "stop:0.51 #bebebe,"
                           "stop:1 #848484);"
                           "color: #231A4C;"
                           "font: bold;}"

                           "QDoubleSpinBox::up-button:hover {background-color: "
                           "qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                           "stop:0 #cfcccc,"
                           "stop:0.5 #333232,"
                           "stop:0.51 #000000,"
                           "stop:1 #585c5f);"
                           "color: #00cc00;}"

                           "QDoubleSpinBox::up-arrow {width: 10px; height: 10px;}"
                           "QDoubleSpinBox::down-arrow {width: 10px; height: 10px;}"

                           "QDoubleSpinBox::up-arrow {image: url(:/upblack.png);}"

                           "QDoubleSpinBox::up-arrow:hover {image: url(:/upgreen.png);}"

                           "QDoubleSpinBox::down-arrow {image: url(:/downblack.png);}"

                           "QDoubleSpinBox::down-arrow:hover {image: url(:/downred.png);}"

                           "QDoubleSpinBox::down-button {subcontrol-position: left; subcontrol-origin: border;"
                           "min-width: 15px; min-height: 15px;"
                           "background-color: "
                           "qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                           "stop:0 #d3d3d3,"
                           "stop:0.5 #bebebe,"
                           "stop:0.51 #bebebe,"
                           "stop:1 #848484);"
                           "color: #231A4C;"
                           "font: bold;}"

                           "QDoubleSpinBox::down-button:hover {background-color: "
                           "qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                           "stop:0 #cfcccc,"
                           "stop:0.5 #333232,"
                           "stop:0.51 #000000,"
                           "stop:1 #585c5f);"
                           "color: #00cc00;}"

                           "QSpinBox {padding-left: 15px; padding-right: 15px; border: 1px;}"

                           "QSpinBox::up-button {subcontrol-position: right; subcontrol-origin: border; "
                           "min-width: 15px; min-height: 15px;"
                           "background-color: "
                           "qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                           "stop:0 #d3d3d3,"
                           "stop:0.5 #bebebe,"
                           "stop:0.51 #bebebe,"
                           "stop:1 #848484);"
                           "color: #231A4C;"
                           "font: bold;}"

                           "QSpinBox::up-button:hover {background-color: "
                           "qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                           "stop:0 #cfcccc,"
                           "stop:0.5 #333232,"
                           "stop:0.51 #000000,"
                           "stop:1 #585c5f);"
                           "color: #00cc00;}"

                           "QSpinBox::up-arrow {width: 10px; height: 10px;}"
                           "QSpinBox::down-arrow {width: 10px; height: 10px;}"

                           "QSpinBox::up-arrow {image: url(:/upblack.png);}"

                           "QSpinBox::up-arrow:hover {image: url(:/upgreen.png);}"

                           "QSpinBox::down-arrow {image: url(:/downblack.png);}"

                           "QSpinBox::down-arrow:hover {image: url(:/downred.png);}"

                           "QSpinBox::down-button {subcontrol-position: left; subcontrol-origin: border;"
                           "min-width: 15px; min-height: 15px;"
                           "background-color: "
                           "qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                           "stop:0 #d3d3d3,"
                           "stop:0.5 #bebebe,"
                           "stop:0.51 #bebebe,"
                           "stop:1 #848484);"
                           "color: #231A4C;"
                           "font: bold;}"

                           "QSpinBox::down-button:hover {background-color: "
                           "qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                           "stop:0 #cfcccc,"
                           "stop:0.5 #333232,"
                           "stop:0.51 #000000,"
                           "stop:1 #585c5f);"
                           "color: #00cc00;}"
                           );
    readSettings();
    createPanel();
    setFilter =false;
    createActions();
    createMenu();
    createContextMenu();

    viewYarn();
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
}

void MainWindow::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Yarn");
    restoreGeometry(settings.value("Geometry").toByteArray());
}

void MainWindow::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Yarn");
    settings.setValue("Geometry", saveGeometry());
    settings.remove("CurrentUser");
}

void MainWindow::createPanel()
{
    panel = new QWidget;
    tableLabel = new QLabel;
    tableView = new QTableView(this);
    templateModel = new QSqlRelationalTableModel(this);
    templateModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);

    addRecordButton = new QPushButton(tr("Add"));
    QPixmap pixAdd(":/add.png");
    addRecordButton->setIcon(pixAdd);
    connect(addRecordButton,SIGNAL(clicked()),this,SLOT(addRecordOfTable()));

    deleteRecordButton = new QPushButton(tr("Delete"));
    QPixmap pixDelete(":/delete.png");
    deleteRecordButton->setIcon(pixDelete);
    connect(deleteRecordButton,SIGNAL(clicked()),this,SLOT(deleteRecordOfTable()));

    editRecordButton = new QPushButton(tr("Edit"));
    QPixmap pixEdit(":/edit.png");
    editRecordButton->setIcon(pixEdit);
    connect(editRecordButton,SIGNAL(clicked()),this,SLOT(editRecordOfTable()));

    QHBoxLayout *buttonBox = new QHBoxLayout;

    buttonBox->addWidget(addRecordButton);
    buttonBox->addWidget(deleteRecordButton);
    buttonBox->addWidget(editRecordButton);
    buttonBox->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(buttonBox);
    mainLayout->addWidget(tableLabel);
    mainLayout->addWidget(tableView);
    panel->setLayout(mainLayout);
    connect(tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(editRecordOfTable()));

    splitterCreate();

}

void MainWindow::createActions()
{
    //File Action
    exitAction = new QAction(tr("Exit..."),this);
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));

    //References Action
    yarnAction = new QAction(tr("Yarn..."),this);
    connect(yarnAction,SIGNAL(triggered()),this,SLOT(viewYarn()));
    structureAction = new QAction(tr("Structure..."),this);
    connect(structureAction,SIGNAL(triggered()),this,SLOT(viewStructure()));
    materialAction = new QAction(tr("Material..."),this);
    connect(materialAction,SIGNAL(triggered()),this,SLOT(viewMaterial()));
    colorAction = new QAction(tr("Color..."),this);
    connect(colorAction,SIGNAL(triggered()),this,SLOT(viewColor()));
    unitAction = new QAction(tr("Unit..."),this);
    connect(unitAction,SIGNAL(triggered()),this,SLOT(viewUnit()));

    //Documents Action

    //Reports Action

    //Service Action
    //prefixAction = new QAction(tr("Prefix..."),this);
    //connect(prefixAction,SIGNAL(triggered()),this,SLOT(prefixFunction()));

    putAction = new QAction(tr("Put Base on FTP"),this);
    connect(putAction,SIGNAL(triggered()),this,SLOT(putBaseProcedure()));

    getAction = new QAction(tr("Get Base from FTP"),this);
    connect(getAction,SIGNAL(triggered()),this,SLOT(getBaseProcedure()));

    //Help Action
    aboutQtAction = new QAction(tr("About Qt..."),this);
    connect(aboutQtAction,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
    aboutProgAction = new QAction(tr("About programm..."),this);
    connect(aboutProgAction,SIGNAL(triggered()),this,SLOT(aboutProgramm()));
}

void MainWindow::createMenu()
{
    fileMenu = menuBar()->addMenu(tr("File"));
    menuBar()->setStyleSheet("font: bold; color: darkblue;");
    fileMenu->addAction(exitAction);

    referenceMenu = menuBar()->addMenu(tr("References"));
    referenceMenu->addAction(yarnAction);
    referenceMenu->addSeparator();
    referenceMenu->addAction(structureAction);
    referenceMenu->addAction(materialAction);
    referenceMenu->addAction(colorAction);
    referenceMenu->addAction(unitAction);

    //documentMenu = menuBar()->addMenu(tr("Documents"));
    //documentMenu->addSeparator();

    //reportMenu = menuBar()->addMenu(tr("Reports"));
    //reportMenu->addSeparator();

    serviceMenu = menuBar()->addMenu(tr("Service"));
    //serviceMenu->addAction(prefixAction);
    serviceMenu->addAction(putAction);
    serviceMenu->addAction(getAction);
    serviceMenu->addSeparator();

    menuBar()->addSeparator();
    helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(aboutQtAction);
    helpMenu->addAction(aboutProgAction);
}

void MainWindow::splitterCreate()
{
    splitter = new QSplitter(Qt::Horizontal);
    splitter->setFrameStyle(QFrame::StyledPanel);
    splitter->addWidget(panel);
    splitter->setStretchFactor(1,1);
    setCentralWidget(splitter);
}

void MainWindow::createContextMenu()
{
    addAction = new QAction(tr("Add Record"),this);
    QPixmap pixAdd(":/add.png");
    addAction->setIcon(pixAdd);
    connect(addAction,SIGNAL(triggered()),this,SLOT(addRecordOfTable()));

    QPixmap pixDelete(":/delete.png");
    deleteAction = new QAction(tr("Delete Record"),this);
    deleteAction->setIcon(pixDelete);
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(deleteRecordOfTable()));

    QPixmap pixEdit(":/edit.png");
    editAction = new QAction(tr("Edit Record"),this);
    editAction->setIcon(pixEdit);
    connect(editAction,SIGNAL(triggered()),this,SLOT(editRecordOfTable()));

    tableView->addAction(addAction);
    tableView->addAction(deleteAction);
    tableView->addAction(editAction);
    tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
}
void MainWindow::aboutProgramm()
{
    QSettings iniSettings("Yarn.ini",QSettings::IniFormat);
    QString version = iniSettings.value("Version").toString();
   //    int fontId = iniSettings.value("Font").toInt();

    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Yarn"));
    QPixmap pixLogo(":/logo.png");
    msgBox.setIconPixmap(pixLogo);
    QString textMessage = tr("<p style=\"color:darkblue\"; style=\"font-family:Times New Roman\"; "
                             "style=\"font-size :20pt\"; "
                             "style=\"font: bold\" >"
                             "Yarn "
                             "<br/> (SQLite version) ");
    textMessage += version;
    textMessage += tr("</p>"
                      "<p style=\"color:darkgreen\"; style=\"font-family:Times New Roman\"; "
                      "style=\"font-size :12pt\">Copyright 2011-15 A+O Batrakov Inc.</p>"
                      "<p style=\"font-size :12pt\">"
                      "The program helps to keep Yarn information.</p>"
                      "<p style=\"font-family:Times New Roman\"; style=\"font-size :10pt\">"
                      "An how to improve, extend the functionality of the program to address: "
                      "<a href=\"www.mail.ru\">hothit@list.ru</a>;</p>");
    msgBox.setText(textMessage);
    //msgBox.setFont(QFont(QFontDatabase::applicationFontFamilies(fontId).first()));
    msgBox.addButton(QMessageBox::Yes);
    msgBox.exec();
}

void MainWindow::editRecordOfTable()
{
    QString stringVar = templateModel->tableName();
    QModelIndex index = tableView->currentIndex();
    if(index.isValid()){
        QSqlRecord record =templateModel->record(index.row());
        if(stringVar == "color"){
            QString iD = record.value("colorid").toString();
            ColorForm form(iD, this, false);
            form.exec();
        }else if(stringVar == "material"){
            QString iD = record.value("materialid").toString();
            MaterialForm form(iD, this, false);
            form.exec();
        }else if(stringVar == "structure"){
            QString iD = record.value("structureid").toString();
            StructureForm form(iD, this, false);
            form.exec();
        }else if(stringVar == "yarn"){
            QString iD = record.value("yarnid").toString();
            YarnForm form(iD, this, false);
            form.exec();
        }else if(stringVar == "unit"){
            QString iD = record.value("unitid").toString();
            UnitForm form(iD, this, false);
            form.exec();
        }
    }
    QModelIndex modIndex = tableView->currentIndex();
    MainWindow::updatePanel(modIndex);
}

void MainWindow::addRecordOfTable()
{
    QSettings settings("AO_Batrakov_Inc.", "Yarn");
    QString userName = settings.value("CurrentUser").toString();
    QSqlQuery query;
    query.prepare("SELECT role FROM users WHERE userid = :id");
    query.bindValue(":id",userName);
    query.exec();
    query.next();
    //if(query.value(0).toInt() == 1){
        QString valueTemp = templateModel->tableName();
        if(valueTemp == "color"){
            ColorForm form("",this,false);
            form.exec();
        }else if(valueTemp == "material"){
            MaterialForm form("",this,false);
            form.exec();
        }else if(valueTemp == "structure"){
            StructureForm form("",this,false);
            form.exec();
        }else if(valueTemp == "yarn"){
            YarnForm form("",this,false);
            form.exec();
        }else if(valueTemp == "unit"){
            UnitForm form("",this,false);
            form.exec();
        }
//        if(tableView->currentIndex().isValid()){
//            qDebug()<<"index valid";
            QModelIndex modIndex = tableView->currentIndex();
            MainWindow::updatePanel(modIndex);
//        }else{
//            qDebug()<<"index not valid";
//        }
        //    }else{
        //        QMessageBox::warning(this,"Attention!!!","You don't add record, Seed Role!");
        //    }

}

void MainWindow::deleteRecordOfTable()
{
    QModelIndex index = tableView->currentIndex();
    if(index.isValid()){
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("Attention!!!"));
        msgBox.setText(tr("Really delete?"));

        msgBox.setIcon(QMessageBox::Question);
        msgBox.addButton(QMessageBox::Yes);
        QPushButton *noButton = msgBox.addButton(QMessageBox::No);

        msgBox.setDefaultButton(noButton);

        noButton->setStyleSheet("QPushButton:hover {color: red}");
        int k = msgBox.exec();
        if(k == QMessageBox::Yes){
            QString valueTemp, iDValue;
            valueTemp = templateModel->tableName();
            QModelIndex index = tableView->currentIndex();
            QSqlRecord record = templateModel->record(index.row());
            if(valueTemp == "color"){
                iDValue = record.value("colorid").toString();
                ColorForm form(iDValue,this,false);
                form.deleteRecord();
            }else if(valueTemp == "material"){
                iDValue = record.value("materialid").toString();
                MaterialForm form(iDValue,this,false);
                form.deleteRecord();
            }else if(valueTemp == "structure"){
                iDValue = record.value("structureid").toString();
                StructureForm form(iDValue,this,false);
                form.deleteRecord();
            }else if(valueTemp == "yarn"){
                iDValue = record.value("yarnid").toString();
                YarnForm form(iDValue,this,false);
                form.deleteRecord();
            }else if(valueTemp == "unit"){
                iDValue = record.value("unitid").toString();
                UnitForm form(iDValue,this,false);
                form.deleteRecord();
            }
        }
    }
    updatePanel(index);
}

void MainWindow::viewTemplateTable(QString tempTable)
{
    if(tableView->model())
    for(int i=0; i<tableView->model()->columnCount();i++){
        QAbstractItemDelegate* delegate = tableView->itemDelegateForColumn( i );
        //Remove the delegate from the view
        tableView->setItemDelegateForColumn( i, NULL );
        delete delegate;
    }
    tableView->clearSpans();
    templateModel->clear();
    templateModel->setTable(tempTable);

    QString strivgValue; bool delAll = false;
    if(tempTable == "color"){
        templateModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
        if(setFilter){
            templateModel->setFilter(QString("organizationname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = tr("Color");
    }else if(tempTable == "material"){
        templateModel->setHeaderData(1,Qt::Horizontal, tr("Material Name"));
        templateModel->setHeaderData(2,Qt::Horizontal, tr("Percent"));
        if(setFilter){
            templateModel->setFilter(QString("materialname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = tr("Material");
    }else if(tempTable == "structure"){
        templateModel->setHeaderData(1,Qt::Horizontal, tr("Structure Name"));
        if(setFilter){
            templateModel->setFilter(QString("structurename LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = tr("Structure");
    }else if(tempTable == "yarn"){
        templateModel->setHeaderData(1,Qt::Horizontal, tr("Yarn Name"));
        templateModel->setHeaderData(2,Qt::Horizontal, tr("Color"));
        templateModel->setHeaderData(3,Qt::Horizontal, tr("Lenght"));
        templateModel->setHeaderData(4,Qt::Horizontal, tr("Weight"));
        templateModel->setHeaderData(5,Qt::Horizontal, tr("Structure"));
        templateModel->setHeaderData(6,Qt::Horizontal, tr("Produser"));
        templateModel->setHeaderData(7,Qt::Horizontal, tr("Thickness"));
        templateModel->setHeaderData(8,Qt::Horizontal, tr("Unit"));
        if(setFilter){
            templateModel->setFilter(QString("yarnname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = tr("Yarn");
    }else if(tempTable == "unit"){
        templateModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
        if(setFilter){
            templateModel->setFilter(QString("unitname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = tr("Unit");
    }
    else{
        tableView->setModel(0);
        tableLabel->clear();
        delAll = true;
    }
    if(!delAll){
        templateModel->select();
//        while(templateModel->canFetchMore())
//            templateModel->fetchMore();
        QHeaderView *head = tableView->horizontalHeader();
        connect(head,SIGNAL(sectionClicked(int)),this,SLOT(sortTable(int)));
        tableView->setModel(templateModel);
        if(tempTable == "users" || tempTable == "trainingsafety" || tempTable == "contractdoc"){
            tableView->setColumnHidden(0, false);
        }else{
            tableView->setColumnHidden(0, true);
        }

        if(tempTable == "employee"){
            tableView->setColumnHidden(2,true);
            tableView->setColumnHidden(5,true);
            for(int i = 7; i < 15; ++i){
                tableView->setColumnHidden(i,true);
            }
        }

        tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        /*if(tempTable == "driver"){
        tableView->setItemDelegateForColumn(7, new DateItemDelegate);
    }
    if(tempTable == "waybill"){
        tableView->setItemDelegateForColumn(2, new DateItemDelegate);
    }*/
//        if(tempTable == "post"){
//            tableView->setItemDelegateForColumn(2, new BoolItemDelegate);
//        }
//        if(tempTable == "laterm"){
//            tableView->setItemDelegateForColumn(1, new BoldItemDelegate);
//        }

        tableView->setAlternatingRowColors(true);
        tableView->resizeColumnsToContents();
        tableView->horizontalHeader()->setResizeContentsPrecision(1000);
        head->setStretchLastSection(true);
        tableLabel->clear();
        tableLabel->setText(tr("Name of Table: %1").arg(strivgValue));
        tableLabel->setStyleSheet("font: bold; color: darkblue;");
        setFilter = false;
    }
}

void MainWindow::sortTable(int index)
{
    templateModel->setSort(index,Qt::AscendingOrder);
    templateModel->select();
    //while(templateModel->canFetchMore())
    //    templateModel->fetchMore();
}

void MainWindow::updatePanel(QModelIndex inDex)
{
    templateModel->select();
    tableView->resizeColumnsToContents();
    QHeaderView *head = tableView->horizontalHeader();
    head->setStretchLastSection(true);
    //while(templateModel->canFetchMore())
    //    templateModel->fetchMore();
    tableView->setCurrentIndex(inDex);
}

void MainWindow::viewColor()
{
    viewTemplateTable("color");
}

void MainWindow::viewMaterial()
{
    viewTemplateTable("material");
}

void MainWindow::viewStructure()
{
    viewTemplateTable("structure");
}

void MainWindow::viewYarn()
{
    viewTemplateTable("yarn");
}

void MainWindow::viewUnit()
{
    viewTemplateTable("unit");
}

void MainWindow::putBaseProcedure()
{
    PutBase put(this);
    put.putBase();
}

void MainWindow::getBaseProcedure()
{

}
