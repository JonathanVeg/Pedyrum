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


 #include "login.h"
#include <QDebug>
#include <QMessageBox>

Login::Login(QObject *parent) :
    QObject(parent)
{
}

void Login::start(QSqlDatabase *sdb)
{
    bd.sdb = *sdb;

    // aqui dentro ele já verifica se existe usuário e, caso não exista ele cria um

    if (!bd.search("select * from usersystem")){

        if (!bd.exec("insert into usersystem values(1, 'root', '3114311131113116', 'No primeiro acesso, a senha é root', '3114311131113116')"))
        {
            QString errorMessage = QString(tr("O usuário inicial não pode ser criado.\nErro para informar ao suporte:\n\n%1")).arg(bd.lastError);

            QMessageBox::warning(NULL, tr("Usuário inicial não criado"), errorMessage, QMessageBox::Ok);

            return;
        }

        // como o user inicial é o root do sistema, ele tem acesso a tudo
        bd.exec("insert into accesslevel values(1, 'y', 'y', 'y', 'y', 'y', 'y', 'y', 'y', 'y', 'y')");
    }

    if (! bd.search("select * from statusServer")){

        bd.exec(QString("insert into statusServer values('%1')").arg(encryption.encrypt("false")));

    }

    if (bd.sdb.hostName() == "localhost"){
        qDebug() << "aqui1" << bd.exec(QString("update statusServer set status = '%1'").arg(encryption.encrypt("true")));

    }else{
        qDebug() << "aqui2" << bd.exec(QString("update statusServer set status = '%1'").arg(encryption.encrypt("false")));
    }
}

bool Login::login(QString username, QString password)
{
    QString sql;

    sql = QString("select * from usersystem where username = '%1' and password = '%2'").arg(username, encryption.encrypt(password));

    if (! bd.search(sql)){
        QMessageBox::warning(NULL, ("Dados incorretos"), tr("Dados digitados estão inválidos."), QMessageBox::Ok);

        return false;
    }

    QList<bool> permissions;

    QList<QString> permissionsS;

    sql = QString("select * from accesslevel where iduser = %1").arg(bd.loadSimple(0));

    if (bd.search(sql)){
        permissionsS = bd.loadLine(0);

        foreach(QString p, permissionsS){
            if (p == "true")
                permissions << true;
            else if(p == "false")
                permissions << false;
        }
    }

    emit logado(username, bd.loadSimple(0), permissions);

    return true;
}

void Login::loginWithSecretQuestion(QString username, QString secretanswer){
    QString sql;

    sql = QString("select * from usersystem where username = '%1' and secretanswer = '%2'").arg(username, encryption.encrypt(secretanswer));

    if (! bd.search(sql)){
        QMessageBox::warning(NULL, tr("Dados incorretos"), tr("Reposta secreta está inválida."), QMessageBox::Ok);

        return;
    }else{
        if (bd.exec(QString("update usersystem set password = '%1' where username = '%2'").arg(encryption.encrypt("pedyrum"), username))){

            QMessageBox::information(NULL, tr("Senha resetada"), tr("Resposta correta. Sua senha foi resetada e agora é 'pedyrum' (sem aspas).\nÉ altamente recomendado que se altere essa senha imediatamente."), QMessageBox::Ok);

        }else{
            qDebug() << "'senha não mudada'";

            qDebug() << QString("update usersystem set password = '%1'' where username = '%2'").arg(encryption.encrypt("pedyrum"), username);
        }

        QList<bool> permissions;

        QList<QString> permissionsS;

        sql = QString("select * from accesslevel from where iduser = %1").arg(bd.loadSimple(0));

        if (bd.search(sql)){
            permissionsS = bd.loadLine(0);

            foreach(QString p, permissionsS){
                if (p == "true")
                    permissions << true;
                else if(p == "false")
                    permissions << false;
            }
        }

        emit logado(username, bd.loadSimple(1), permissions);
    }
}

void Login::logout()
{
}

QString Login::secretQuestion(QString username)
{
    QString sql = QString("select secretquestion from usersystem where username = '%1'").arg(username);

    if (bd.search(sql))
        return bd.loadSimple(0);

    QMessageBox::warning(NULL, tr("Username não encontrado"), tr("Nome de usuário \"%1\" não cadastrado.").arg(username), QMessageBox::Ok);

    return "";
}

bool Login::saveSerial(QString serial)
{
    QString sql = QString("insert into buyaccess values('%1')").arg(encryption.encrypt(serial));

    return bd.exec(sql);
}

QString Login::serialBank()
{
    if (bd.search("select * from buyaccess"))
        return encryption.decrypt(bd.loadSimple(0));

    return "";
}

bool Login::isLocal()
{
    if (bd.sdb.hostName() == "localhost")
        return true;

    return false;

}
