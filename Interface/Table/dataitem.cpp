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


 #include "dataitem.h"
#include <QDebug>

DataItem::DataItem(QObject *parent) :
    QObject(parent)
{
    resetValues();

    _id = -1;
}

/// --------- Transferir Conta ---------

void DataItem::setFather(int father) {
    _father = father;
    _isChildren = true;
    _isFather = false;
}

void DataItem::setChildren(int children) {
    _children = children;
    _isChildren = false;
    _isFather = true;
}

int DataItem::father() { return _father; }

int DataItem::children() { return _children; }

bool DataItem::isFather() { return _isFather; }

bool DataItem::isChildren() { return _isChildren; }

/// --------- Mesa Composta ---------

void DataItem::setCompound(bool b) { _isCompound = b; }

void DataItem::setCompound(QList<int> indexs) { _listCompound = indexs; _isCompound = true;}

bool DataItem::isCompound() { return _isCompound; }

/// --------- Propriedades ---------

void DataItem::setId(int index) { _id = index; }

int DataItem::id() { return _id; }

void DataItem::setFree(bool b) { _isFree = b; }

bool DataItem::isFree() { return _isFree; }

void DataItem::setBartender(int idbartender) { _idBartender = idbartender; }

int DataItem::bartender() { return _idBartender; }

void DataItem::setValueSimple(double value) {_valueSimple = value;}

void DataItem::setValueComplex(double value) {_valueComplex = value;}

/// --------- Média ---------

void DataItem::setMedia(int people)
{
    _countPeople = people;
    _media = _valueSimple/_countPeople;
    _isMedia = true;
}

bool DataItem::isMedia() { return _isMedia; }

double DataItem::media() { return _media; }

int DataItem::countPeople() { return _countPeople; }

double DataItem::value() { return _valueSimple; }

QList<int> DataItem::listCompound() { return _listCompound; }

void DataItem::resetValues()
{
    _father = -1;
    _children = -1;
    _isFather = false;
    _isChildren = false;

    _isCompound = false;
    _listCompound.clear();

    _isFree = true;
    _idBartender = -1;
    _valueSimple = 0.00;

    _isMedia = false;
    _countPeople = 1;
    _media = 0.00;
}

void DataItem::clearTransfer()
{
    _father = -1;
    _children = -1;
    _isFather = false;
    _isChildren = false;
}

void DataItem::clearCompound()
{
    _isCompound = false;
    _listCompound.clear();
}

bool DataItem::isSimple()
{
    if(isFather())
        return false;

    if(isChildren())
        return false;

    if(isCompound())
        return false;

    if (isMedia())
        return false;

    return true;
}
