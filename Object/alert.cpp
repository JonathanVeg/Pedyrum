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


 #include "alert.h"
#include <QDebug>

Alert::Alert(QObject *parent) :
    QObject(parent)
{
}

void Alert::start(QSqlDatabase *sdb){

    bd.sdb = *sdb;

    areFilterTime = false;

    areFilterType = false;

    instance();

    verifyAlert();
}

bool Alert::instance()
{
    QList<QString> headers, headers2;

    headers << tr("ID");
    headers << tr("Título");
    headers << tr("Assunto");
    headers << tr("Data");
    headers << tr("Hora");
    headers << tr("Data do alerta");
    headers << tr("Hora do alerta");
    headers << tr("Prioridade");
    headers << tr("Alertado");
    headers << tr("Anotação");

    headers2 << tr("ID");
    headers2 << tr("Data");
    headers2 << tr("Tarefa");

    if (bd.createModel("alert", headers) && bd.createModel("taskSimple", headers2)){
        // como a opção padrão são os alertas do dia, ele já mostrará os mesmos dessa forma

        areFilterTime = true;

        filterTime = "dateEvent = '" + tools.getDate(Qt::SystemLocaleShortDate) + "'";

        setFilter();

        emit count(bd.getModel("alert")->rowCount());

        if (bd.createCompleter("alertTitle", "alert", 0, false, QCompleter::PopupCompletion)){
            if (bd.createCompleter("alertSubject", "alert", 1, false, QCompleter::PopupCompletion))
                return true;
            else{
                qDebug() << "[alert] [instance]" << "Completer alertSubject não criado";

                return false;
            }
        }else{
            qDebug() << "[alert] [instance]" << "Completer alertTitle não criado";

            return false;
        }
    }else{
        qDebug() << "[alert] [instance]" << "Model de alerta e/ou de tarefas não criado";

        return false;
    }
}

bool Alert::insertTask()
{
    QList<QString> values;

    values << QString::number(bd.generateID("taskSimple", "idTask")) << tools.getDate(Qt::SystemLocaleShortDate) << task;

    return bd.insert("taskSimple", values);
}

bool Alert::updateTask(int id)
{
    QList<QString> fields, values;

    fields << "dateTask" << "task";

    values << tools.getDate(Qt::SystemLocaleShortDate) << task;

    return bd.update("taskSimple", fields, values, "idTask", QString::number(id));
}

bool Alert::removeTask(int id)
{
    return bd.remove("taskSimple", "idtask", QString::number(id));
}

bool Alert::insert()
{
    QList<QString> values;

    values << QString::number(bd.generateID("alert", "idAlert")) << title << subject << date << hour << dateAlert << hourAlert << priority << alerted << note;

    return bd.insert("alert", values);
}


bool Alert::update(int id)
{
    QList<QString> fields, values;

    fields << "title" << "subject" << "dateEvent" << "hourEvent" << "dateAlert" << "hourAlert" << "priority" << "alerted" << "note";

    values << title << subject << date << hour << dateAlert << hourAlert << priority << alerted << note;

    return bd.update("alert", fields, values, "idAlert", QString::number(id));
}

bool Alert::remove(int id)
{
    QString idS = QString::number(id);

    bd.remove("schedulealert", "idalert", idS);

    return bd.remove("alert", "idalert", idS);
}

void Alert::clearData()
{
    title.clear();
    subject.clear();
    date.clear();
    hour.clear();
    dateAlert.clear();
    hourAlert.clear();
    priority.clear();
    alerted = "0";
    note.clear();

    task.clear();
}

void Alert::getCount()
{
    emit count(bd.getModel("alert")->rowCount());
}

void Alert::setFilter()
{
    QString filter = "";

    if (areFilterTime){
        filter += filterTime;

        if (areFilterType){
            filter += " and " + filterType;
        }
    }else if (areFilterType)
        filter += filterType;

    bd.getModel("alert")->setFilter(filter);

    emit count(bd.getModel("alert")->rowCount());
}

void Alert::setFilter(QString filter){
    bd.getModel("alert")->setFilter(filter);

    emit count(bd.getModel("alert")->rowCount());
}

void Alert::clearFilter()
{
    bd.getModel("alert")->setFilter("");

    emit count(bd.getModel("alert")->rowCount());
}

void Alert::getAlertsDays(){
    areFilterTime = true;

    dateIFilter = tools.getDate();
    dateFFilter.clear();

    filterTime = "dateEvent = '" + tools.getDate(Qt::SystemLocaleShortDate) + "'";

    setFilter();

    emit count(bd.getModel("alert")->rowCount());
}

void Alert::getAlertsWeek()
{
    areFilterTime = true;
    QString dateI = tools.getDayBeginWeek(tools.getDate(Qt::SystemLocaleShortDate));
    QString dateF = tools.getDayEndWeek(tools.getDate(Qt::SystemLocaleShortDate));

    dateIFilter = dateI;
    dateFFilter = dateF;

    bd.search("select idalert, dateevent from alert");

    QList<QString> validIds; // ids dos alerta da semana

    QList<QString> dates = bd.loadColumn(1);
    QList<QString> ids = bd.loadColumn(0);

    for (int i = 0; i < dates.length(); i++){
        if (tools.betweenDates(dateI, dateF, dates[i]))
            validIds << ids[i];
    }

    QString filter = "";

    if (validIds.length() == 0){
        setFilter();

        return;
    }

    filter = "(";

    for (int i = 0; i < validIds.length(); i++)
        filter += "idalert = '" + validIds[i] + "' or ";

    filter = filter.mid(0, filter.length() - 3) + ")";

    setFilter();

    emit count(bd.getModel("alert")->rowCount());
}

void Alert::getAlertsMonth()
{

    areFilterTime = true;

    QString dateI = "01" + tools.getDate(Qt::SystemLocaleShortDate).mid(2);
    QString dateF = tools.daysInMonth(tools.getDate(Qt::SystemLocaleShortDate)) + tools.getDate(Qt::SystemLocaleShortDate).mid(2);

    dateIFilter = dateI;
    dateFFilter = dateF;

    bd.search("select idalert, dateevent from alert");

    QList<QString> validIds; // ids dos alerta da semana

    QList<QString> dates = bd.loadColumn(1);
    QList<QString> ids = bd.loadColumn(0);

    for (int i = 0; i < dates.length(); i++){
        if (tools.betweenDates(dateI, dateF, dates[i]))
            validIds << ids[i];
    }

    QString filter = "";

    if (validIds.length() == 0){
        setFilter();

        return;
    }

    filter = "(";
    for (int i = 0; i < validIds.length(); i++)
        filter += "idalert = '" + validIds[i] + "' or ";

    filter = filter.mid(0, filter.length() - 3) + ")";

    filterTime = filter;

    setFilter();

    emit count(bd.getModel("alert")->rowCount());
}

void Alert::getAlertsYear()
{
    areFilterTime = true;

    QString dateI = "01/01/" + tools.getDate(Qt::SystemLocaleShortDate).mid(6);
    QString dateF = "31/12/" + tools.getDate(Qt::SystemLocaleShortDate).mid(6);

    dateIFilter = dateI;
    dateFFilter = dateF;

    bd.search("select idalert, dateevent from alert");

    QList<QString> validIds; // ids dos alerta da semana

    QList<QString> dates = bd.loadColumn(1);
    QList<QString> ids = bd.loadColumn(0);

    for (int i = 0; i < dates.length(); i++){
        if (tools.betweenDates(dateI, dateF, dates[i]))
            validIds << ids[i];
    }

    ids << validIds;

    QString filter = "";

    if (validIds.length() == 0){
        setFilter();

        return;
    }

    filter += "(";

    for (int i = 0; i < validIds.length(); i++)
        filter += "idalert = '" + validIds[i] + "' or ";

    filter = filter.mid(0, filter.length() - 3) + ")";

    filterTime = filter;

    setFilter();

    emit count(bd.getModel("alert")->rowCount());
}

void Alert::getAlertsCustom(QString dateI, QString dateF)
{
    areFilterTime = true;

    dateIFilter = dateI;
    dateFFilter = dateF;

    bd.search("select idalert, dateevent from alert");

    QList<QString> validIds; // ids dos alerta da semana

    QList<QString> dates = bd.loadColumn(1);
    QList<QString> ids = bd.loadColumn(0);

    for (int i = 0; i < dates.length(); i++){
        if (tools.betweenDates(dateI, dateF, dates[i]))
            validIds << ids[i];
    }

    ids << validIds;

    QString filter = "";

    if (validIds.length() == 0){
        setFilter();

        return;
    }

    filter += "(";

    for (int i = 0; i < validIds.length(); i++)
        filter += "idalert = '" + validIds[i] + "' or ";

    filter = filter.mid(0, filter.length() - 3) + ")";

    filterTime = filter;

    setFilter();

    emit count(bd.getModel("alert")->rowCount());
}

void Alert::refreshModels()
{
    bd.refreshModel("alert");
    bd.refreshModel("taskSimple");

}

void Alert::verifyAlert()
{
    QString date = tools.getDate();
    QString time = tools.getTime("hh:mm");

    QString sql = QString("select * from alert where datealert = '%1' and houralert = '%2'").arg(date, time);

    bool newAlert = false;

    if (bd.search(sql)){
        int qntLines = bd.loadColumn(0).length();
        for (int i = 0; i < qntLines; i++){

            // muda o status do alerta de 0 que é "inativo" para 1, que é
            // "ativo", mas não lido ainda

            newAlert = true;
            bd.update("alert", "alerted", "1", "idalert", bd.loadColumn(0)[i]);
        }

        if (qntLines > 1){
            messageObject.setTitle(tr("Novos Alertas"));

            messageObject.setText(tr("Você tem ") + QString::number(qntLines) + tr(" alertas novos."));

            messageObject.setImage(":/Extras/message-alert");

            messageObject.setDelay(5000);
        }else{
            messageObject.setTitle(tr("Novo Alerta"));

            if (bd.loadSimple(1).length() >= 55)
                messageObject.setText(bd.loadSimple(1).mid(0, 52) + "...");
            else
                messageObject.setText(bd.loadSimple(1));

            messageObject.setImage(":/Extras/message-alert");

            messageObject.setDelay(5000);
        }

        emit newAlertMessage(&messageObject);
    }

    // emit a quantidade de alertas novos
    if (bd.search("select count(*) from alert where alerted = '1'"))
        emit newsAlerts(bd.loadSimple(0).toInt(), newAlert);
    else
        emit newsAlerts(0, false);

    refreshModels();
}

void Alert::getAlertsNews(bool news)
{
    filterType = "alerted = '1'";

    if (news)
        areFilterType = true;
    else
        areFilterType = false;

    setFilter();
}

void Alert::alterStatus(QString idAlert, QString newStatus)
{
    bd.update("alert", "alerted", newStatus, "idalert", idAlert);

    refreshModels();
}
