#include "structureform.h"
#include "numprefix.h"
#include "viewlisttable.h"

StructureForm::StructureForm(QString id,QWidget *parent, bool onlyForRead) : QDialog(parent)
{
    readSettings();
    indexTemp = id;

    labelStructure = new QLabel(tr("Name:"));
    editStructure = new LineEdit;
    editStructure->setReadOnly(true);
    //QRegExp regExp("[\\x0410-\\x044f 0-9 \" -]{150}");
    //editMaterial->setValidator(new QRegExpValidator(regExp,this));
    labelStructure->setBuddy(editStructure);

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
    tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("Material Name")<<tr("Persent"));
    QHeaderView *head = tableWidget->horizontalHeader();
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QHBoxLayout *tableButtonBox = new QHBoxLayout;
    tableButtonBox->addWidget(addRecordButton);
    tableButtonBox->addWidget(deleteRecordButton);
    tableButtonBox->addWidget(editRecordButton);
    tableButtonBox->addStretch();

    templateModel = new QSqlRelationalTableModel(this);
    templateModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);

//    if(indexTemp != ""){
//        QSqlQuery query;
//        query.prepare("SELECT materialname, persent FROM material WHERE materialid = ?");
//        query.addBindValue(indexTemp);
//        query.exec();
//        while(query.next()){
//            editMaterial->setText(query.value(0).toString());
//            editPersent->setText(query.value(1).toString());
//        }
//    }else{
//        editMaterial->clear();
//        editPersent->clear();
//    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelStructure,0,0);
    mainLayout->addWidget(editStructure,0,1);
    mainLayout->addLayout(tableButtonBox,2,0,1,2);
    mainLayout->addWidget(tableWidget,3,0,1,2);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,4,1);
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
            queryTable.prepare("SELECT * FROM structuretable WHERE structureid = :idt;");
            queryTable.bindValue(":idt",indexTemp);
            queryTable.exec();
            if(!queryTable.isActive()){
                QMessageBox::warning(this,QObject::tr("Structure Table, SELECT Table ERROR!"),queryTable.lastError().text());
            }

            int row = 0;
            while(queryTable.next()){

                QSqlQuery queryName;
                queryName.prepare("SELECT * FROM material WHERE materialid = :id);");
                queryName.bindValue(":id",queryTable.value(2).toString());
                queryName.exec();
                if(!queryName.isActive()){
                    QMessageBox::warning(this,QObject::tr("Structure Table, SELECT Material ERROR!"),queryName.lastError().text());
                    return;
                }
                while(queryName.next()){
//                    tableWidget->insertRow(row);
//                    QTableWidgetItem *item = new QTableWidgetItem;
//                    tableWidget->setItem(row,0,item);
//                    tableWidget->item(row,0)->setText(queryName.value(0).toString());

                    QTableWidgetItem *item1 = new QTableWidgetItem;
                    tableWidget->setItem(row,0,item1);
                    tableWidget->item(row,0)->setText(queryName.value(1).toString());

                    QTableWidgetItem *item2 = new QTableWidgetItem;
                    tableWidget->setItem(row,0,item2);
                    tableWidget->item(row,0)->setText(queryName.value(2).toString());
                    ++row;
                }
            }
        }
    }else{
        NumPrefix numPrefix(this);
        indexTemp = numPrefix.getPrefix("trainingsafety");
        if(indexTemp == ""){
            close();
        }
    }
    tableWidget->setAlternatingRowColors(true);
    tableWidget->resizeColumnsToContents();
    //tableWidget->setColumnHidden(4,true);
    head->setStretchLastSection(true);
    setLayout(mainLayout);
    connect(tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(editRecordOfTable()));

    setWindowTitle(tr("Structure"));

}

void StructureForm::editRecord()
{

}

void StructureForm::deleteRecord()
{

}

void StructureForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void StructureForm::addRecordOfTable()
{
    //if(checkingFill()){
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

            if(!value.isEmpty())
                value += ", ";
            value += query.value(1).toString();
            value += " ";
            value += query.value(2).toString();
            value += "%";
        }
        editStructure->setText(value);
    //}
}

void StructureForm::deleteRecordOfTable()
{

}

void StructureForm::editRecordOfTable()
{

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
}
