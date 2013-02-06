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


 #ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <Abstract/bdtools.h>
#include <Abstract/tools.h>
#include <Abstract/encryption.h>

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = 0);

    void start(QSqlDatabase *sdb);

    // dados da empresa
    QString bPhone; // esse b é de Business, para não ter que escrever o nome td sempre (é chato)
    QString bName;
    QString bAddress;
    QString bDistrict;
    QString bCity;
    QString bState;

    bool insertDataBusiness();

    void loadDataBusiness();

    // dados do pedido
    int numberOfCopies;
    int fontSize; // inteiro que receberá o indice selecionado no combobox.
    int widthPaper;
    int typeNumering; // idem do fontsize
    int currentNumber;

    bool insertDataOrder();

    void loadDataOrder();

    // dados dos alertas
    QString daysAlertOrder;

    bool insertDataAlertOrder();

    void loadDataAlertOrder();

    // dados do backup
    QString dateLastBackup();

    // formas de pagamento
    QList<QString> getPaymentTypes();

    QList<QString> paymentTypes;

    bool insertDataPayementTypes();

    //rede
    bool saveDataAccessDatabase(QString username, QString password, QString ip, int port);

    bool loadDataNetwork();

    QString username;

    QString password;

    QString ip;

    QString port;

    // contas de usuário
    QString CurrentIdSystemUser;
    QString usernameSystemUser;
    QString passwordSystemUser;
    QString secretQuestionSystemUser;
    QString secretAnswerSystemUser;

    bool canAddAccount();

    bool insertUserAccount();

    bool insertUserAccessLevel(QString id);

    bool updateUserAccount(QString id);

    bool canEditAccount(QString id);

    bool canAlterAccess(QString id);

    QList<QList<QString> > getAllUsers();

    bool removeUser(QString id);

    QString idRoot();

    bool loadDataUser(QString idUser);

    bool cantreatment;
    bool canclient;
    bool canproduct;
    bool canordered;
    bool canemployee;
    bool canalert;
    bool canbill;
    bool cancash;
    bool cancreateuser;
    bool canalteraccess;


    // geral
    BDTools bd;

    Tools tools;

    Encryption e;

signals:

public slots:


};

#endif // CONFIG_H
