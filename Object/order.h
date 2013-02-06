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


 #ifndef ORDER_H
#define ORDER_H

#include <QObject>
#include <Abstract/bdtools.h>
#include <Abstract/tools.h>
#include <Object/client.h>

typedef unsigned short int usint;

class Order : public QObject
{
    Q_OBJECT
public:
    explicit Order(QString connectionName, QObject *parent = 0);

    BDTools bd;
signals:
    void loadComplete();

public:

    void start(QSqlDatabase *sdb);

    QList<QString> getDataClient(QString phone);
    void setTypeClient(int typeClient);

    bool insertOrder();

    bool isAdd;

    bool insertShedule();

    int nextIdClient();

    bool updateClient(QList<QString> data);

    void loadDataOrder(int idOrder);

    void setIdSelectedBartender(int idSelectedBartender);

    QString phoneClient;

    Client *client;

    int typeClient;
    int idClient;
    double totalValueOrderProducts;
    double valuePaid;
    double valueShipping;
    QList<int> orderQuantity;
    QList<double> orderValues;
    QList<QString> orderSizes;
    QList<QList<int> > orderPartProduct;
    QList<QList<QString> > orderIdProduct;
    QList<QList<QString> > orderNameProduct;
    QList<QString> orderNoteProduct;
    QList<QString> orderPaymentForm;
    QList<QString> orderPaymentFormValue;
    bool isPaid;

    // coisas de mesa
    bool isMedia;
    bool isCompound;
    bool isChildren;
    bool isFather;
    unsigned int idBartender;
    QString linkedTables;
    usint countPeople; 
    usint currentTable;

    // agendamento

    QString dateSchedule;
    QString hourSchedule;

    // dados do cliente

    QString name;
    QString phone;

    // dados de endereço
    QString street;
    QString district;
    QString number;
    QString complement;
    QString cep;

    QString note;

    float shipping;
    QString nameRespondible;
    QString corporateName;
    QString fantasyName;
    QString CNPJ;

    bool insertClient();

    QString idOrder;

    int typeOrder;

    enum{
        DELYVERY = 1,
        TABLE = 2,
        COUNTER = 3
    };

    QList<QString> paymentTypes();
    QList<QList<QString> > bartenders();

    QList<QList<QString> > listBartenders;

    int _idSelectedBartender;

    int totalOrders();

protected:
    void instance();

private:
    Tools tools;

};

#endif // ORDER_H
