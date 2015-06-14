#include "phototeg.h"
#include <QtSql>
#include "tegtableform.h"

PhotoTeg::PhotoTeg(QWidget *parent) : QLabel(parent)
{

}

void PhotoTeg::mouseReleaseEvent(QMouseEvent *)
{
//    if(event->button() == Qt::LeftButton){
//        emit clicked();
//    }
//    if(event->button() == Qt::RightButton){
//        QMenu menu(this);
//        QPixmap pixD(":/edit.png");
//        menu.addAction(pixD,tr("Edit Tegs For Search"));
//        menu.setContextMenuPolicy(Qt::ActionsContextMenu);
//        QAction *m = menu.exec(event->globalPos());
//        if(m){
//            TegTableForm openForm(indexTemp,this,false);
//            openForm.exec();
//        }
////        this->clear();
////        this->setText(tr("No Photo"));
//    }
}
