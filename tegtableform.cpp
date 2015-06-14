#include "tegtableform.h"
#include "numprefix.h"
#include <QtSql>
#include "viewlisttable.h"
#include "tegform.h"

TegTableForm::TegTableForm(QString iD, QString idPhoto, QWidget *parent, bool onlyForRead) : QDialog(parent)
{
    readSettings();
    indexTemp = iD;
    indexJournalPhoto = idPhoto;

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

    tableWidget = new QTableWidget(0,2);
    tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("Teg"));
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
    mainLayout->addLayout(tableButtonBox,1,0,1,2);
    mainLayout->addWidget(tableWidget,2,0,1,2);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,3,1);
    }
    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT * FROM tegtable WHERE journalphotoid = :journalphotoid");
        query.bindValue(":journalphotoid",indexJournalPhoto);
        //query.bindValue(":id",indexTemp);
        query.exec();
        int row = 0;
        while(query.next()){
            QSqlQuery queryTable;
            queryTable.prepare("SELECT * FROM teg WHERE tegid = :id");
            queryTable.bindValue(":id",query.value(1).toString());
            queryTable.exec();

            while(queryTable.next()){
                tableWidget->insertRow(row);
                QTableWidgetItem *item = new QTableWidgetItem;
                tableWidget->setItem(row,0,item);
                tableWidget->item(row,0)->setText(queryTable.value(1).toString());

                QTableWidgetItem *item1 = new QTableWidgetItem;
                tableWidget->setItem(row,1,item1);
                tableWidget->item(row,1)->setText(queryTable.value(0).toString());
                ++row;
            }
        }
    }else{
        NumPrefix numPrefix(this);
        indexTemp = numPrefix.getPrefix("tegtable");
    }
    tableWidget->setAlternatingRowColors(true);
    tableWidget->resizeColumnsToContents();
    tableWidget->setColumnHidden(1,true);
    head->setStretchLastSection(true);
    setLayout(mainLayout);
    connect(tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(editRecordOfTable()));

    setWindowTitle(tr("Teg"));
    exchangeFile.setFileName("exchange.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }
}

void TegTableForm::deleteRecord()
{
    QSqlQuery query;
    query.prepare("DELETE FROM tegtable WHERE tegtableid = :id");
    query.bindValue(":id",indexTemp);
    query.exec();
}

void TegTableForm::saveRecord()
{
    emit accept();
}

void TegTableForm::addRecordOfTable()
{
    ViewListTable viewListTable("","teg",this);
    viewListTable.exec();
    QString rr = viewListTable.rowOut();

    int row = tableWidget->rowCount();
    for(int i = 0; i < row; ++i){
        if(rr == tableWidget->item(i,1)->text()){

            QMessageBox::warning(this,tr("Attention!"),tr("%1 is present in tegs!").arg(tableWidget->item(i,0)->text()));
            return;
        }
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM teg WHERE tegid = :id");
    query.bindValue(":id",rr);
    query.exec();

    while(query.next()){

        tableWidget->insertRow(row);
        QTableWidgetItem *item1 = new QTableWidgetItem;
        tableWidget->setItem(row,0,item1);
        tableWidget->item(row,0)->setText(query.value(1).toString());

        QTableWidgetItem *item3 = new QTableWidgetItem;
        tableWidget->setItem(row,1,item3);
        tableWidget->item(row,1)->setText(query.value(0).toString());

        QSqlQuery queryTable;
        queryTable.prepare("INSERT INTO tegtable ("
                           "tegtableid, tegid, journalphotoid"
                           ") VALUES(:tegtableid, :tegid, :journalphotoid)");
        queryTable.bindValue(":tegtableid",indexTemp);
        queryTable.bindValue(":tegid",query.value(0).toString());
        queryTable.bindValue(":journalphotoid",indexJournalPhoto);
        queryTable.exec();
    }
    tableWidget->repaint();
}

void TegTableForm::deleteRecordOfTable()
{
    int que = QMessageBox::warning(this,tr("Attention!"),
                                   tr("Really delete?"),
                                   QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if(que == QMessageBox::Yes){
        int rowNow = tableWidget->currentRow();

        QSqlQuery query;
        query.prepare("DELETE FROM tegtable WHERE tegid = :id");
        query.bindValue(":id",tableWidget->item(rowNow,1)->text());
        query.exec();
        tableWidget->removeRow(tableWidget->currentRow());
        tableWidget->repaint();
    }
}

void TegTableForm::editRecordOfTable()
{
    int rowNow = tableWidget->currentRow();
    TegForm openForm(tableWidget->item(rowNow,1)->text(),this,false);
    openForm.exec();
    QString tegID = openForm.rowOut();

    QSqlQuery query;
    query.prepare("SELECT * FROM teg WHERE tegid = :id");
    query.bindValue(":id",tegID);
    query.exec();
    while(query.next()){
        QTableWidgetItem *item1 = new QTableWidgetItem;
        tableWidget->setItem(rowNow,0,item1);
        tableWidget->item(rowNow,0)->setText(query.value(1).toString());

        QTableWidgetItem *item3 = new QTableWidgetItem;
        tableWidget->setItem(rowNow,1,item3);
        tableWidget->item(rowNow,1)->setText(query.value(0).toString());

        //sortTable(1);
        tableWidget->setColumnHidden(1,true);
        tableWidget->repaint();
    }
}

void TegTableForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Yarn");
    restoreGeometry(settings.value("TegTable").toByteArray());
}

void TegTableForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Yarn");
    settings.setValue("TegTable", saveGeometry());
}

void TegTableForm::sortTable(int index)
{
    tableWidget->setSortingEnabled(true);
    tableWidget->sortByColumn(index,Qt::AscendingOrder);
}

void TegTableForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}
