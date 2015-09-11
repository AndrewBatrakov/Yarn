#include "getjournal.h"

GetJournal::GetJournal(QWidget *parent) : QDialog(parent)
{
    //QDirModel *dirModel = new QDirModel;
//    QTreeView treeView;

//    QStringList tt = getJournalHttp();
//    qDebug()<<tt;
//    QStringListModel model;
//    model.setStringList(tt);
//    treeView.setModel(&model);
//    treeView.setItemDelegate(new QItemDelegate(&model));
//    QGridLayout *mainLayout = new QGridLayout;
//    mainLayout->addWidget(&treeView,0,0);
//    setLayout(mainLayout);
}

QStringList GetJournal::getJournalHttp(QString urlString)
{
    QStringList stringList;

    progressDialog = new QProgressDialog(this);
    fileHttp = new QFile("Yarn.dat");
    fileHttp->open(QIODevice::WriteOnly);
    url = urlString;//"http://91.102.219.74/QtProject/Yarn/";
    //url = "http://91.102.219.74/QtProject/Yarn/Yarn.exe";

    reply = http.get(QNetworkRequest(url));
    connect(reply,SIGNAL(readyRead()),this,SLOT(httpReadyRead()));
    connect(reply,SIGNAL(finished()),this,SLOT(httpDone()));

    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,
            SLOT(updateDataReadProgress(qint64,qint64)));
    connect(reply,SIGNAL(uploadProgress(qint64,qint64)),this,
            SLOT(updateDataReadProgress(qint64,qint64)));

    progressDialog->setLabelText(tr("Downloading DataBase ..."));
    progressDialog->setEnabled(true);
    progressDialog->exec();
    QString fileList = reply->readAll();
    qDebug()<<fileList;
    int startIter = fileList.indexOf("Parent Directory");
    fileList = fileList.right(fileList.count() - startIter);
    int fileListAll = fileList.count();
    for(int i=0;i<fileListAll;++i){
        int iter = fileList.indexOf("> ");
        if(iter != -1){
            fileList = fileList.right(fileList.count()- 2 - iter);
            //
            QString word = fileList;
            word = word.left(word.indexOf("</a>"));
            if(word.right(1) == "/")
                continue;
            qDebug()<<iter<<word;
            stringList<<word;
        }else{
            break;
        }
    }
    return stringList;
}

void GetJournal::httpDone()
{
    fileHttp->close();
    closeConnection();
}

void GetJournal::updateDataReadProgress(qint64 readBytes, qint64 totalBytes)
{
    progressDialog->setMaximum(totalBytes);
    progressDialog->setValue(readBytes);
}

void GetJournal::httpReadyRead()
{
//    if(fileHttp)
//        fileHttp->write(reply->readAll());

}

void GetJournal::closeConnection()
{
    progressDialog->hide();
    http.destroyed();
}

