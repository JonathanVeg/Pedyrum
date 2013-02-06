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


 #ifndef DATAITEM_H
#define DATAITEM_H

#include <QObject>
#include <QList>

class DataItem : public QObject
{
    Q_OBJECT
public:
    explicit DataItem(QObject *parent = 0);

    /// --------- Transferir Conta ---------

    void setFather(int father);
    void setChildren(int children);
    int father();
    int children();
    bool isFather();
    bool isChildren();
    void clearTransfer();

    /// --------- Mesa Composta ---------

    bool isCompound();
    void setCompound(bool b);
    void setCompound(QList<int> indexs);
    QList<int> listCompound();
    void clearCompound();

    /// --------- Propriedades ---------

    void setId(int index);
    int id();

    void setFree(bool b);
    bool isFree();

    void setBartender(int idbartender);
    int bartender();

    double value();

    void setValueSimple(double value);

    void setValueComplex(double value); // valor considerando tds as mesas ligadas a ela

    bool isSimple();

    /// --------- Média ---------

    void setMedia(int countPeople);
    bool isMedia();
    double media();
    int countPeople();

    /// --------- reset values ---------

    void resetValues();


signals:

public slots:

public:

    /// --------- Transferir Conta ---------

    int _father;
    int _children;
    bool _isFather;
    bool _isChildren;

    /// --------- Mesa Composta ---------

    bool _isCompound;
    QList<int> _listCompound;

    /// --------- Propriedades ---------

    int _id;
    bool _isFree;
    int _idBartender;
    double _valueSimple;
    double _valueComplex;

    /// --------- Média ---------

    bool _isMedia;
    double _media;
    short int _countPeople;

};

#endif // DATAITEM_H
