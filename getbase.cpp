#include "getbase.h"

GetBase::GetBase(QWidget *parent) : QDialog(parent)
{
}

void GetBase::getBaseHttp()
{
    progressDialog = new QProgressDialog(this);
    fileHttp = new QFile("Yarn.dat");
    fileHttp->open(QIODevice::WriteOnly);
    url = "http://91.102.219.74/QtProject/Yarn/Base/Yarn.dat";

    reply = http.get(QNetworkRequest(url));
    connect(reply,SIGNAL(finished()),this,SLOT(httpDone()));
    connect(reply,SIGNAL(readyRead()),this,SLOT(httpReadyRead()));
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,
            SLOT(updateDataReadProgress(qint64,qint64)));

    progressDialog->setLabelText(tr("Downloading DataBase ..."));
    progressDialog->setEnabled(false);
    progressDialog->exec();
}

void GetBase::httpDone()
{
    fileHttp->close();
    http.destroyed();
}

void GetBase::updateDataReadProgress(qint64 readBytes, qint64 totalBytes)
{
    progressDialog->setMaximum(totalBytes);
    progressDialog->setValue(readBytes);
}

void GetBase::httpReadyRead()
{
    if(fileHttp)
        fileHttp->write(reply->readAll());
    progressDialog->hide();
}
