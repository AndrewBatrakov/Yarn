#include "photolabel.h"
#include <QtSql>

PhotoLabel::PhotoLabel(QWidget *parent) :
    QLabel(parent)
{
}

void PhotoLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        emit clicked();
    }
    if(event->button() == Qt::RightButton){
        QMenu menu(this);
        QPixmap pixD(":/delete.png");
        menu.addAction(pixD,tr("Delete Photo"));
        menu.setContextMenuPolicy(Qt::ActionsContextMenu);
        QAction *m = menu.exec(event->globalPos());
        if(m){
            QSettings settings("AO_Batrakov_Inc.","Yarn");
            QString in = settings.value("indexPhoto","").toString();
            QSqlQuery query;
            query.prepare("DELETE FROM photo WHERE photoid = :id");
            query.bindValue(":id",in);
            query.exec();
            query.next();
        }
        this->clear();
        this->setText(tr("No Photo"));
    }
}
