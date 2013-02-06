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


 #include "product.h"
#include <QDebug>

Product::Product(QObject *parent) :
    QObject(parent)
{
}

void Product::start(QSqlDatabase *sdb){
    bd.sdb = *sdb;

    emitCountProducts();

    instance();
}

int Product::getIdProduct(){
    return idProduct;
}

QString Product::getName(){
    return name;
}

QString Product::getNickname(){
    return nickname;
}

void Product::getIdCategory(){
    if (bd.search("select idcategory from category where name = '" + nameCategory + "'"))
        setIdCategory(bd.loadSimple(0, 0));
    else{
        setIdCategory(QString::number(bd.generateID("category", "idcategory")));
        bd.exec("insert into category values(" + idCategory + ", '" + nameCategory + "')");
    }
}

QString Product::getNameCategory(){
    return nameCategory;
}

void Product::getIdSubCategory(){
    if (bd.search("select idsubcategory from subcategory where name = '" + nameSubCategory + "' and idcategory = " + idCategory))
        setIdSubCategory(bd.loadSimple(0, 0));
    else{
        bd.search("select max(idsubcategory) from subcategory where idcategory = " + idCategory);

        bd.loadSimple(0, 0);

        if (bd.loadSimple(0, 0) == "")
            idSubCategory = "1";
        else
            idSubCategory = QString::number(bd.loadSimple(0, 0).toInt() + 1);

        bd.exec("insert into subcategory values(" + idCategory + ", " + idSubCategory + ", '" + nameSubCategory + "')");
    }

}

QString Product::getNameSubCategory(){
    return nameSubCategory;
}

QString Product::getNote(){
    return note;
}

QList<QString> Product::getSizes(){
    return listSizes;
}

QList<QString> Product::getValues(){
    return listValues;
}

void Product::setIdProduct(int idProduct){
    this->idProduct = idProduct;
}

void Product::setName(QString name){
    this->name = name;
}

void Product::setNickname(QString nickname){
    this->nickname = nickname;
}

void Product::setIdCategory(QString idCategory){
    this->idCategory = idCategory;
}

void Product::setNameCategory(QString nameCategory){
    this->nameCategory = nameCategory;
}

void Product::setIdSubCategory(QString idSubCategory){
    this->idSubCategory = idSubCategory;
}

void Product::setNameSubCategory(QString nameSubCategory){
    this->nameSubCategory = nameSubCategory;
}

void Product::setNote(QString note){
    this->note = note;
}

void Product::setSizes(QList<QString> sizes){
    this->listSizes = sizes;
}

void Product::setValues(QList<QString> values){
    this->listValues = values;
}

bool Product::loadData(int idProduct){
    setIdProduct(idProduct);

    //    if (bd.search("select p.name, p.nickname, c.name, sc.name, p.note from product p, category c, subcategory sc"
    //              "where sc.idcategory = c.idcategory and c.idcategory = p.idcategory"
    //              "and sc.idsubcategory = p.idsubcategory and p.idproduct = " + QString::number(idProduct))){

    if (bd.search("select name, nickname, idcategory, idsubcategory, note from product where idproduct = " + QString::number(idProduct))){

        QList<QString> data = bd.loadLine(0);

        setName(data[0]);
        setNickname(data[1]);
        setIdCategory(data[2]);
        setIdSubCategory(data[3]);

        if (bd.search("select name from category where idcategory = " + data[2], 1))
            setNameCategory(bd.loadSimple(0, 1));

        if (bd.search("select name from subcategory where idsubcategory = " + data[3] + " and idcategory = " + data[2], 1))
            setNameSubCategory(bd.loadSimple(0, 1));

        setNote(data[4]);

        getPrices();

        return true;
    }
    return false;
}

void Product::clearData(){
    name.clear();
    nickname.clear();
    idCategory = "0";
    idSubCategory = "0";
    note.clear();
}

void Product::instance(){
    QList<QString> headers;


    headers << tr("Id") << tr("Nome") << tr("Apelido") << tr("Categoria") << tr("Seção") << tr("Anotação");
    bd.createModelQuery("product", "select p.idproduct, p.name as \"Nome\", p.nickname as \"apelido\", c.name as \"Categoria\", sc.name as \"Seção\", p.note as \"Anotação\" from product p, category c, subcategory sc where c.idcategory = p.idcategory and sc.idsubcategory = p.idsubcategory and sc.idcategory = c.idcategory order by p.idproduct", headers);


    headers.clear();
    headers << "Id" << "Nome";

    bd.createModel("category", headers);

    bd.createModel("size", headers);

    bd.createCompleter("category", "category", 1, false, QCompleter::InlineCompletion);

    bd.createCompleter("product", "product", 1, false, QCompleter::PopupCompletion, BDTools::QueryModel);

    bd.createModelQuery("subcategory", "select * from subcategory where idCategoria = 0"); // deixa o model recem criado, mas sem dados, para não afetar o completer

    bd.createCompleter("subcategory", "subcategory", 2, false, QCompleter::PopupCompletion, BDTools::QueryModel);

    bd.createCompleter("size", "size", 1, false, QCompleter::InlineCompletion);
}

void Product::refreshModels(){
    bd.refreshQueryModel("product");

    bd.refreshModel("category");

    bd.refreshModel("size");

    bd.refreshQueryModel("subcategory");
}

void Product::setFilter(QString filter){
    bd.getModel("product")->setFilter(filter);

    refreshModels();
}

void Product::clearFilter(){
    bd.getModel("product")->setFilter("");

    refreshModels();
}

QString Product::countProducts(QString idCategory, QString idSubcategory){
    if (!idCategory.isEmpty()){
        if (!idSubcategory.isEmpty()){
            bd.search("select count(*) from product where idCategory = " + idCategory + " and idSubcategory = " + idSubcategory, 3);
            return bd.loadSimple(0, 3);
        }else{
            bd.search("select count(*) from product where idCategory = " + idCategory, 3);
            return bd.loadSimple(0, 3);
        }
    }
    return QString::number(bd.countRecord("product"));
}

void Product::emitCountProducts(){
    emit count(QString::number(bd.countRecord("product")));
}

bool Product::insert(){
    QList<QString> values;

    // pegando o id da categoria
    if (bd.search("select * from category where lower(name) = lower('" + nameCategory + "')"))
        setIdCategory(bd.loadSimple(0, 0));
    else
        // se a categoria não existir, ele cria na hora
        bd.exec("insert into category values(" + QString::number(bd.generateID("category", "idcategory")) + ", '" + nameCategory + "')");

    // agora que ele criou, com certeza achará a categoria
    bd.search("select * from category where lower(name) = lower('" + nameCategory + "')");

    setIdCategory(bd.loadSimple(0, 0));

    //pegando o id da subcategoria
    if (bd.search("select idsubcategory from subcategory where lower(name) = lower('" + nameSubCategory + "') and idcategory = " + idCategory))
        setIdSubCategory(bd.loadSimple(0, 0));
    else{
        // pesquisando o id da subcageoria a ser criada
        bd.search("select max(idsubcategory) from subcategory where idcategory = " + idCategory);

        if (bd.loadSimple(0, 0) == "")
            idSubCategory = "1";
        else
            idSubCategory = QString::number(bd.loadSimple(0, 0).toInt() + 1);

        bd.exec("insert into subcategory values(" + idCategory + ", " + idSubCategory + ", '" + nameSubCategory + "')");
    }

    // pega o id do produto, gerado pelo sistema
    QString idProduct = QString::number(bd.generateID("product", "idproduct"));

    values << idProduct;
    values << name << nickname << idCategory << idSubCategory << note;

    // insere o produto, antes de inserir os valores
    bool insert = bd.insert("product", values);

    if (insert){

        QString sql;
        QString idSize;

        for (int i = 0; i < listSizes.length(); i++){
            //primeiro ve se o tamanho existe
            if (bd.search("select idsize from size where lower(name) = lower('" + listSizes[i] + "')")){
                idSize = bd.loadSimple(0, 0);

                bd.exec("insert into productvalue values(" + idProduct + ", " + idSize + ", '" + listValues[i] + "')");

            }else{
                // gera o novo id
                idSize = QString::number(bd.generateID("size", "idsize"));

                // se o tamanho não existir, ele cria na hora
                sql = "insert into size values(";
                sql += idSize;
                sql += ", '" + listSizes[i] + "')";

                bd.exec(sql);

                // agora associa o valor ao produto
                bd.exec("insert into productvalue values(" + idProduct + ", " + idSize + ", '" + listValues[i] + "')");
            }
        }

        emitCountProducts();
    }

    // da refresh nos models
    bd.refreshModel("size");
    bd.refreshModel("category");
    bd.refreshQueryModel("product");

    return insert;
}

bool Product::update(int idProduct){
    QString idP = QString::number(idProduct);

    if (listSizes.length() != listValues.length())
        return false; // tem que ter a mesma quantidade de tamanhos e valores

    QString sql;
    QString idSize;

    bd.exec("delete from productvalue where idproduct = " + idP);

    for (int i = 0; i < listSizes.length(); i++){
        //primeiro ve se o tamanho existe
        if (bd.search("select idsize from size where lower(name) = lower('" + listSizes[i] + "')")){
            idSize = bd.loadSimple(0, 0);

            bd.exec("insert into productvalue values(" + idP + ", " + idSize + ", '" + listValues[i] + "')");

        }else{
            // gera o novo id
            idSize = QString::number(bd.generateID("size", "idsize"));

            // se o tamanho não existir, ele cria na hora
            sql = "insert into size values(";
            sql += idSize;
            sql += ", '" + listSizes[i] + "')";

            bd.exec(sql);

            // agora associa o valor ao produto
            bd.exec("insert into productvalue values(" + idP + ", " + idSize + ", '" + listValues[i] + "')");
        }
    }

    // faz a edição de produtos em si

    QList<QString> values;
    QList<QString> fields;

    fields << "name" << "nickname" << "idcategory" << "idsubcategory" << "note";

    // pegando o idCategory
    getIdCategory();
    getIdSubCategory();

    values << name << nickname << idCategory << idSubCategory << note;

    if (bd.update("product", fields, values, "idproduct", QString::number(idProduct)))
        bd.refreshQueryModel("product");

    // da refresh nos models
    bd.refreshModel("size");
    bd.refreshModel("category");

    return true;
}

bool Product::remove(int idProduct){
    if (bd.exec("delete from productvalue where idproduct = " + QString::number(idProduct))){
        bool remove = bd.remove("product", "idproduct", QString::number(idProduct));

        if (remove){
            emitCountProducts();
            bd.refreshQueryModel("product");
        }

        return remove;
    }
    return false;
}

void Product::getPrices()
{
    bd.search("select s.name, pv.valueproduct from size s, productvalue pv where pv.idproduct = " + QString::number(idProduct) + " and s.idsize = pv.idsize");

    listSizes = bd.loadColumn(0);
    listValues = bd.loadColumn(1);
}

QList<QString> Product::getAllCategory()
{
    // só vai exbir no combobox as categorias que tiverem algum produto ligado a ela
    bd.search("select distinct(c.name) from category c, product p where c.idcategory = p.idcategory group by c.name");
    return bd.loadColumn(0);
}

QList<QString> Product::getAllSubCategory(QString nameCategory)
{
    bd.search("select idCategory from category where name = '" + nameCategory + "'");

    bd.search("select distinct(sc.name) from subcategory sc, product p where sc.idcategory = " + bd.loadSimple(0, 0) + " and p.idsubcategory = sc.idsubcategory group by sc.name", 1);

    return bd.loadColumn(0, 1);
}
