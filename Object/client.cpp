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


 #include "client.h"
#include <QDebug>

Client::Client(){
}

Client::~Client(){
}

void Client::start(QSqlDatabase *sdb){
    bd.sdb = *sdb;

    emitCountClients();

    instance();
}

void Client::setShipping(float shipping){
    this->shipping = shipping;
}

float Client::getShipping(){
    return shipping;
}

QString Client::getNameRespondible(){
    return nameRespondible;
}

QString Client::getCorporateName(){
    return corporateName;
}

QString Client::getFantasyName(){
    return fantasyName;
}

QString Client::getCNPJ(){
    return CNPJ;
}

void Client::setNameRespondible(QString nameRespondible){
    this->nameRespondible = nameRespondible;
}

void Client::setCorporateName(QString corporateName){
    this->corporateName = corporateName;
}

void Client::setFantasyName(QString fantasyName){
    this->fantasyName = fantasyName;
}

void Client::setCNPJ(QString CNPJ){
    this->CNPJ = CNPJ;
}

void Client::setTypeClient(int typeClient){
    this->typeClient = typeClient;

    emitCountClients();
}

bool Client::loadData(int idClient){
    setId(idClient);
    if (bd.search("select name, phone, street, district, nhouse, complement, cep, note, shipping from client where idclient = " + QString::number(idClient))){

        QList<QString> data = bd.loadLine(0);

        setName(data[0]);
        setPhone(data[1]);
        setStreet(data[2]);
        setDistrict(data[3]);
        setNumber(data[4]);
        setComplement(data[5]);
        setCEP(data[6]);
        setNote(data[7]);
        setShipping(data[8].toFloat());

        return true;
    }

    return false;
}

bool Client::loadDataBusiness(int idClient){
    setId(idClient);

    if (bd.search("select namerespondible, phone, street, district, nhouse, complement, cep, note, shipping, namerespondible, corporatename, fantasyname, cnpj from clientbusiness where idclient = " + QString::number(idClient))){

        QList<QString> data = bd.loadLine(0);

        setName(data[0]);
        setPhone(data[1]);
        setStreet(data[2]);
        setDistrict(data[3]);
        setNumber(data[4]);
        setComplement(data[5]);
        setCEP(data[6]);
        setNote(data[7]);
        setShipping(data[8].toFloat());
        setNameRespondible(data[9]);
        setCorporateName(data[10]);
        setFantasyName(data[11]);
        setCNPJ(data[12]);

        return true;
    }

    return false;
}

void Client::clearData(){
    setId(0);
    setPhone("");
    setCEP("");
    setName("");
    setStreet("");
    setDistrict("");
    setNumber("");
    setComplement("");
    setNote("");
    setShipping(0);
}

bool Client::instance(){
    QList<QString> headers;

    headers << tr("id") << tr("Nome") << tr("Telefone") << tr("Rua") << tr("Número") << tr("Complemento") << tr("Bairro") << tr("CEP") << tr("Frete") << tr("Anotação");

    if (!bd.createModel("client", headers))
        return false;

    if (!bd.createCompleter("phone", "client", 2, false, QCompleter::PopupCompletion))
        return false;

    headers.clear();

    /// model do cliente empresa

    headers << tr("id") << tr("Responsável") << tr("Razão Social") << tr("Nome Fantasia") << tr("CNPJ") << tr("Telefone") << tr("Rua") << tr("Número") << tr("Complemento") << tr("Bairro") << tr("CEP") << tr("Frete") << tr("Anotação");

    if (!bd.createModel("clientBusiness", headers))
        return false;

    if (!bd.createCompleter("phoneBusiness", "clientBusiness", 5, false, QCompleter::PopupCompletion))
        return false;

    return true;
}

void Client::emitCountClients()
{
    if (typeClient == 1)
        emit count(QString::number(bd.countRecord("client")));
    else
        emit count(QString::number(bd.countRecord("clientBusiness")));
}

QString Client::countClients()
{
    if (typeClient == 1)
        return QString::number(bd.countRecord("client"));

    return QString::number(bd.countRecord("clientBusiness"));
}

bool Client::insert()
{
    if (typeClient == 1){
        QList<QString> values;

        values << QString::number(bd.generateID("client","idClient"));
        values << name << phone << street << number << complement << district << cep << QString::number(shipping) << note;

        bool insert = bd.insert("client",values);

        if (insert)
        {
            emitCountClients();
            bd.refreshModel("client");
        }

        return insert;
    }else{
        QList<QString> values;

        values << QString::number(bd.generateID("clientBusiness","idClient"));
        values << nameRespondible << corporateName << fantasyName << CNPJ << phone << street << number << complement << district << cep << QString::number(shipping) << note;

        bool insert = bd.insert("clientBusiness",values);

        if (insert)
        {
            emitCountClients();
            bd.refreshModel("clientBusiness");
        }

        return insert;
    }
}

bool Client::update(int id)
{
    if (typeClient == 1){
        QList<QString> values;
        QList<QString> fields;

        values << name << phone << street << number << complement << district << cep << note << QString::number(shipping);
        fields << "name" << "phone" << "street" << "nhouse" << "complement" << "district" << "cep" << "note" << "shipping";

        return bd.update("client", fields, values, "idClient", QString::number(id)); // update não precisa dar reload na quantidade de clientes
    }else{
        QList<QString> values;
        QList<QString> fields;

        values << nameRespondible << corporateName << fantasyName << CNPJ << phone << street << number << complement << district << cep << QString::number(shipping) << note;
        fields << "namerespondible" << "corporatename" << "fantasyname" << "cnpj" << "phone" << "street" << "nhouse" << "complement" << "district" << "cep" << "shipping" << "note";

        return bd.update("clientbusiness", fields, values, "idClient", QString::number(id)); // update não precisa dar reload na quantidade de clientes
    }
}

bool Client::remove(int id)
{
    if (typeClient == 1){
        bool remove = bd.remove("client", "idClient", QString::number(id));

        if (remove)
            emitCountClients();

        return remove;
    }else{
        bool remove = bd.remove("clientBusiness", "idClient", QString::number(id));

        if (remove)
            emitCountClients();

        return remove;
    }
}

bool Client::getDataAdress(QString cep)
{
    if (bd.search("select street, district, shipping from client where cep = '" + cep + "'")){
        QList<QString> data = bd.loadLine(0);

        setStreet(data[0]);
        setDistrict(data[1]);
        setShipping(data[2].toFloat());

        return true;
    }
    return false;
}

bool Client::getDataClient(QString phone){
    if (bd.search("select idClient from client where phone = '" + phone + "'")){
        setId(bd.loadSimple(0).toInt());
        loadData(id);
        return true;
    }
    return false;
}

bool Client::getDataClientBusiness(QString phone){
    if (bd.search("select idClient from clientBusiness where phone = '" + phone + "'")){
        setId(bd.loadSimple(0).toInt());
        loadDataBusiness(id);
        return true;
    }
    return false;
}

void Client::refreshModel()
{
    bd.refreshModel("client");
    bd.refreshModel("clientBusiness");
}

void Client::setFilter(QString filter)
{
    if (typeClient == 1)
        bd.getModel("client")->setFilter(filter);
    else
        bd.getModel("clientBusiness")->setFilter(filter);
    refreshModel();
}

void Client::clearFilter(){
    bd.getModel("client")->setFilter("");
    bd.getModel("clientBusiness")->setFilter("");
    refreshModel();
}

void Client::setFilterBusiness(QString filter)
{
    bd.getModel("clientBusiness")->setFilter(filter);
    refreshModel();
}

void Client::clearFilterBusiness(){
    bd.getModel("clientBusiness")->setFilter("");
    refreshModel();
}

int Client::totalClients()
{
    int total = 0;

    QString sql = "select count(*) from client";

    if (bd.search(sql))
        total += bd.loadSimple(0).toInt();

    sql = "select count(*) from clientbusiness";

    if (bd.search(sql))
        total += bd.loadSimple(0).toInt();

    return total;
}
