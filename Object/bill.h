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


 #ifndef BILL_H
#define BILL_H

#include <QObject>
#include <Abstract/bdtools.h>
#include <Abstract/tools.h>

class Bill : public QObject
{
    Q_OBJECT
public:
    explicit Bill(QObject *parent = 0);

    void start(QSqlDatabase *sdb);

public:
    int idBill;
    QString name;
    float value;
    QString type;
    QString status;
    QString dateExpiration;
    QString datePayment;
    QString note;


    bool loadData(int id);

    bool instance();

    bool insert(); // ele pega todos os dados da classe e faz o insert no BD
    bool update(int id); // manda o id aqui, ele faz o load com tds os dados
    bool remove(int id);

    QString countBills();

    void emitCountBills();

    void getTotalValue();

    void setFilter();
    void setFilter(QString filter);
    void setFilterTime(QString dateI, QString dateF);

    void clearFilter();

    bool areFilterTime;
    QString filterTime;
    void getBillsDays();
    void getBillsWeek();
    void getBillsMonth();
    void getBillsYear();
    void getBillsCustom(QString dateI, QString dateF);

    bool areFilterType;
    QString filterType;

protected:

signals:
    void count(QString);
    void totalValue(QString);

public:
    BDTools bd;
    Tools tools;

    QString dateIFilter;
    QString dateFFilter;

public slots:
    void refreshModel();

};

#endif // BILL_H
