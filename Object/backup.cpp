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


 #include "backup.h"
#include <QDebug>
#include <QFile>
#include <QtXml/QDomDocument>
#include <QtXml/qdom.h>
#include <pthread.h>

Backup::Backup(QObject *parent) :
    QObject(parent)
{

}

void Backup::start()
{
    bd.start("bdpedyrum", "connectionBackup", "QPSQL");
}

void Backup::threadBackup(void *fileName){
}

void Backup::createBackup(QString fileName)
{
    QList<QString> tables;

    tables << "\"TABRJ\"";
    tables << "client";
    tables << "clientBusiness";
    tables << "configAlert";
    tables << "employee";
    tables << "statusemployee";
    tables << "category";
    tables << "subcategory";
    tables << "product";
    tables << "size";
    tables << "productvalue";
    tables << "bill";
    tables << "movementcash";
    tables << "ordered";
    tables << "employeeorder";
    tables << "itemOrder";
    tables << "productItem";
    tables << "paymentorder";
    tables << "alert";
    tables << "schedule";
    tables << "schedulealert";
    tables << "tasksimple";
    tables << "business";
    tables << "configorder";
    tables << "datecloseorder";

    int totalRecords = 0;

    for (int i = 0; i < tables.length(); i++){
        if (bd.search("select count (*) from " + tables[i]))
            totalRecords += bd.loadSimple(0).toInt();
        else
            qDebug() << "[backup] [createBackup] " << tables[i] << "vazia. sql dela: " << "select count (*) from " + tables[i];
    }

    qDebug() << "[backup] [total de registros]" << totalRecords;

    emit totalRecordsBackup(totalRecords); // total de registros

    QString backup = "";

    int current = 0;

    /// aqui começa a gerar o backup em si
    for (int i = 0; i < tables.length(); i++){

        qDebug() << "[BACKUP] [createBackup] Tabela atual:" << tables[i];

        if (bd.search("select * from " + tables[i])){
            // esse for, pegará a quantidade de linhas da tabela

            backup += "<table>" + tables[i] + "\n";

            qDebug() << "bd.loadColumn(0).length()" << bd.loadColumn(0).length();

            int j = 0;

            while (j < bd.loadColumn(0).length()){

                backup += "   <line>\n";

                qDebug() << "j" << j;

                // aqui ele pega a linha em si
                for (int k = 0; k < bd.loadLine(0).length(); k++){

                    backup += "      <field>\n";
                    backup += "         " + e.encrypt(bd.loadLine(j)[k]) + "\n";
                    backup += "      </field>\n";

                }
                backup += "   </line>\n";

                current ++;

                j++;

                Sleep(1);

                emit currentPositionBackup(current);
            }

            backup += "</table>\n";
        }
    }

    /// aqui ele salva o arquivo do backup
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)){
        QTextStream stream(&file);
        stream << (backup);
    }
    file.close();

    emit finishedBackup();
}

void Backup::restoreBackup(QString fileName)
{
    // limpa o banco td;

    qDebug() << "[backup] [restoreBackup] deletando dados do banco";

    if (! bd.exec("delete from configAlert"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela configAlert não limpa";

    if (! bd.exec("delete from statusemployee"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela statusemployee não limpa";

    if (! bd.exec("delete from employee"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela employee não limpa";

    if (! bd.exec("delete from subcategory"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela subcategory não limpa";

    if (! bd.exec("delete from productvalue"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela productvalue não limpa";

    if (! bd.exec("delete from itemOrder"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela itemorder não limpa";

    if (! bd.exec("delete from size"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela size não limpa";

    if (! bd.exec("delete from bill"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela bill não limpa";

    if (! bd.exec("delete from movementcash"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela movementcash não limpa";

    if (! bd.exec("delete from paymentorder"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela paymentorder não limpa";

    if (! bd.exec("delete from ordered"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela ordered não limpa";

    if (! bd.exec("delete from employeeorder"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela employeeorder não limpa";

    if (! bd.exec("delete from productItem"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela productitem não limpa";

    if (! bd.exec("delete from alert"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela alert não limpa";

    if (! bd.exec("delete from schedule"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela schedule não limpa";

    if (! bd.exec("delete from schedulealert"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela schedulealert não limpa";

    if (! bd.exec("delete from tasksimple"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela tasksimple não limpa";

    if (! bd.exec("delete from business"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela business não limpa";

    if (! bd.exec("delete from configorder"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela configorder não limpa";

    if (! bd.exec("delete from client"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela client não limpa";

    if (! bd.exec("delete from clientBusiness"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela clientbusiness não limpa";

    if (! bd.exec("delete from product"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela product não limpa";

    if (! bd.exec("delete from category"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela category não limpa";

    if (! bd.exec("delete from datecloseorder"))
        qDebug() << "[backup] [restoreBackup] [Limpeza do banco] tabela datecloseorder não limpa";

    qDebug() << "[backup] [restoreBackup] todos os dados do bd deletado";

    /// aqui eu descriptografarei o arquivo e irei ler os dados e restaura-los

    QString table;

    // por que não jogar direto em values a string? campos como anotação tem mais de uma linha, logo, como ele le linha a linha, ia dar errado.
    QString value; // valor para jogar em values e add a tabela.

    QList<QString> values; // valores para adição

    int current = 0;
    int totalReg = 0;

    QFile fileCount(fileName);
    if (fileCount.open(QIODevice::ReadOnly)){
        QTextStream inCount(&fileCount);

        QString line;

        do{
            line = inCount.readLine();

            if (line.count("</line>") > 0){
                totalReg ++;
            }

        }while (! line.isNull());

        emit totalRecordsRestore(totalReg);
    }

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)){
        QTextStream in(&file);

        QString line;
        do {

            // linha em questao do arquivo
            line = in.readLine();

            if (line.count("<table>") > 0){
                // inicio de uma tabela especifica.

                line.replace("<table>", "");

                table = line;

            }else if (line.count("</field>") > 0){
                // final de um campo especifico

                values << value;

                value.clear();

            }else if (line.count("</line>") > 0){
                // final de uma linha (aqui ele deve inserir a linha já preparada)

                current++;

                emit currentPositionRestore(current);

                bd.insert(table, values);

                values.clear();
            }else if (line.count("         ") > 0){
                // ve se tem os 9 espaços iniciais (caracteristica da linha de valor

                line = line.replace("         ", "");

                value = e.decrypt(line);
            }

        }while(! line.isNull());
    }

    emit finishedRestore();
}
