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


 #include "order.h"
#include <QDebug>
#include <QDateTime>

Order::Order(QString connectionName, QObject *parent) :
    QObject(parent)
{
}

void Order::start(QSqlDatabase *sdb){
    bd.sdb = *sdb;

    typeClient = 1;

    instance();

    client = new Client();

    client->start(sdb);
}

void Order::setTypeClient(int typeClient){
    this->typeClient = typeClient;
}

QList<QString> Order::getDataClient(QString phone)
{
    QList<QString> listReturn;

    switch (typeClient){

    case 1:
        if (bd.search("select * from client where phone = '" + phone + "'"))
            return bd.loadLine(0);
        return listReturn;

    case 2:
        if (bd.search("select * from clientbusiness where phone = '" + phone + "'"))
            return bd.loadLine(0);
        return listReturn;
    default:
        return listReturn;

    }
}

void Order::instance()
{
    _idSelectedBartender = -1;

    bd.createModelQuery("treatmenttable", "select * from treatmenttable");

    bd.createModelQuery("product", "select * from product order by idProduct");

    bd.createModelQuery("client", "select phone from client");

    bd.createModelQuery("clientBusiness", "select phone from clientbusiness");

    bd.createCompleter("product", "product", 1, false, QCompleter::PopupCompletion, BDTools::QueryModel);

    bd.createCompleter("phone", "client", 0, false, QCompleter::PopupCompletion, BDTools::QueryModel);

    bd.createCompleter("phoneBusiness", "clientBusiness", 0, false, QCompleter::PopupCompletion, BDTools::QueryModel);
}

bool Order::insertOrder(){

    qDebug() << " ---------------- SIZES" << orderSizes ;

    Q_UNUSED(orderNameProduct);

    // insert de pedido

    // preparando a sql de insert

    if (! isAdd){
        bd.exec("delete from schedule where idOrder = " + idOrder);
        bd.exec("delete from paymentorder where idOrder = " + idOrder);
        bd.exec("delete from productitem where idOrder = " + idOrder);
        bd.exec("delete from itemorder where idOrder = " + idOrder);

        bd.exec("commit");

        QList<QString> fields;
        QList<QString> values;

        fields << "typeclient";
        values << QString::number(typeClient);
        fields << "idclient";
        values << QString::number(idClient);
        fields << "typeorder";

        switch (typeOrder){

        case DELYVERY:{
            values << "Delivery";

            break;
        }

        case TABLE:{
            values << "Mesa";

            break;
        }

        case COUNTER:{
            values << "Balcão";

            break;
        }

        }

        fields << "valueorder";
        values << QString::number(totalValueOrderProducts);

        fields << "valueshipping";
        values << QString::number(valueShipping);

        fields << "valuepaid";
        values << QString::number(valuePaid);

        fields << "ispaid";
        if (isPaid)
            values << "true";
        else
            values << "false";


        if (! bd.update("ordered", fields, values, "idorder", idOrder)){
            qDebug() << "[ORDER] [UPDATE ORDER] Erro na edição do pedido";
        }

    }else{
        QString sqlEmployeeOrder = "";

        idOrder = QString::number(bd.generateID("ordered", "idorder"));

        QString sql = "insert into ordered values (";
        sql += idOrder + ", ";
        sql += QString::number(typeClient) + ", ";
        sql += QString::number(idClient) + ", ";
        sql += "'" + tools.convertDate(tools.getDate(Qt::SystemLocaleShortDate)) + "', ";
        sql += "'" + tools.getTime("hh:mm:ss") + "', ";

        QString status;

        switch (typeOrder){

        case DELYVERY:{
            sql += "'Delivery', ";

            status = "Em andamento";

            break;
        }

        case TABLE:{
            sql += "'Mesa', ";

            status = "Fechado";

            // aqui faz o tratamento especifico de mesa

            QString sqlTable = "insert into treatmenttable values('";

            sqlTable += idOrder;
            sqlTable += "', '";
            sqlTable += isMedia? "true": "false";
            sqlTable += "', '";
            sqlTable += isCompound? "true": "false";
            sqlTable += "', '";
            sqlTable += isChildren? "true": "false";
            sqlTable += "', '";
            sqlTable += isFather? "true": "false";
            sqlTable += "', '";
            sqlTable += QString::number(idBartender);
            sqlTable += "', '";
            sqlTable += linkedTables;
            sqlTable += "', '";
            if (isMedia)
                sqlTable += QString::number(countPeople);
            else
                sqlTable += "0";
            sqlTable += "', '";
            qDebug() << "currentTable" << currentTable;
            sqlTable += QString::number(currentTable);
            sqlTable += "')";

            bd.exec(sqlTable);


            // verifica se tem garçom e, caso tenha, insere no employeeorder

            if (_idSelectedBartender != -1){
                 sqlEmployeeOrder = QString("insert into employeeorder values('%1', '%2')").arg(idOrder, QString::number(_idSelectedBartender));

                 qDebug() << "_idSelectedBartender" << _idSelectedBartender;

                qDebug() << "sqlEmployeeOrder" << sqlEmployeeOrder;
            }

            break;
        }

        case COUNTER:{
            sql += "'Balcão', ";

            status = "Fechado";

            break;
        }

        }

        sql += QString::number(totalValueOrderProducts) + ", ";
        sql += QString::number(valueShipping) + ", ";
        sql += QString::number(valuePaid) + ", ";

        if (isPaid)
            sql += "'true', ";
        else
            sql += "'false', ";

        sql += "'";

        sql += status;

        sql += "', ";

        QString number = "";

        if (bd.search("select typenumbering, currentnumber from configorder")){
            if (bd.loadSimple(0) == "p"){
                number = QString::number(bd.loadSimple(1).toInt() + 1);
            }else{

                number = tools.getDate().mid(0, 2) + tools.getDate().mid(3, 2) + tools.getDate().mid(8, 2);

                if (bd.search("select numberorder from ordered where numberorder like '" + number + "%' order by idorder desc")){

                    QString sufix = bd.loadSimple(0).replace(number, ""); // tira o inicio, para ter a parte a ser somada 1.

                    sufix = QString::number(sufix.toInt() + 1);

                    number += sufix;

                }else{
                    number += "1";
                }
            }

            bd.exec("update configorder set currentnumber = '" + number + "'");
        }else{
            number = "1";

            QList<QString> fields;
            QList<QString> values;

            fields << "typenumbering" << "currentnumber";
            values << "p" << "1";

            bd.insert("configorder", fields, values);
        }

        sql += "'" + number + "')"; // numero do pedido, ver esquema depois direito

        if ( ! bd.exec(sql)){
            qDebug() << "[order] [insertOrder] erro na sql" << sql;
            return false;
        }

        if (! sqlEmployeeOrder.isEmpty())
            bd.exec(sqlEmployeeOrder);

        if (status == "Fechado" || isPaid){

            // se for pago, ele irá considerar o pedido como fechado, para facilitar no fechamento de caixa.

            sql = QString("insert into datecloseorder values('%1','%2','%3')").arg(idOrder, tools.currentYear() + "-" + tools.currentMonth2() + "-" + tools.currentDay(), tools.currentTime());

            bd.exec(sql);

        }
    }

    QString sql;

    QList<QString> idSizes;

    for (int i = 0; i < orderSizes.length(); i++){

        if (! bd.search("select idsize from size where name = '" + orderSizes.value(i) + "'")){
            qDebug() << "[order] [insertOrder] erro na sql" << sql;
            return false;
        }

        idSizes << bd.loadSimple(0);
    }

    for (int i = 0; i < orderQuantity.length(); i++){
        sql = "insert into itemorder values(";
        sql += idOrder + ", ";
        sql += QString::number(i + 1) + ", ";
        sql += idSizes.value(i) + ", ";
        sql += QString::number(orderValues.value(i)) + ", ";
        sql += QString::number(orderQuantity.value(i)) + ", ";
        sql += "'" + orderNoteProduct.value(i) + "')";

        if (! bd.exec(sql)){
            qDebug() << "[order] [insertOrder] erro na sql" << sql;
            return false;
        }

        for (int j = 0; j < orderIdProduct.value(i).length(); j++){
            sql = "insert into productitem values (";
            sql += idOrder + ", ";
            sql += QString::number(i + 1) + ", ";
            sql += orderIdProduct.value(i).value(j) + ", ";
            sql += QString::number(orderPartProduct.value(i).value(j)) + ", ";

            if (bd.search("select idsize from size where name = '" + orderSizes.value(i) + "'"))
                sql += bd.loadSimple(0);

            sql += ")"; // idsize aqui

            if (! bd.exec(sql)){
                qDebug() << "[order] [insertOrder] erro na sql" << sql;
                return false;
            }
        }
    }

    if (orderPaymentForm.length() > 0){
        for (int i = 0; i < orderPaymentForm.length(); i++){
            sql = "insert into paymentorder values (";
            sql += idOrder + ", ";
            sql += "'" + orderPaymentForm.value(i) + "', ";
            sql += orderPaymentFormValue.value(i) + ")";

            if (! bd.exec(sql)){
                qDebug() << "[order] [insertOrder] erro na sql" << sql;
                return false;
            }
        }
    }else{
        sql = "insert into paymentorder values (";
        sql += idOrder + ", ";
        sql += "'Dinheiro', ";
        sql += QString::number(valuePaid) + ")";

        if (! bd.exec(sql)){
            qDebug() << "[order] [insertOrder] erro na sql" << sql;
            return false;
        }
    }

    bd.exec("select * from ordered");

    return true;
}

bool Order::insertShedule(){
    QString sql = "insert into schedule values(";
    sql += idOrder;
    sql += ", '" + tools.convertDate(dateSchedule) + "', ";
    sql += "'" + hourSchedule + "')";

    if (! bd.exec(sql)){
        qDebug() << "[order] [insertShedule] erro na sql" << sql;
        return false;
    }else{
        // cria o aviso
        int idAlert = bd.generateID("alert", "idAlert");

        int minutes = 60; // minutos para o alerta (60 minutos é o padrao, caso isso não tenha sido configurado;

        if (bd.search("select daysAlertOrder from configalert")){
            minutes = bd.loadSimple(0).toInt();
        }

        QList<QString> values;

        values << QString::number(idAlert) << tr("Pedido") << tr("Agendamento de pedido") << dateSchedule << hourSchedule << tools.subDate(dateSchedule, hourSchedule, minutes).date().toString("dd/MM/yyyy") << tools.subDate(dateSchedule, hourSchedule, minutes).time().toString("hh:mm") << tr("Alta") << "0" << "Pedido número: ";

        bd.insert("alert", values);

        values.clear();

        values << idOrder << QString::number(idAlert);

        bd.insert("schedulealert", values);
    }

    //altera o tipo do pedido para agendado

    if (! bd.update("ordered", "statusorder", "Agendado", "idorder", idOrder))
        return false;

    return true;
}

bool Order::insertClient(){
    if (typeClient == 1){
        QList<QString> values;

        values << QString::number(bd.generateID("client","idClient"));
        values << name << phone << street << number << complement << district << cep << QString::number(shipping) << note;

        bool insert = bd.insert("client",values);

        if (insert)
            bd.refreshModel("client");

        return insert;
    }else{
        QList<QString> values;

        values << QString::number(bd.generateID("clientBusiness","idClient"));
        values << nameRespondible << corporateName << fantasyName << CNPJ << phone << street << number << complement << district << cep << QString::number(shipping) << note;

        bool insert = bd.insert("clientBusiness",values);

        if (insert)
            bd.refreshModel("clientBusiness");

        return insert;
    }
}

int Order::nextIdClient(){
    if (typeClient == 1)
        return bd.generateID("client", "idClient");
    else
        return bd.generateID("clientBusiness","idClient");
}

bool Order::updateClient(QList<QString> data)
{
    int id = data.value(0).toInt();

    data.removeAt(0);

    QList<QString> fields;

    if (typeClient == 1){

        fields << "name" << "phone" << "street" << "nhouse" << "complement" << "district" << "cep" << "shipping" << "note";

        return bd.update("client", fields, data, "idClient", QString::number(id));

    }else{

        fields << "namerespondible" << "corporatename" << "fantasyname" << "cnpj" << "phone" << "street" << "nhouse" << "complement" << "district" << "cep" << "shipping" << "note";

        return bd.update("clientbusiness", fields, data, "idClient", QString::number(id));

    }
}

void Order::loadDataOrder(int idOrder)
{
    QString idS = QString::number(idOrder);

    this->idOrder = idS;

    bool canProceed = false; // para saber se pode prosseguir nas etapas seguintes do load.
    if (bd.search("select * from ordered where idorder = " + idS)){
        canProceed = true;

        typeClient = bd.loadSimple(1).toInt();
        idClient = bd.loadSimple(2).toInt();
        typeOrder = bd.loadSimple(5).toInt();
        totalValueOrderProducts = bd.loadSimple(6).toDouble();
        valueShipping = bd.loadSimple(7).toDouble();
        valuePaid = bd.loadSimple(8).toDouble();
        number = bd.loadSimple(11);

        // pegando o telefone do cliente
        if(typeClient == 1){
            if (bd.search("select phone from client where idclient = " + QString::number(idClient), 1)){

                phoneClient = bd.loadSimple(0, 1);

                client->loadData(idClient);
            }

        }else if(typeClient == 2){
            if (bd.search("select phonebusiness from client where idclient = " + QString::number(idClient), 1)){

                phoneClient = bd.loadSimple(0, 1);

                client->loadDataBusiness(idClient);
            }
        }

        if (bd.loadSimple(9) == "true")
            isPaid = true;
        else
            isPaid = false;
    }


    if (canProceed){
        if (bd.search("select * from schedule where idorder = " + idS)){
            dateSchedule = tools.convertDate2(bd.loadSimple(1));

            hourSchedule = bd.loadSimple(2);
        }else{
            dateSchedule.clear();

            hourSchedule.clear();
        }
    }

    orderSizes.clear();
    orderValues.clear();
    orderQuantity.clear();
    orderNoteProduct.clear();
    orderIdProduct.clear();
    orderNameProduct.clear();
    orderPartProduct.clear();

    QString sql;

    if (canProceed){

        QString sql = QString("select * from itemorder where idorder = %1").arg(idS);

        qDebug() << "SQL 1" << sql;

        if (bd.search(sql)){
            for (int i = 0; i < bd.modelSearch[0]->rowCount(); i++){
                // load de itens do pedido

                sql = QString("select name from size where idsize = %1").arg(bd.loadLine(i).value(2));

                qDebug() << "SQL 2" << sql;

                if (bd.search(sql, 1))
                    orderSizes << bd.loadSimple(0, 1);

                orderValues << bd.loadLine(i).value(3).toDouble();
                orderQuantity << bd.loadLine(i).value(4).toInt();
                orderNoteProduct << bd.loadLine(i).value(5);

                sql = QString("select * from productitem where idorder = %1 and iditem = %2").arg(idS, bd.loadLine(i).value(1));

                qDebug() << "SQL 3" << sql;

                if (bd.search(sql, 1)){
                    QList<int> part;
                    QList<QString> id;
                    QList<QString> name;

                    for (int i = 0; i < bd.modelSearch[1]->rowCount(); i++){
                        id << bd.loadLine(i, 1).value(2);
                        part << bd.loadLine(i, 1).value(3).toInt();
                    }

                    qDebug() << "ids" << id;

                    for (int i = 0; i < id.length(); i++){

                        sql = QString("select name from product where idProduct = %1").arg(id.value(i));

                        qDebug() << "SQL 4" << sql;

                        if (bd.search(sql, 2)){
                            name << bd.loadSimple(0, 2);
                        }else{
                            qDebug() << "[ORDER] [loadDataOrder] Produto de ID" << id.value(i) << "não encontrado";
                        }
                    }

                    orderPartProduct << part;

                    orderIdProduct << id;

                    orderNameProduct << name;
                }
            }
        }else
            canProceed = false;
    }

    if (canProceed){
        sql = QString("select * from paymentorder where idorder = %1").arg(idS);

        qDebug() << "SQL 5" << sql;

        if (bd.search(sql)){
            orderPaymentForm = bd.loadColumn(1);
            orderPaymentFormValue = bd.loadColumn(2);
        }
    }

    emit loadComplete();
}

QList<QString> Order::paymentTypes()
{
    QList<QString> listEmpty;

    listEmpty << "Nenhuma forma cadastrada";

    if (bd.search("select * from paymenttypes"))
        return bd.loadColumn(1);
    else
        return listEmpty;
}

QList<QList<QString> > Order::bartenders(){
    QList<QList<QString> > listReturn;

    if (! bd.search("select idemployee, name from employee where jobrole = 'Garçom';")){
        listReturn.append(QList<QString>());
        listReturn.append(QList<QString>());

        return listReturn;
    }

    listReturn.append(bd.loadColumn(0));
    listReturn.append(bd.loadColumn(1));

    listBartenders = listReturn;

    return listReturn;
}

int Order::totalOrders()
{
    QString sql = "select count(*) from ordered";

    if (bd.search(sql))
        return bd.loadSimple(0).toInt();

    return 0;
}

void Order::setIdSelectedBartender(int idSelectedBartender)
{
    if (idSelectedBartender != 0)
        _idSelectedBartender = idSelectedBartender;
//        _idSelectedBartender = listBartenders[0][idSelectedBartender - 1].toInt();
}
