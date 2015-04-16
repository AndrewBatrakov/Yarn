#include "registrationform.h"
#include "dbdirection.h"

RegistrationForm::RegistrationForm(QWidget *parent) : QDialog(parent)
{
}

bool RegistrationForm::checkInput()
{
    bool value;
    DBDirection manipulation;
    if(!manipulation.connectDataBase()){
        close();
        value = false;
    }else{
        value = true;
    }
    return value;
}

bool RegistrationForm::checkFile()
{
    QFile file("Yarn.dat");
    if(!file.exists()){
        int quest = QMessageBox::warning(this,tr("Attention!"),
                                         tr("No DataBase File!\n"
                                            "Upload from Server?"),
                                         QMessageBox::Yes,QMessageBox::No);
        if(quest == QMessageBox::Yes){
            return true;
        }else{
            return false;
        }
    }else{
        return true;
    }
}
