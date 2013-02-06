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


 #ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <Object/people.h>
#include <Abstract/bdtools.h>
#include <Abstract/tools.h>

class Employee : public People
{
    Q_OBJECT

public:

    void start(QSqlDatabase *sdb);

    // funções da classe em si (sem ser gets e sets
    bool loadData(int idEmployee); // vc seta o id e da um load, ele carrega todos os dados

    void clearData(); // limpa todos os "campos" da classe

    bool instance(); // instancia o model de client, cria os completers e afins

    bool insert(); // ele pega todos os dados da classe e faz o insert no BD
    bool update(int id); // manda o id aqui, ele faz o load com tds os dados
    bool remove(int id);

    void setFilter(QString filter);

    void setFilter();

    void setFilterJobRole(QString jobRole);

    void clearFilterJobRole();

    void setFilterStatus(bool free);

    bool areFilterStatus;
    bool areFilterJobRole;
    QString filterStatus;
    QString filterJobRole;

    void clearFilter();

    double getValueCommission(QString id, double commission, QString dateI, QString dateF);


public:
    QString phoneMobile;
    QString radio;
    QString jobRole;
    double commission;
    QString cpf;
    QString rg;
    QString cc;
    QString ag;
    QString bank;

    BDTools bd;

    Tools tools;

public slots:
    void refreshModel();

};

#endif // EMPLOYEE_H
