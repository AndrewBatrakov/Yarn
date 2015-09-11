#ifndef GETJOURNAL_H
#define GETJOURNAL_H

#include <QtWidgets>
#include <QtNetwork>

class GetJournal : public QDialog
{
    Q_OBJECT

public:
    GetJournal(QWidget *parent);

public slots:
    QStringList getJournalHttp(QString urlString);

private slots:
    void httpDone();
    void updateDataReadProgress(qint64,qint64);
    void httpReadyRead();

    void closeConnection();

private:
    QUrl url;
    QNetworkAccessManager http;
    QNetworkReply *reply;
    QFile *fileHttp;
    QProgressDialog *progressDialog;
};

#endif // GETJOURNAL_H
