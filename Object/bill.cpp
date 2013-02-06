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


 #include "bill.h"
#include <QDebug>

Bill::Bill(QObject *parent) :
    QObject(parent)
{
}

void Bill::start(QSqlDatabase *sdb){
    bd.sdb = *sdb;

    emitCountBills();

    instance();
}

bool Bill::instance(){
    QList<QString> headers;

    headers << tr("ID") << tr("Nome") << tr("Valor") << tr("Tipo") << tr("Status") << tr("Data de vencimento") << tr("Data de pagamento") << tr("Anotação");

    if (bd.createModel("bill", headers))
        return true;

    return false;
}

bool Bill::loadData(int id){
    this->idBill = id;

    if (bd.search("select * from bill where idbill = " + QString::number(id))){
        QList<QString> data = bd.loadLine(0);

        name = (data[1]);
        value = (data[3].toDouble());
        type = (data[4]);
        status = data[5];
        dateExpiration = tools.convertDate2(data[6]);
        datePayment = tools.convertDate2(data[7]);
        note = (data[8]);

        return true;
    }

    return false;
}

QString Bill::countBills()
{
    return QString::number(bd.countRecord("bill"));
}

void Bill::refreshModel()
{
    bd.refreshModel("bill");        
}

void Bill::setFilter(QString filter)
{
    bd.getModel("bill")->setFilter(filter);
    refreshModel();
}

void Bill::setFilterTime(QString dateI, QString dateF){
    bd.search("select idbill, dateexpiration from bill");

    QList<QString> validIds; // ids dos alerta da semana

    QList<QString> dates = bd.loadColumn(1);
    QList<QString> ids = bd.loadColumn(0);

    for (int i = 0; i < dates.length(); i++){
        if (tools.betweenDates(dateI, dateF, tools.convertDate2(dates[i]))){
            validIds << ids[i];
        }
    }

    if (validIds.length() == 0){
        areFilterTime = false;

        setFilter();

        return;
    }

    areFilterTime = true;

    filterTime = "(";

    for (int i = 0; i < validIds.length(); i++)
        filterTime += "idbill = '" + validIds[i] + "' or ";

    filterTime = filterTime.mid(0, filterTime.length() - 3) + ")";

    setFilter();

    emit count(QString::number(bd.getModel("bill")->rowCount()));
}

void Bill::setFilter(){
    QString filter = "";

    if (areFilterTime){
        filter += filterTime;

        if (areFilterType){
            filter += " and " + filterType;
        }
    }else if (areFilterType)
        filter += filterType;

    bd.getModel("bill")->setFilter(filter);

    emit count(QString::number(bd.getModel("bill")->rowCount()));
}

void Bill::clearFilter()
{
    bd.getModel("bill")->setFilter("");
    refreshModel();
}

void Bill::emitCountBills(){
    emit count(QString::number(bd.countRecord("bill")));
}

bool Bill::insert()
{
    QList<QString> values;
    values << QString::number(bd.generateID("bill", "idbill"));
    values << name << QString::number(value) << type << status;
    values << tools.convertDate(dateExpiration) << tools.convertDate(datePayment) << note;

    bool insert = bd.insert("bill", values);
    if (insert){
        emitCountBills();

        refreshModel();

        getTotalValue();
    }

    return insert;
}

bool Bill::update(int id)
{
    QList<QString> fields;
    QList<QString> values;

    values << name << QString::number(value) << type << status;
    values << tools.convertDate(dateExpiration) << tools.convertDate(datePayment) << note;

    fields << "name" << "valuebill" << "typebill" << "status" << "dateexpiration" << "datepayment" << "note";

    bool update = bd.update("bill", fields, values, "idbill", QString::number(id));

    if (update){
        refreshModel();
        getTotalValue();
    }


    return update;
}

bool Bill::remove(int id)
{
    bool remove = bd.exec("delete from bill where idbill = " + QString::number(id));

    if (remove){
        refreshModel();

        emitCountBills();

        getTotalValue();
    }

    return remove;
}

void Bill::getTotalValue()
{
    QString sql;

    sql = "select sum(valuebill) from bill where lower(typebill) = 'entrada'";

    float total;
    if (bd.search(sql))
        total = bd.loadSimple(0).toFloat();
    else
        total = 0;

    sql = "select sum(valuebill) from bill where lower(typebill) = 'saída'";

    if (bd.search(sql))
        total -= bd.loadSimple(0).toFloat();

    emit totalValue(QString::number(total));
}

void Bill::getBillsDays()
{
    areFilterTime = true;

    dateIFilter = tools.getDate();
    dateFFilter.clear();

    filterTime = "dateexpiration = '" + tools.convertDate(tools.getDate(), "-") + "'";

    setFilter();

    emit count(QString::number(bd.getModel("bill")->rowCount()));
}

void Bill::getBillsWeek()
{
    areFilterTime = true;

    QString dateI = tools.getDayBeginWeek(tools.getDate(Qt::SystemLocaleShortDate));
    QString dateF = tools.getDayEndWeek(tools.getDate(Qt::SystemLocaleShortDate));

    dateIFilter = dateI;
    dateFFilter = dateF;

    setFilterTime(dateI, dateF);
}


void Bill::getBillsMonth()
{
    areFilterTime = true;

    QString dateI = "01" + tools.getDate(Qt::SystemLocaleShortDate).mid(2);
    QString dateF = tools.daysInMonth(tools.getDate(Qt::SystemLocaleShortDate)) + tools.getDate(Qt::SystemLocaleShortDate).mid(2);

    dateIFilter = dateI;
    dateFFilter = dateF;

    setFilterTime(dateI, dateF);
}

void Bill::getBillsYear()
{
    areFilterTime = true;

    QString dateI = "01/01/" + tools.getDate(Qt::SystemLocaleShortDate).mid(6);
    QString dateF = "31/12/" + tools.getDate(Qt::SystemLocaleShortDate).mid(6);

    dateIFilter = dateI;
    dateFFilter = dateF;

    setFilterTime(dateI, dateF);
}

void Bill::getBillsCustom(QString dateI, QString dateF){
    areFilterTime = true;

    dateIFilter = dateI;
    dateFFilter = dateF;

    bd.search("select idbill, dateexpiration from bill");

    QList<QString> validIds; // ids dos alerta da semana

    QList<QString> dates = bd.loadColumn(1);
    QList<QString> ids = bd.loadColumn(0);

    for (int i = 0; i < dates.length(); i++){
        if (tools.betweenDates(dateI, dateF, tools.convertDate2(dates[i]))){
            validIds << ids[i];
        }
    }

    if (validIds.length() == 0){
        areFilterTime = false;

        setFilter();

        return;
    }

    areFilterTime = true;

    filterTime = "(";

    for (int i = 0; i < validIds.length(); i++)
        filterTime += "idbill = '" + validIds[i] + "' or ";

    filterTime = filterTime.mid(0, filterTime.length() - 3) + ")";

    setFilter();

    emit count(QString::number(bd.getModel("bill")->rowCount()));
}
