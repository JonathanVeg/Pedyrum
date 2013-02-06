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


 #ifndef MOVEMENTCASH_H
#define MOVEMENTCASH_H

#include <QObject>
#include <Abstract/bdtools.h>
#include <Abstract/tools.h>

class MovementCash : public QObject
{
    Q_OBJECT
public:
    explicit MovementCash(QObject *parent = 0);

    bool instance();

    void start(QSqlDatabase *sdb);

    bool insert();
    bool update(int id);
    bool remove(int id);

    QString countMovementCash();

    void emitCountMovementCash();

    void getTotalValue();

    void clearFilter();

    bool canOpenCash();

    bool canCloseCash();

    QString valueCloseBox(QString date, QString time);

    Tools tools;

    void setFilter();
    void setFilter(QString filter);
    void setFilterTime(QString dateI, QString dateF);

    bool areFilterTime;
    QString filterTime;
    void getCashDays();
    void getCashWeek();
    void getCashMonth();
    void getCashYear();
    void getCashCustom(QString dateI, QString dateF);

    bool areFilterType;
    QString filterType;

    inline QSqlTableModel *model(); // para facilitar o trabalho com model

signals:
    void count(QString);
    void totalValue(QString);

public:
    int idMovement;
    float value;
    QString type;
    QString date;
    QString hour;
    QString note;

    BDTools bd;

    QString dateIFilter;
    QString dateFFilter;

private:

public slots:
    void refreshModel();

};

#endif // MOVEMENTCASH_H
