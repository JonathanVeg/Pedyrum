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


 #include "generaldata.h"
#include <QDebug>

GeneralData::GeneralData(QObject *parent) :
    QObject(parent)
{
    createComponent();
    input();
}

void GeneralData::createComponent()
{
    for (int i = 0; i < 100; ++i) {
        _dataItem[i] = new DataItem(this);
    }
    for (int i = 0; i < 101; ++i) {
        _dataOrder[i] = new DataOrder(this);
    }
}

void GeneralData::input(){
    EMPTY_MODEL = 100;

    QList<QString> headers, headers2;

    headers << tr("Nome") << tr("Tamanho") << tr("Quantidade") << tr("Valor") << tr("Anotação");

    headers2 << tr("Forma de pagamento") << tr("Valor");

    for(usint i = 0; i < 101; i++){ // um a mais para ser o model vazio
        _modelItems[i] = new DataModel(this, headers);
        _modelPayment[i] = new DataModel(this, headers2);

    }    
}

DataModel * GeneralData::getModelItems(usint index)
{
    return _modelItems[index];
}

void GeneralData::setModelItems(usint index, DataModel *model)
{
    _modelItems[index] = model;
}

DataModel * GeneralData::getModelPayment(usint index)
{
    return _modelPayment[index];
}

void GeneralData::setModelPayment(usint index, DataModel *model){
    _modelPayment[index] = model;
}

void GeneralData::setDataOrder(usint index, DataOrder *dataOrder)
{
    if (index == 0) index = 100;

    _dataOrder[index] = dataOrder;
}

DataItem * GeneralData::getDataItem(usint index)
{
    return _dataItem[index];
}

DataOrder * GeneralData::getDataOrder(usint index)
{
    if (index == 0) index = 100;

    return _dataOrder[index];
}

void GeneralData::clearDataOrder(usint i)
{    
    qDebug() << Q_FUNC_INFO << " - i:" << i;

    usint j = i;

    if (i == 0)
        i = 100;


    if(_dataItem[j]->isCompound())
    {
        qDebug("------------------------------------------------");
        QList<int> list = _dataItem[j]->listCompound();

        qDebug() << list;

        foreach (int i, list) {
            qDebug() << "i j" << i << j;
            qDebug() << "indexOf" << _dataItem[i]->listCompound().indexOf(j);
            qDebug() << _dataItem[i]->listCompound();
            _dataItem[i]->_listCompound.removeAt(_dataItem[i]->_listCompound.indexOf(j));
            qDebug() << _dataItem[i]->listCompound();
        }

        if (list.count() == 1){
            _dataItem[list.at(0)]->setCompound(false);
        }
    }

   _dataOrder[i]->orderQuantity.clear();
   _dataOrder[i]->orderValues.clear();
   _dataOrder[i]->orderSizes.clear();    
   _dataOrder[i]->orderPartProduct.clear();
   _dataOrder[i]->orderIdProduct.clear();
   _dataOrder[i]->orderNameProduct.clear();
   _dataOrder[i]->orderNoteProduct.clear();

   _dataItem[j]->clearCompound();
   _dataItem[j]->clearTransfer();
   _dataItem[j]->setFree(true);
   _dataItem[j]->setBartender(-1);
   _dataItem[j]->setValueSimple(0);
   _dataItem[j]->setValueComplex(0);

   _modelItems[j]->clear();

   _modelPayment[j]->clear();


   emit updateData();
}
