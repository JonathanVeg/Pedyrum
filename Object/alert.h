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


 #ifndef ALERT_H
#define ALERT_H

#include <QObject>
#include <Abstract/bdtools.h>
#include <Abstract/tools.h>
#include <Object/messageobject.h>

class Alert : public QObject
{
    Q_OBJECT
public:
    explicit Alert(QObject *parent = 0);

    void start(QSqlDatabase *sdb);

    bool instance();

    void getCount();

    // dados dos alertas
    QString title;
    QString subject;
    QString date;
    QString hour;
    QString dateAlert;
    QString hourAlert;
    QString priority;
    QString alerted;
    QString note;

    // dados das tarefas
    QString task;

    bool insertTask();
    bool updateTask(int id);
    bool removeTask(int id);

    bool insert();
    bool update(int id);
    bool remove(int id);

    void setFilter();

    void setFilter(QString filter);

    void clearFilter();

    bool areFilterTime; // mostra se há algum tipo de filtro por data ativo.
    QString filterTime; // armazena esse filtro.
    void getAlertsDays(); // pega os alertas da semana
    void getAlertsWeek(); // pega os alertas da semana
    void getAlertsMonth(); // pega os alertas do mes
    void getAlertsYear(); // pega os alertas do ano
    void getAlertsCustom(QString dateI, QString dateF); // pega os alertas do ano

    bool areFilterType; // verifica se tem filtro de tipo (novo ou não) ativo.
    QString filterType;
    void getAlertsNews(bool news);

    void clearData();

    void verifyAlert();

    void alterStatus(QString idAlert, QString newStatus);

    BDTools bd;

    Tools tools;

    MessageObject messageObject;

    QString dateIFilter;
    QString dateFFilter;

signals:
    void count(int);

    void newsAlerts(int, bool);

    void newAlertMessage(MessageObject *messageObject);

public slots:
    void refreshModels();

};

#endif // ALERT_H
