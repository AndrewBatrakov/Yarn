#include "viewlisttable.h"
#include "searchform.h"
#include "materialform.h"
#include "structureform.h"
#include "colorform.h"
#include "firmaform.h"
#include "tegform.h"

ViewListTable::ViewListTable(QString idTable, QString nameTable, QWidget *parent) : QDialog(parent)
{
    readSetting();
    tableName = nameTable;
    iDTemp = idTable;
    labelName = "";

    addButton = new QPushButton(tr("Add record"));
    QPixmap pixAdd(":/add.png");
    addButton->setIcon(pixAdd);
    connect(addButton,SIGNAL(clicked()),this,SLOT(addRecord()));

    deleteButton = new QPushButton(tr("Delete record"));
    QPixmap pixDel(":/delete.png");
    deleteButton->setIcon(pixDel);
    connect(deleteButton,SIGNAL(clicked()),this,SLOT(deleteRecord()));

    editButton = new QPushButton(tr("Edit record"));
    QPixmap pixEdit(":/edit.png");
    editButton->setIcon(pixEdit);
    connect(editButton,SIGNAL(clicked()),this,SLOT(editRecord()));

    searchButton = new QPushButton(tr("Search (by Name)"));
    connect(searchButton,SIGNAL(clicked()),this,SLOT(searchProcedure()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(searchButton);
    buttonLayout->addStretch();

    tableView = new QTableView;
    tableView->clearSpans();

    templateModel = new QSqlRelationalTableModel(this);

    viewTemplateTable(tableName);
    if(iDTemp != ""){
        for(int row = 0; row  < templateModel->rowCount(); ++row){
            QSqlRecord record = templateModel->record(row);
            QModelIndex index = templateModel->index(row,1);
            if(record.value(0).toString() == iDTemp){
                tableView->setCurrentIndex(index);
                break;
            }
        }
    }else{
        tableView->setCurrentIndex(templateModel->index(0,0));
    }

    tableView->resizeColumnsToContents();
    tableView->setAlternatingRowColors(true);
    QHeaderView *header = tableView->horizontalHeader();
    header->setStretchLastSection(true);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(tableView);
    setLayout(mainLayout);

    connect(tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(accept()));

    setWindowTitle(tr("List of %1").arg(labelName));
    resize(tableView->size().width()+100,tableView->size().height());

    createContextMenu();
}
void ViewListTable::done(int result)
{
    writeSetting();
    if(result == QDialog::Accepted){
        QModelIndex index = tableView->currentIndex();
        if(index.isValid()){
            QSqlRecord record = templateModel->record(index.row());
            QString per = tableName;
            per += "id";
            iDTemp = record.value(per).toString();
        }else{
            iDTemp = "";
        }
    }

    QDialog::done(result);
}

void ViewListTable::addRecord()
{
    QString nameList;
    if(tableName == "material"){
        MaterialForm listForm("",this,false);
        listForm.exec();
        nameList = listForm.rowOut();
    }else if(tableName == "color"){
        ColorForm listForm("",this,false);
        listForm.exec();
        nameList = listForm.rowOut();
    }else if(tableName == "structure"){
        StructureForm listForm("",this,false);
        listForm.exec();
        nameList = listForm.rowOut();
    }else if(tableName == "firma"){
        FirmaForm listForm("",this,false);
        listForm.exec();
        nameList = listForm.rowOut();
    }else if(tableName == "teg"){
        TegForm listForm("",this,false);
        listForm.exec();
        nameList = listForm.rowOut();
    }
    templateModel->select();
    for(int row = 0; row < templateModel->rowCount(); ++row){
        QSqlRecord record = templateModel->record(row);
        QModelIndex index = templateModel->index(row,1);
        if(record.value(0).toString() == nameList){
            tableView->setCurrentIndex(index);
            updatePanel(index);
            break;
        }
    }
}

void ViewListTable::deleteRecord()
{
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
        QModelIndex index = tableView->currentIndex();
        QSqlRecord record = templateModel->record(index.row());
        QString idList = record.value(0).toString();
        if(tableName == "material"){
            MaterialForm listForm(idList,this,false);
            listForm.deleteRecord();
        }else if(tableName == "color"){
            ColorForm listForm(idList,this,false);
            listForm.deleteRecord();
        }else if(tableName == "structure"){
            StructureForm listForm(idList,this,false);
            listForm.deleteRecord();
        }else if(tableName == "firma"){
            FirmaForm listForm(idList,this,false);
            listForm.deleteRecord();
        }else if(tableName == "teg"){
            TegForm listForm(idList,this,false);
            listForm.deleteRecord();
        }
        updatePanel(index);
    }
}

void ViewListTable::editRecord()
{
    QModelIndex index = tableView->currentIndex();
    QSqlRecord record = templateModel->record(index.row());
    QString idList = record.value(0).toString();
    if(tableName == "material"){
        MaterialForm listFrom(idList,this,false);
        listFrom.exec();
    }else if(tableName == "color"){
        ColorForm listFrom(idList,this,false);
        listFrom.exec();
    }else if(tableName == "structure"){
        StructureForm listFrom(idList,this,false);
        listFrom.exec();
    }else if(tableName == "firma"){
        FirmaForm listFrom(idList,this,false);
        listFrom.exec();
    }else if(tableName == "teg"){
        TegForm listFrom(idList,this,false);
        listFrom.exec();
    }
    updatePanel(index);
}

void ViewListTable::updatePanel(QModelIndex index)
{
    templateModel->select();
    tableView->setCurrentIndex(index);
}

void ViewListTable::createContextMenu()
{
    addAction = new QAction(tr("Add Record"),this);
    QPixmap pixAdd(":/add.png");
    addAction->setIcon(pixAdd);
    connect(addAction,SIGNAL(triggered()),this,SLOT(addRecord()));

    QPixmap pixDelete(":/delete.png");
    deleteAction = new QAction(tr("Delete Record"),this);
    deleteAction->setIcon(pixDelete);
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(deleteRecord()));

    QPixmap pixEdit(":/edit.png");
    editAction = new QAction(tr("Edit Record"),this);
    editAction->setIcon(pixEdit);
    connect(editAction,SIGNAL(triggered()),this,SLOT(editRecord()));

    tableView->addAction(addAction);
    tableView->addAction(deleteAction);
    tableView->addAction(editAction);
    tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void ViewListTable::searchProcedure()
{
    QString valueTempModel = templateModel->tableName();

    SearchForm searchForm(valueTempModel, this);
    searchForm.exec();
    filterTable = searchForm.rowOut();
    setFilter = true;

    viewTemplateTable(valueTempModel);
}

void ViewListTable::viewTemplateTable(QString)
{
    templateModel->setTable(tableName);

    if(tableName == "material"){
        templateModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
        templateModel->setHeaderData(1,Qt::Horizontal,tr("Persent"));
        labelName = tr("Material");
        if(setFilter){
            templateModel->setFilter(QString("materialname LIKE '%%1%'").arg(filterTable));
        }
    }else if(tableName == "subdivision"){
        templateModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
        templateModel->setRelation(2,QSqlRelation("organization","organizationid","organizationname"));
        templateModel->setHeaderData(2,Qt::Horizontal,tr("Organization Name"));
        if(setFilter){
            templateModel->setFilter(QString("subdivisionname LIKE '%%1%'").arg(filterTable));
        }
    }else if(tableName == "color"){
        templateModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
        labelName = tr("Color");
        if(setFilter){
            templateModel->setFilter(QString("colorname LIKE '%%1%'").arg(filterTable));
        }
    }else if(tableName == "structure"){
        templateModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
        labelName = tr("Structure");
        if(setFilter){
            templateModel->setFilter(QString("structurename LIKE '%%1%'").arg(filterTable));
        }
    }else if(tableName == "firma"){
        templateModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
        labelName = tr("Firma");
        if(setFilter){
            templateModel->setFilter(QString("firmaname LIKE '%%1%'").arg(filterTable));
        }
    }else if(tableName == "teg"){
        templateModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
        labelName = tr("Teg");
        if(setFilter){
            templateModel->setFilter(QString("teglowname LIKE '%%1%'").arg(filterTable));
        }
    }
    templateModel->setSort(1,Qt::AscendingOrder);
    templateModel->select();
    tableView->setModel(templateModel);
    tableView->setColumnHidden(0,true);
    //    if(tableName == "employee"){
    //        tableView->setColumnHidden(2,true);
    //        tableView->setColumnHidden(5,true);
    //        for(int i = 7; i < 15; ++i){
    //            tableView->setColumnHidden(i,true);
    //        }
    //    }else if(tableName == "empcertdate"){
    //        //tableView->setColumnHidden(1,true);
    //    }
    tableView->setItemDelegate(new QSqlRelationalDelegate(this));
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}


void ViewListTable::readSetting()
{
    QSettings settings("AO_Batrakov_Inc.", "Yarn");
    restoreGeometry(settings.value("ViewListTable").toByteArray());
}

void ViewListTable::writeSetting()
{
    QSettings settings("AO_Batrakov_Inc.", "Yarn");
    settings.setValue("ViewListTable", saveGeometry());
}
