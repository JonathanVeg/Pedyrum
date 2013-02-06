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


 #include "movementcash.h"

MovementCash::MovementCash(QObject *parent) :
    QObject(parent)
{
    areFilterTime = true;
    areFilterType = false;
}

void MovementCash::start(QSqlDatabase *sdb){
    bd.sdb = *sdb;

    emitCountMovementCash();

    instance();

    getCashDays();
}

bool MovementCash::instance()
{
    QList<QString> headers;

    headers << tr("ID") << tr("Valor") << tr("Tipo") << tr("Data") << tr("Hora") << tr("Anotação");

    if (bd.createModel("movementCash", headers)){
        bd.getModel("movementCash")->setSort(0, Qt::AscendingOrder); // organiza pelo ID

        getTotalValue();

        return true;
    }

    return false;
}

bool MovementCash::insert()
{
    QList<QString> values;

    values << QString::number(bd.generateID("movementCash", "idMovement"));
    values << QString::number(value) << type << tools.convertDate(date) << hour << note;

    bool insert = bd.insert("movementCash", values);

    if (insert){
        emitCountMovementCash();

        refreshModel();

        getTotalValue();
    }

    return insert;
}

bool MovementCash::update(int id)
{
    QList<QString> fields, values;

    fields << "valuemovement" << "typemovement" << "datemovement" << "hourmovement" << "note";

    values << QString::number(value) << type << tools.convertDate(date) << hour << note;

    bool update = bd.update("movementCash", fields, values, "idmovement", QString::number(id));

    if (update){
        emitCountMovementCash();

        refreshModel();

        getTotalValue();
    }

    return update;
}

bool MovementCash::remove(int id)
{
    bool remove = bd.exec("delete from movementcash where idmovement = " + QString::number(id));

    if (remove){
        refreshModel();

        emitCountMovementCash();

        getTotalValue();
    }

    return remove;
}

QString MovementCash::countMovementCash()
{
    return QString::number(bd.countRecord("movementCash"));
}

void MovementCash::emitCountMovementCash()
{
    emit count(QString::number(bd.countRecord("movementCash")));
}

void MovementCash::getTotalValue()
{
    double total = 0;

    QString date = tools.currentYear() + "-" + tools.currentMonth2() + "-" + tools.currentDay();

    total = valueCloseBox(date, tools.currentTime()).toDouble();


    if (type == "Fechamento"){
        emit totalValue(0);
    }else{
        emit totalValue(QString::number(total));
    }
}

void MovementCash::setFilter(QString filter)
{
    bd.getModel("movementCash")->setFilter(filter);

    refreshModel();
}

void MovementCash::setFilter(){
    QString filter = "";

    if (areFilterTime){
        filter += filterTime;

        if (areFilterType){
            filter += " and " + filterType;
        }
    }else if (areFilterType)
        filter += filterType;

    bd.getModel("movementCash")->setFilter(filter);

    getTotalValue();

    emit count(QString::number(bd.getModel("movementCash")->rowCount()));
}

void MovementCash::setFilterTime(QString dateI, QString dateF){
    bd.search("select idmovement, datemovement from movementcash");

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
        filterTime += "idmovement = '" + validIds[i] + "' or ";

    filterTime = filterTime.mid(0, filterTime.length() - 3) + ")";

    setFilter();

    emit count(QString::number(bd.getModel("movementCash")->rowCount()));
}

void MovementCash::clearFilter()
{
    bd.getModel("movementCash")->setFilter("");

    refreshModel();
}

void MovementCash::refreshModel()
{
    bd.refreshModel("movementCash");
}

bool MovementCash::canOpenCash()
{
    if (bd.search("select * from movementcash where typemovement = 'Abertura' or typemovement = 'Fechamento' order by datemovement desc, hourmovement desc;")){
        qDebug() << bd.loadSimple(2);
        if (bd.loadSimple(2) != "Fechamento")
            return false;

        return true;
    }

    return true;
}

bool MovementCash::canCloseCash()
{
    //    if (bd.search("select * from movementcash where typemovement = 'Abertura' or typemovement = 'Fechamento' order by datemovement desc, hourmovement desc;")){

    if (bd.search("select * from movementcash where typemovement = 'Abertura' or typemovement = 'Fechamento' order by idmovement desc;")){
        if (bd.loadSimple(2) == "Fechamento")
            return false;

        return true;
    }

    return false;
}

QString MovementCash::valueCloseBox(QString date, QString time)
{
    QString dateLastOpen;
    QString hourLastOpen;

    // pressupõe aqui que o fechamento pode ser executado sem problemas
    QString sql = "select datemovement, hourmovement from movementcash where typemovement = 'Abertura' order by datemovement desc, hourmovement desc";

    // esse if é por conciencia, pq se ele ta fechando o caixa, busca por abertura com certeza retornará algo.
    if (bd.search(sql)){
        dateLastOpen = bd.loadSimple(0);
        hourLastOpen = bd.loadSimple(1);
    }

    sql = QString("select closebox('%1','%2','%3','%4')").arg(dateLastOpen, hourLastOpen, date, time);

    double valueClose = 0;

    if (bd.search(sql))
        valueClose = bd.loadSimple(0).toDouble();

    return QString::number(valueClose);
}

void MovementCash::getCashDays()
{
    areFilterTime = true;

    dateIFilter = tools.getDate();
    dateFFilter.clear();

    filterTime = "datemovement = '" + tools.convertDate(tools.getDate(), "-") + "'";

    setFilter();

    getTotalValue();

    emit count(QString::number(bd.getModel("movementCash")->rowCount()));
}

void MovementCash::getCashWeek()
{
    areFilterTime = true;

    QString dateI = tools.getDayBeginWeek(tools.getDate(Qt::SystemLocaleShortDate));
    QString dateF = tools.getDayEndWeek(tools.getDate(Qt::SystemLocaleShortDate));

    dateIFilter = dateI;
    dateFFilter = dateF;

    setFilterTime(dateI, dateF);
}

void MovementCash::getCashMonth()
{
    areFilterTime = true;

    QString dateI = "01" + tools.getDate(Qt::SystemLocaleShortDate).mid(2);
    QString dateF = tools.daysInMonth(tools.getDate(Qt::SystemLocaleShortDate)) + tools.getDate(Qt::SystemLocaleShortDate).mid(2);

    dateIFilter = dateI;
    dateFFilter = dateF;

    setFilterTime(dateI, dateF);
}

void MovementCash::getCashYear()
{
    areFilterTime = true;

    QString dateI = "01/01/" + tools.getDate(Qt::SystemLocaleShortDate).mid(6);
    QString dateF = "31/12/" + tools.getDate(Qt::SystemLocaleShortDate).mid(6);

    dateIFilter = dateI;
    dateFFilter = dateF;

    setFilterTime(dateI, dateF);
}

void MovementCash::getCashCustom(QString dateI, QString dateF)
{
    bd.search("select idmovement, datemovement from movementcash");

    dateIFilter = dateI;
    dateFFilter = dateF;

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
        filterTime += "idmovement = '" + validIds[i] + "' or ";

    filterTime = filterTime.mid(0, filterTime.length() - 3) + ")";

    setFilter();

    emit count(QString::number(bd.getModel("movementCash")->rowCount()));
}

QSqlTableModel * MovementCash::model()
{
    return bd.getModel("movementCash");
}
