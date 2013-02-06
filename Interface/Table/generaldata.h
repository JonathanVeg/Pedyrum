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


 #ifndef GENERALDATA_H
#define GENERALDATA_H

#include <QObject>
#include <Interface/Table/dataitem.h>
#include <Abstract/datamodel.h>
#include <Interface/Table/dataorder.h>

typedef unsigned short int usint;

class GeneralData : public QObject
{
    Q_OBJECT
public:

//    struct DataOrder{
//        QList<int> orderQuantity;
//        QList<double> orderValues;
//        QList<QString> orderSizes;
//        QList<QList<int> > orderPartProduct;
//        QList<QList<QString> > orderIdProduct;
//        QList<QList<QString> > orderNameProduct;
//        QList<QString> orderNoteProduct;
//    };

    explicit GeneralData(QObject *parent = 0);
    void createComponent();

    void input(); // prepara os model vazios, já com cabecalhos

    DataModel *getModelItems(usint index);
    void setModelItems(usint index, DataModel *model);

    DataModel *getModelPayment(usint index);
    void setModelPayment(usint index, DataModel *model);

    DataOrder *getDataOrder(usint index);
    void setDataOrder(usint index, DataOrder *dataOrder);

    DataItem *getDataItem(usint index);

    usint EMPTY_MODEL;

    void clearDataOrder(usint index);

signals:
    void updateData();

public slots:

public:
    DataItem *_dataItem[100];
    DataModel *_modelItems[100];
    DataModel *_modelPayment[100];
    DataOrder *_dataOrder[101];
};

#endif // GENERALDATA_H
