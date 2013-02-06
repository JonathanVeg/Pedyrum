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


 #include "backupthread.h"

BackupThread::BackupThread(QObject *parent) :
    QThread(parent)
{
}

void BackupThread::setDatabase(QString bdName, QString driver){
    this->bdName = bdName;

    this->driver = driver;
}

void BackupThread::setTables(QList<QString> tables)
{
    this->tables = tables;
}

QList<QString> BackupThread::getTables()
{
    return tables;
}

void BackupThread::setFilename(QString fileName)
{
    this->fileName = fileName;
}

QString BackupThread::getFileName(){
    return fileName;
}

void BackupThread::makeBackup()
{
    bd.start(bdName, "connectionBackup", driver);

    qDebug() << "Backup da tabela" << tables;

    int totalRecords = 0;

    for (int i = 0; i < tables.length(); i++){
        if (bd.search("select count (*) from " + tables[i]))
            totalRecords += bd.loadSimple(0).toInt();
        else
            qDebug() << "[backup] [createBackup] " << tables[i] << "vazia. sql dela: " << "select count (*) from " + tables[i];
    }

    qDebug() << "[backup] [total de registros]" << totalRecords;

    emit totalRecordsBackup(totalRecords); // total de registros

    int jump = 1;

    if (totalRecords > 100)
        jump = totalRecords / 100;

    QString backup = "";

    int current = 0;

    QList<QString> line;

    /// aqui começa a gerar o backup em si
    for (int i = 0; i < tables.length(); i++){

        qDebug() << "[BACKUP] [createBackup] Tabela atual:" << tables[i];

        if (bd.search("select * from " + tables[i])){
            // esse for, pegará a quantidade de linhas da tabela

            backup += "<table>" + tables[i] + "\n";

            int j = 0;

            while (j < bd.loadColumn(0).length()){

                backup += "   <line>\n";

                // aqui ele pega a linha em si

                line = bd.loadLine(j);

                for (int k = 0; k < bd.loadLine(0).length(); k++){

                    backup += "      <field>\n";
                    backup += "         " + encryption.encrypt(line[k]) + "\n";
                    backup += "      </field>\n";

                }

                backup += "   </line>\n";

                current ++;

                j++;

                if (current % jump == 0)
                    emit currentPositionBackup(current);

            }

            // esse emit é para ter o emit de 100%
            emit currentPositionBackup(current);

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
