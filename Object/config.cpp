/*
Copyright (c) 2013, Luiz Cândido da Silva Júnior, heron@jaguati.com.br

Copyright (c) 2013, Jonathan,  jonathan@jaguati.com.br
 All rights reserved. 

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 

 Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 

 Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. 

 Neither the name of Pedyrum may be used to endorse or promote products derived from this software without specific prior written permission. 

 THIS SOFTWARE IS PROVIDED BY {{THE COPYRIGHT HOLDERS AND CONTRIBUTORS}} "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL {{THE COPYRIGHT HOLDER OR CONTRIBUTORS}} BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


 #include "config.h"
#include <QDebug>
#include <QFile>
#include <QDir>

Config::Config(QObject *parent) :
    QObject(parent)
{
}

void Config::start(QSqlDatabase *sdb){
    bd.sdb = *sdb;
}

bool Config::insertDataBusiness()
{
    bd.exec("delete from business");

    QList<QString> values;

    values << bName << bPhone << bAddress << bDistrict << bCity << bState;

    bool insert = bd.insert("business", values);

    if (!insert)
        qDebug() << "[CONFIG] [insertDataBusiness] Erro na inserção";

    return insert;
}

void Config::loadDataBusiness()
{
    if (bd.search("select * from business")){
        bName = bd.loadSimple(0);
        bPhone = bd.loadSimple(1);
        bAddress = bd.loadSimple(2);
        bDistrict = bd.loadSimple(3);
        bCity = bd.loadSimple(4);
        bState = bd.loadSimple(5);
    }else{
        bName.clear();
        bPhone.clear();
        bAddress.clear();
        bDistrict.clear();
        bCity.clear();
        bState.clear();
    }
}

bool Config::insertDataOrder()
{
    bd.exec("delete from configorder");

    QList<QString> values;

    values << QString::number(numberOfCopies);

    switch (fontSize){

    case 1:{
        values << "p";
        break;
    }

    case 2:{
        values << "m";
        break;
    }

    case 3:{
        values << "g";
        break;
    }

    }

    values << QString::number(widthPaper);

    switch(typeNumering){
    case 1:{
        values << "p"; // padrão
        break;
    }

    case 2:{
        values << "d"; // data
        break;
    }

    }

    values << QString::number(currentNumber);

    bool insert = bd.insert("configorder", values);

    if (!insert)
        qDebug() << "[CONFIG] [insertDataOrder] Erro na inserção";

    return insert;
}

void Config::loadDataOrder()
{
    if (bd.search("select * from configorder")){
        numberOfCopies = bd.loadSimple(0).toInt();

        if (bd.loadSimple(1) == "p")
            fontSize = 1;
        else if (bd.loadSimple(1) == "m")
            fontSize = 2;
        else if (bd.loadSimple(1) == "g")
            fontSize = 3;
        else
            fontSize = 0;

        widthPaper = bd.loadSimple(2).toInt();

        if (bd.loadSimple(3) == "p")
            typeNumering = 1;
        else if (bd.loadSimple(3) == "d")
            typeNumering = 2;
        else
            typeNumering = 0;

        currentNumber = bd.loadSimple(4).toInt();
    }else{
        numberOfCopies = 1;
        fontSize = 0;
        widthPaper = 4;
        typeNumering = 0;
        currentNumber = 0;
    }
}

bool Config::insertDataAlertOrder()
{
    if (bd.search("select * from configAlert"))
        return bd.exec("update configAlert set daysAlertOrder = " + daysAlertOrder);

    return bd.insert("configAlert", "daysAlertOrder", daysAlertOrder);
}

void Config::loadDataAlertOrder()
{
    if (bd.search("select daysAlertOrder from configAlert")){
        daysAlertOrder = bd.loadSimple(0);
    }else{
        daysAlertOrder = "1";
    }
}

QString Config::dateLastBackup()
{
    QString date;
    if (bd.search("select datelastbackup from generalConfig")){
        date = QString("Último backup foi realizado no dia: %1.").arg(tools.convertDate2(bd.loadSimple(0)));
    }else{
        date = "Nenhum backup realizado até o momento.";
    }

    return date;
}

QList<QString> Config::getPaymentTypes()
{
    QList<QString> listEmpty;

    if (bd.search("select * from paymenttypes"))
        return bd.loadColumn(1);

    return listEmpty;
}

bool Config::insertDataPayementTypes()
{
    if (bd.exec("delete from paymenttypes")){
        int i = 1;

        foreach(QString type, paymentTypes){

            if (bd.exec(QString("insert into paymenttypes values(%1, '%2')").arg(QString::number(i), type))){

                i++;

            }else{

                qDebug() << "[config] [insertDataPayementTypes] Erro:" << bd.lastError;

                return false;
            }
        }

        return true;
    }

    return false;
}

bool Config::saveDataAccessDatabase(QString username, QString password, QString ip, int port)
{
    QFile file(QDir::currentPath() + "/config.txt");

    if (file.open(QIODevice::WriteOnly)){

        QTextStream stream(&file);

        stream << e.encrypt(username) + "\n";

        stream << e.encrypt(password) + "\n";

        stream << e.encrypt(ip) + "\n";

        stream << e.encrypt(QString::number(port));

        file.close();

        return true;
    }

    return false;
}

bool Config::loadDataNetwork()
{
    QFile file(QDir::currentPath() + "/config.txt");

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        username = e.decrypt(in.readLine());
        password = e.decrypt(in.readLine());
        ip = e.decrypt(in.readLine());
        port = e.decrypt(in.readLine());

        return true;
    }

    return false;
}

bool Config::canAddAccount(){
    QString sql;

    sql = QString("select * from usersystem where username = '%1'").arg(usernameSystemUser);

    if (bd.search(sql))
        return false;

    return true;
}

QList<QList<QString> > Config::getAllUsers()
{
    QList<QList<QString> >users;

    if (bd.search("select * from usersystem")){
        users.append(bd.loadColumn(0));
        users.append(bd.loadColumn(1));
    }

    return users;
}

bool Config::removeUser(QString id)
{
    QString sql = QString("delete from usersystem where idusersystem = '%1'").arg(id);

    if (bd.exec(sql))
        return true;

    return false;
}

QString Config::idRoot()
{
    if (bd.search("select min(idusersystem) from usersystem"))
        return bd.loadSimple(0);

    return "0";
}

bool Config::loadDataUser(QString idUser)
{
    if (bd.search(QString("select * from usersystem where idusersystem = '%1'").arg(idUser))){

        usernameSystemUser = bd.loadSimple(1);
        passwordSystemUser = e.decrypt(bd.loadSimple(2));
        secretQuestionSystemUser = bd.loadSimple(3);
        secretAnswerSystemUser = e.decrypt(bd.loadSimple(4));

        bd.search(QString("select * from accesslevel where iduser = %1").arg(idUser));

        QList<QString> levels = bd.loadLine(0);

        cantreatment = levels[1] == "true"? true: false;
        canclient = levels[2] == "true"? true: false;
        canproduct = levels[3] == "true"? true: false;
        canordered = levels[4] == "true"? true: false;
        canemployee = levels[5] == "true"? true: false;
        canalert = levels[6] == "true"? true: false;
        canbill = levels[7] == "true"? true: false;
        cancash = levels[8] == "true"? true: false;
        cancreateuser = levels[9] == "true"? true: false;
        canalteraccess = levels[10] == "true"? true: false;
    }
}

bool Config::insertUserAccount()
{
    QString sql;

    sql = QString("insert into usersystem values('%1', '%2', '%3', '%4', '%5')").arg(QString::number(bd.generateID("usersystem", "idusersystem")), usernameSystemUser, e.encrypt(passwordSystemUser), secretQuestionSystemUser, e.encrypt(secretAnswerSystemUser));

    if (bd.exec(sql))
        return true;

    return false;
}

bool Config::updateUserAccount(QString id)
{
    QString sql;

    sql = QString("update usersystem set username = '%1', password = '%2', secretquestion = '%3', secretanswer = '%4' where idusersystem = '%5';").arg(usernameSystemUser, e.encrypt(passwordSystemUser), secretQuestionSystemUser, e.encrypt(secretAnswerSystemUser), id);

    if (bd.exec(sql))
        return true;

    return false;
}

bool Config::insertUserAccessLevel(QString id)
{
    QString sql = QString("delete from accesslevel where iduser = %1").arg(id);
    if (bd.exec(sql)){
        QList<QString> values;

        values << id;

        values << (cantreatment?"y":"n");
        values << (canclient?"y":"n");
        values << (canproduct?"y":"n");
        values << (canordered?"y":"n");
        values << (canemployee?"y":"n");
        values << (canalert?"y":"n");
        values << (canbill?"y":"n");
        values << (cancash?"y":"n");
        values << (cancreateuser?"y":"n");
        values << (canalteraccess?"y":"n");

        if (bd.insert("accesslevel", values))
            return true;

        return false;
    }

    return false;
}

bool Config::canEditAccount(QString id)
{
    QString sql = QString("select cancreateuser from accesslevel where iduser = %1").arg(id);

    if (bd.search(sql))
        return (bd.loadSimple(0) == "true"? true: false);

    return false;
}

bool Config::canAlterAccess(QString id)
{
    QString sql = QString("select canalteraccess from accesslevel where iduser = %1").arg(id);

    if (bd.search(sql))
        return (bd.loadSimple(0) == "true"? true: false);

    return false;
}

