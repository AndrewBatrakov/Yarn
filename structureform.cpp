#include "structureform.h"
#include "numprefix.h"
#include "viewlisttable.h"
#include "materialform.h"

StructureForm::StructureForm(QString id,QWidget *parent, bool onlyForRead) : QDialog(parent)
{
    readSettings();
    indexTemp = id;

    labelStructure = new QLabel;
    editStructure = new LineEdit;
    editStructure->setReadOnly(true);
    labelStructure->setBuddy(editStructure);

    saveButton = new QPushButton(tr("Save"));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(saveRecord()));
    saveButton->setToolTip(tr("Save And Close Button"));

    cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setDefault(true);
    cancelButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(accept()));
    cancelButton->setToolTip(tr("Cancel Button"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(saveButton,QDialogButtonBox::ActionRole);

    //Table
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

    tableWidget = new QTableWidget(0,4);
    tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("Material Name")<<tr("Persent"));
    QHeaderView *head = tableWidget->horizontalHeader();
    connect(head,SIGNAL(sectionClicked(int)),this,SLOT(sortTable(int)));
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QHBoxLayout *tableButtonBox = new QHBoxLayout;
    tableButtonBox->addWidget(addRecordButton);
    tableButtonBox->addWidget(deleteRecordButton);
    tableButtonBox->addWidget(editRecordButton);
    tableButtonBox->addStretch();

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelStructure,0,0);
    mainLayout->addWidget(editStructure,0,1);
    mainLayout->addLayout(tableButtonBox,1,0,1,2);
    mainLayout->addWidget(tableWidget,2,0,1,2);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,3,1);
        editStructure->selectAll();
    }
    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT * FROM structure WHERE structureid = ?");
        query.addBindValue(indexTemp);
        query.exec();
        while(query.next()){
            editStructure->setText(query.value(1).toString());

            QSqlQuery queryTable;
            queryTable.prepare("SELECT * FROM structuretable WHERE structureid = :id");
            queryTable.bindValue(":id",indexTemp);
            queryTable.exec();
            int row = 0;
            while(queryTable.next()){
                QSqlQuery queryName;
                queryName.prepare("SELECT * FROM material WHERE materialid = :id");
                queryName.bindValue(":id",queryTable.value(2).toString());
                queryName.exec();
                while(queryName.next()){
                    tableWidget->insertRow(row);
                    QTableWidgetItem *item = new QTableWidgetItem;
                    tableWidget->setItem(row,0,item);
                    tableWidget->item(row,0)->setText(queryName.value(1).toString());

                    QTableWidgetItem *item1 = new QTableWidgetItem;
                    tableWidget->setItem(row,1,item1);
                    tableWidget->item(row,1)->setText(queryName.value(2).toString());

                    QTableWidgetItem *item2 = new QTableWidgetItem;
                    tableWidget->setItem(row,2,item2);
                    tableWidget->item(row,2)->setText(queryName.value(0).toString());

                    QTableWidgetItem *item3 = new QTableWidgetItem;
                    tableWidget->setItem(row,3,item3);
                    tableWidget->item(row,3)->setText(queryTable.value(0).toString());
                    ++row;
                }
            }
        }
    }else{
        NumPrefix numPrefix(this);
        indexTemp = numPrefix.getPrefix("structure");
        if(indexTemp == ""){
            close();
        }
    }
    tableWidget->setAlternatingRowColors(true);
    tableWidget->resizeColumnsToContents();
    tableWidget->setColumnHidden(2,true);
    tableWidget->setColumnHidden(3,true);
    head->setStretchLastSection(true);
    setLayout(mainLayout);
    connect(tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(editRecordOfTable()));

    setWindowTitle(tr("Structure"));

}

void StructureForm::editRecord()
{
    QSqlQuery query;
    query.prepare("SELECT * FROM structure WHERE structureid = :id");
    query.bindValue(":id",indexTemp);
    query.exec();
    query.next();
    if(query.isValid()){
        QSqlQuery queryU;
        queryU.prepare("UPDATE structure SET "
                       "structurename = :name "
                       "WHERE structureid = :id");
        queryU.bindValue(":name",editStructure->text().simplified());
        queryU.bindValue(":id",indexTemp);
        queryU.exec();
    }else{
        QSqlQuery queryI;
        queryI.prepare("INSERT INTO structure ("
                       "structurename, structureid) VALUES (:name, "
                       ":id)");
        queryI.bindValue(":name",editStructure->text().simplified());
        queryI.bindValue(":id",indexTemp);
        queryI.exec();
    }
}

void StructureForm::deleteRecord()
{
    QSqlQuery query;
    query.prepare("DELETE FROM structure WHERE structureid = :id");
    query.bindValue(":id",indexTemp);
    query.exec();
}

void StructureForm::saveRecord()
{
    int que = 0;
    int row = tableWidget->rowCount();
    for(int val = 0; val < row; ++val){
        que = que + tableWidget->item(val,1)->text().toInt() ;

    }
    if(que < 100){
        QMessageBox::warning(this,tr("Attention!"),tr("Structure probably 100 %!!!\n"
                                                      "You select %1\%").arg(que));
        return;
    }
    editStructure->clear();
    tableWidget->sortByColumn(1,Qt::DescendingOrder);
    QString value = "";// = editStructure->text();
    for(int rowT = 0; rowT < tableWidget->rowCount(); ++rowT){
        if(!value.isEmpty())
            value += ", ";
        value += tableWidget->item(rowT,0)->text();
        value += " ";
        value += tableWidget->item(rowT,1)->text();
        value += "%";
    }
    editStructure->setText(value);

    QSqlQuery query;
    query.prepare("SELECT * FROM structure WHERE structurename = :structurename");
    query.bindValue(":structurename",editStructure->text());
    query.exec();
    query.next();
    if(!query.isValid()){
        editRecord();
        emit accept();
    }else{
        QString tempString = editStructure->text();
        tempString += QObject::tr(" is availble!");
        QMessageBox::warning(this,QObject::tr("Atention!!!"),tempString);
    }
}

void StructureForm::cancelRecord()
{
    for(int row; row < tableWidget->rowCount(); ++row){
        QSqlQuery query;
        query.prepare("DELETE FROM structuretable WHERE structuretableid = :id");
        query.bindValue(":id",tableWidget->item(row,3)->text());
        query.exec();
    }
    indexTemp = "";
    emit accept();
}

void StructureForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void StructureForm::addRecordOfTable()
{
    ViewListTable viewListTable("","material",this);
    viewListTable.exec();
    QString rr = viewListTable.rowOut();
    QSqlQuery query;
    query.prepare("SELECT * FROM material WHERE materialid = :id");
    query.bindValue(":id",rr);
    query.exec();
    int row = tableWidget->rowCount();
    int que = 0;
    for(int val = 0; val < row; ++val){
        que = que + tableWidget->item(val,1)->text().toInt() ;
    }
    QString value = editStructure->text();
    while(query.next()){

        int val = que + query.value(2).toInt();
        if(val > 100){
            QMessageBox::warning(this,tr("Attention!"),
                                 tr("Structure probably 100 %!!!\n"
                                    "You select %1\%").arg(val));
            break;
        }
        tableWidget->insertRow(row);
        QTableWidgetItem *item1 = new QTableWidgetItem;
        tableWidget->setItem(row,0,item1);
        tableWidget->item(row,0)->setText(query.value(1).toString());

        QTableWidgetItem *item2 = new QTableWidgetItem;
        tableWidget->setItem(row,1,item2);
        tableWidget->item(row,1)->setText(query.value(2).toString());

        QTableWidgetItem *item3 = new QTableWidgetItem;
        tableWidget->setItem(row,2,item3);
        tableWidget->item(row,2)->setText(query.value(0).toString());

        if(!value.isEmpty())
            value += ", ";
        value += query.value(1).toString();
        value += " ";
        value += query.value(2).toString();
        value += "%";

        NumPrefix numPrefix(this);
        QString rowPrefix = numPrefix.getPrefix("structuretable");

        QTableWidgetItem *item4 = new QTableWidgetItem;
        tableWidget->setItem(row,3,item4);
        tableWidget->item(row,3)->setText(rowPrefix);

        QSqlQuery queryTable;
        queryTable.prepare("INSERT INTO structuretable ("
                           "structuretableid, structureid, materialid"
                           ") VALUES(:structuretableid, :structureid, :materialid)");
        queryTable.bindValue(":structuretableid",rowPrefix);
        queryTable.bindValue(":structureid",indexTemp);
        queryTable.bindValue(":materialid",query.value(0).toString());
        queryTable.exec();
    }
    editStructure->setText(value);
}

void StructureForm::deleteRecordOfTable()
{
    int que = QMessageBox::warning(this,tr("Attention!"),
                                   tr("Really delete?"),
                                   QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if(que == QMessageBox::Yes){
        int rowNow = tableWidget->currentRow();

        QSqlQuery query;
        query.prepare("DELETE FROM structuretable WHERE structuretableid = :id");
        query.bindValue(":id",tableWidget->item(rowNow,3)->text());
        query.exec();

        tableWidget->removeRow(tableWidget->currentRow());
        tableWidget->repaint();
    }
}

void StructureForm::editRecordOfTable()
{
    int rowNow = tableWidget->currentRow();
    MaterialForm materialForm(tableWidget->item(rowNow,2)->text(),this,false);
    materialForm.exec();
    QString materialID = materialForm.rowOut();

    QSqlQuery query;
    query.prepare("SELECT * FROM material WHERE materialid = :id");
    query.bindValue(":id",materialID);
    query.exec();
    while(query.next()){
        //tableWidget->insertRow(row);
        QTableWidgetItem *item1 = new QTableWidgetItem;
        tableWidget->setItem(rowNow,0,item1);
        tableWidget->item(rowNow,0)->setText(query.value(1).toString());

        QTableWidgetItem *item2 = new QTableWidgetItem;
        tableWidget->setItem(rowNow,1,item2);
        tableWidget->item(rowNow,1)->setText(query.value(2).toString());

        QTableWidgetItem *item3 = new QTableWidgetItem;
        tableWidget->setItem(rowNow,2,item3);
        tableWidget->item(rowNow,2)->setText(query.value(0).toString());

        //sortTable(1);
        tableWidget->setColumnHidden(2,true);
        tableWidget->repaint();
    }
}

void StructureForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Yarn");
    restoreGeometry(settings.value("Structure").toByteArray());
}

void StructureForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Yarn");
    settings.setValue("Structure", saveGeometry());
    settings.remove("indexPhoto");
}

void StructureForm::sortTable(int index)
{
    tableWidget->setSortingEnabled(true);
    tableWidget->sortByColumn(index,Qt::AscendingOrder);
}
