#include "tegtableform.h"
#include "numprefix.h"
#include <QtSql>
#include "viewlisttable.h"

TegTableForm::TegTableForm(QString iD, QWidget *parent, bool onlyForRead) : QDialog(parent)
{
    readSettings();
    indexJournalPhoto = iD;

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
    if(indexJournalPhoto != ""){
        QSqlQuery query;
        query.prepare("SELECT * FROM tegtable WHERE journalphotoid = ?");
        query.addBindValue(indexJournalPhoto);
        query.exec();
        int row = 0;
        while(query.next()){
            indexTemp = query.value(0).toString();
            qDebug()<<indexTemp;
            QSqlQuery queryTable;
            queryTable.prepare("SELECT * FROM teg WHERE tegid = :id");
            queryTable.bindValue(":id",indexTemp);
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
    }
    tableWidget->setAlternatingRowColors(true);
    tableWidget->resizeColumnsToContents();
    tableWidget->setColumnHidden(1,true);
    head->setStretchLastSection(true);
    setLayout(mainLayout);
    connect(tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(editRecordOfTable()));

    setWindowTitle(tr("Teg"));
}

void TegTableForm::deleteRecord()
{

}

void TegTableForm::editRecord()
{

}

void TegTableForm::saveRecord()
{

}

void TegTableForm::cancelRecord()
{

}

void TegTableForm::addRecordOfTable()
{
    ViewListTable viewListTable("","teg",this);
    viewListTable.exec();
    QString rr = viewListTable.rowOut();

    QSqlQuery query;
    query.prepare("SELECT * FROM teg WHERE tegid = :id");
    query.bindValue(":id",rr);
    query.exec();
    int row = tableWidget->rowCount();
    while(query.next()){

        tableWidget->insertRow(row);
        QTableWidgetItem *item1 = new QTableWidgetItem;
        tableWidget->setItem(row,0,item1);
        tableWidget->item(row,0)->setText(query.value(1).toString());

        QTableWidgetItem *item3 = new QTableWidgetItem;
        tableWidget->setItem(row,1,item3);
        tableWidget->item(row,1)->setText(query.value(0).toString());

        NumPrefix numPrefix(this);
        QString rowPrefix = numPrefix.getPrefix("tegtable");
        QSqlQuery queryTable;
        queryTable.prepare("INSERT INTO tegtable ("
                           "tegtableid, tegid, journalphotoid"
                           ") VALUES(:tegtableid, :tegid, :journalphotoid)");
        queryTable.bindValue(":tegtableid",rowPrefix);
        queryTable.bindValue(":tegid",query.value(0).toString());
        queryTable.bindValue(":journalphotoid",indexJournalPhoto);
        queryTable.exec();
    }
    //editStructure->setText(value);
}

void TegTableForm::deleteRecordOfTable()
{

}

void TegTableForm::editRecordOfTable()
{

}

void TegTableForm::readSettings()
{

}

void TegTableForm::writeSettings()
{

}

void TegTableForm::sortTable(int)
{

}

void TegTableForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}
