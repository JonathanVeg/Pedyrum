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


 #ifndef PRODUCT_H
#define PRODUCT_H

#include <QObject>
#include <Abstract/bdtools.h>

class Product : public QObject
{
    Q_OBJECT
public:
    explicit Product(QObject *parent = 0);

public:

    void start(QSqlDatabase *sdb);

    int getIdProduct();
    QString getName();
    QString getNickname();
    QString getNameCategory();
    QString getNameSubCategory();
    QString getNote();
    QList<QString> getSizes();
    QList<QString> getValues();

    void setIdProduct(int idProduct);
    void setName(QString name);
    void setNickname(QString nickname);
    void setIdCategory(QString idCategory);
    void setNameCategory(QString nameCategory);
    void setIdSubCategory(QString idSubCategory);
    void setNameSubCategory(QString nameSubCategory);
    void setNote(QString note);
    void setSizes(QList<QString> listSizes);
    void setValues(QList<QString> listValues);

    bool loadData(int idProduct);

    void clearData();

    void instance(); // terminar


    bool insert();
    bool update(int idProduct);
    bool remove(int idProduct);

    QString countProducts(QString idCategory = "", QString idSubcategory = "");

    void emitCountProducts();

    void setFilter(QString filter);

    void clearFilter();

    void getPrices(); // pelo id do produto ele carrega os valores e os tamanhos para sizes e values para ser carregado na tabela de valores

    QList<QString> getAllCategory();
    QList<QString> getAllSubCategory(QString nameCategory);

protected:
    int idProduct;
    QString name;
    QString nickname;
    QString idCategory;
    QString nameCategory;
    QString idSubCategory;
    QString nameSubCategory;
    QString note;
    QList<QString> listSizes;
    QList<QString> listValues;

public:
    BDTools bd;

signals:
    void count(QString);

protected:
    void getIdCategory();
    void getIdSubCategory();

public slots:
    void refreshModels();

};

#endif // PRODUCT_H
