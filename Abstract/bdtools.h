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


 #ifndef BDTOOLS_H
#define BDTOOLS_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QModelIndex>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QCompleter>
#include <QSqlRelationalTableModel>
#include <Abstract/encryption.h>

class BDTools : public QObject
{
    Q_OBJECT
public: // função
    explicit BDTools(QObject *parent = 0);

    enum typeModel{
        TableModel = 0,
        QueryModel = 1,
        RelationalModel = 2
    };

    void start(QString database, QString connectionName, QString driver = "QPSQL");

    void createComponent();

    bool exec(QString sql);

    bool insert(QString table, QList<QString> values);

    bool insert(QString table, QList<QString> fields, QList<QString> values);

    bool insert(QString table, QString field, QString value);

    bool remove(QString table, QString fieldCondition, QString valueCondition);
    bool remove(QString table, int column); // fazer

    bool update(QString table, QList<QString> fields, QList<QString> values, QString fieldCondition, QString valueCondition);
    bool update(QString table, QString field, QString value, QString fieldCondition, QString valueCondition);

    bool search(QString sql, int modelSearchIndex = 0);
    bool search(QString table, QList<QString> fieldsCondition, QList<QString> valuesCondition, int modelSearchIndex = 0);
    bool search(QString table, QString fieldCondition, QString valueCondition, int modelSearchIndex = 0);

    QString loadSimple(int column, int modelSearchIndex = 0);

    QList<QString> loadLine(int line, int modelSearchIndex = 0);
    QList<QString> loadColumn(int column, int modelSearchIndex = 0);

    bool createModel(QString table, QList<QString> headers);
    bool createModelQuery(QString table, QString sql);
    bool createModelQuery(QString table, QString sql, QList<QString> headers);
    bool createModelRelational(QString table, int column, QString relationalTable, QString relationalField, QString displayColumn, QList<QString> headers);
    bool updateModelQuery(QString table, QString newSQL);

    QSqlTableModel *getModel(QString table);
    QSqlQueryModel *getModelQuery(QString table);
    QSqlRelationalTableModel *getModelRelational(QString table);

    bool refreshModel(QString table);
    bool refreshQueryModel(QString table);
    bool refreshRelationalModel(QString table);

    int headerToColumn(QString table, QString header); // pega o cabeçalho e retorna a coluna dele

    bool createCompleter(QString name, QString table, int column, bool caseSensitive, QCompleter::CompletionMode mode, typeModel type = TableModel);
    bool createCompleter(QString name, QString table, QString column, bool caseSensitive, QCompleter::CompletionMode mode, typeModel type = TableModel);
    QCompleter *getCompleter(QString name);

    int countRecord(QString table);

    int generateID(QString table, QString field);

signals:

public: // variáveis
    Encryption e;

    QSqlDatabase sdb;

    QSqlQueryModel *modelSearch[5];

    QCompleter *completers[5];
    QList<QString> nameCompleters;

    QSqlTableModel *models[5];
    QList<QString> nameModels;
    QList<QString> headers[5];

    QSqlRelationalTableModel *modelsRelational[3];
    QList<QString> nameModelsRelational;
    QList<QString> headersRelational[3];

    QSqlQueryModel *modelsQuery[5];
    QList<QString> nameModelsQuery;
    QList<QString> modelsQuerySql;
    QList<QList<QString> > modelsQueryHeaders;

    QString lastError; // guarda o erro dado no bd.exec, caso tenha

public slots:

};

#endif // BDTOOLS_H
