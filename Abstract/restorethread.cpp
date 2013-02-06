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


 #include "restorethread.h"

RestoreThread::RestoreThread(QObject *parent) :
    QThread(parent)
{
}

void RestoreThread::setDatabase(QString bdName, QString driver)
{
    this->bdName = bdName;

    this->driver = driver;
}

void RestoreThread::setFileName(QString fileName)
{
    this->fileName = fileName;
}

void RestoreThread::setTables(QList<QString> tables)
{
    this->tables = tables;
}

void RestoreThread::makeRestore(){
    bd.start(bdName, "connectionRestore", driver);

    // primeiro ele irá limpar o banco.

    QString sql;
    foreach (QString table, tables){
        sql = QString("delete from %1").arg(table);

        if (! bd.exec(sql))
            qDebug() << QString("[RestoreThread] [makeRestore] Tabela \"%1\" não pode ser esvaziada").arg(table);
    }

    // agora ele restaura os dados do arquivo

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

                value = encryption.decrypt(line);
            }

        }while(! line.isNull());
    }

    emit finishedRestore();

}
