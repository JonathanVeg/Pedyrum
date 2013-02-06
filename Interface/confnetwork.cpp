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


 #include "confnetwork.h"
#include <QDebug>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QSqlError>
#include <QPushButton>
#include <QSqlQuery>

ConfNetwork::ConfNetwork(QWidget *parent) :
    QWidget(parent)
{
    createComponent();    
    connections();
    adjustLayout();
    statics();
    input();

    dbName = "bdpedyrum";
}

void ConfNetwork::createComponent()
{
    wGroup = new QWidget(this);

    lBackground = new QLabel(wGroup);

    lLogo = new QLabel(wGroup);

    lIpDisplay = new QLabel(wGroup);

    eUsername = new Edit(wGroup, true, tr("Usuário"));

    ePassword = new Edit(wGroup, false, tr("Senha"));

    aShowPassword = new QAction(tr("Exibir senha"),this);

    aHidePassword = new QAction(tr("Ocultar senha"),this);

    tbViewPassword = new QToolButton(wGroup);

    cbTypeConection = new ComboBox(wGroup);

    eIP = new Edit(wGroup, true, tr("IP"));

    ePort = new Edit(wGroup, true, tr("Porta"));

    eDir = new Edit(wGroup, true, tr("Diretório"));

    pbConnect = new QPushButton(wGroup);
}

void ConfNetwork::input()
{
    lBackground->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.498,"
                               "y1:0, x2:0.494636, y2:1, stop:0 rgba(255, 255, 255, 210),"
                               "stop:0.982955 rgba(255, 255, 255, 170));"
                               "border-radius: 15px;border: 1px solid rgba(0,0,0,25);");

    lLogo->setPixmap(QPixmap(":/NetConfig/logoNetwork"));
    lLogo->setScaledContents(true);

    QFont f;

    f.setPixelSize(16);

    lIpDisplay->setText(tr("IP: ") + getIP());
    lIpDisplay->setAlignment(Qt::AlignCenter);
    lIpDisplay->setFont(f);
    lIpDisplay->setStyleSheet("color: rgba(0,0,0,100);");

    f.setPixelSize(12);

    aShowPassword->setCheckable(true);
    aShowPassword->setChecked(false);
    aShowPassword->setEnabled(true);

    aHidePassword->setCheckable(true);
    aHidePassword->setChecked(true);
    aShowPassword->setEnabled(false);

    tbViewPassword->setAutoRaise(true);
    tbViewPassword->setPopupMode(QToolButton::MenuButtonPopup);
    tbViewPassword->addAction(aShowPassword);
    tbViewPassword->addAction(aHidePassword);
    tbViewPassword->setIcon(QIcon(":/Extras/question"));
    tbViewPassword->setIconSize(QSize(28,28));

    cbTypeConection->addItem(tr("Usar como"));
    cbTypeConection->addItem(tr("Cliente"));
    cbTypeConection->addItem(tr("Servidor"));    
    cbTypeConection->setFont(f);

    ePassword->setObjectName("ePassword");

    pbConnect->setText(tr("Entrar"));

    eUsername->setNextComponent(ePassword);
    ePassword->setNextComponent(cbTypeConection);
    cbTypeConection->setNextComponent(eIP);
    eIP->setNextComponent(ePort);
    ePort->setNextComponent(eDir);
}

unsigned short int ConfNetwork::testConnectionFile()
{
    qDebug() << "testConnectionFile()";
    QString username;
    QString password;
    QString ip;
    int port;

    qDebug() << "Caminho do arquivo de configuração:" << QDir::currentPath() + "/config.txt";

    QFile file(QDir::currentPath() + "/config.txt");
    if (file.open(QIODevice::ReadOnly)){
        QTextStream in(&file);
        username = e.decrypt(in.readLine());
        password = e.decrypt(in.readLine());
        ip = e.decrypt(in.readLine());
        port = e.decrypt(in.readLine()).toInt();
    }


    qDebug() << "----- dados do arquivo -----";
    qDebug() << username;
    qDebug() << password;
    qDebug() << ip;
    qDebug() << port;
    qDebug() << dbName;
    qDebug() << "-----        fim       -----";
    file.close();

    sdb = QSqlDatabase::addDatabase("QPSQL","testConnection");
    sdb.setHostName(ip);
    sdb.setUserName(username);
    sdb.setPassword(password);
    sdb.setPort(port);
    sdb.setDatabaseName("");


    if (sdb.open()){

        qDebug() << " ---------- CONECTADO SEM BANCO";

        sdb.close();
        sdb.setDatabaseName(dbName);
        if (sdb.open()){
            sdb.close();

            emit connected();

            return ConnectionFull;
        }else{
            // vai deixar a conexão ligada sem banco mesmo, para criá-lo na hora

            sdb.setDatabaseName("");

            sdb.open();

            qDebug() << " ---------- AQUI ELE CRIA O BD";

            if (createDatabase(sdb))
                if (createTables(sdb))
                    emit connected();

            return ConnectionNoBank;
        }
    }

    qDebug() << sdb.lastError().text();
    return NoConnection;
}

unsigned short int ConfNetwork::testConnectionInput(){
    sdb.setHostName(eIP->text());
    sdb.setUserName(eUsername->text());
    sdb.setPassword(ePassword->text());
    sdb.setPort(ePort->text().toInt());

    sdb.setDatabaseName(dbName);

    if (sdb.open()){
        sdb.close();
        sdb.setDatabaseName(dbName);
        if (sdb.open()){
            sdb.close();
            return ConnectionFull;
        }else{
            // vai deixar a conexão ligada sem banco mesmo, para criá-lo na hora

            sdb.setDatabaseName("");

            sdb.open();

            if (createDatabase(sdb))
                if (createTables(sdb))
                    emit connected();

            return ConnectionNoBank;
        }
    }

    qDebug() << sdb.lastError();

    return NoConnection;
}

void ConfNetwork::saveData(QString username, QString password, QString ip, int port){
    QFile file(QDir::currentPath() + "/config.txt");
    if (file.open(QIODevice::WriteOnly)){
        QTextStream stream(&file);
        stream << e.encrypt(username) << "\n" << e.encrypt(password) << "\n" << e.encrypt(ip) << "\n" << e.encrypt(QString::number(port));
    }
    file.close();
}

bool ConfNetwork::isValid(QString text)
{
    QChar _char;
    int ascii;

    for (int i = 0; i < text.length(); i++){
        _char = text.at(i).toAscii();
        ascii = _char.toAscii();
        if (ascii > 57 || ascii < 48)
            return false;
    }
    return true;
}

void ConfNetwork::adjustLayout()
{
    wGroup->setGeometry(this->width()/2 - 190,this->height()/2 - 288,380,576);
}

void ConfNetwork::statics()
{
    lBackground->setGeometry(0,0,wGroup->width(), wGroup->height());

    lLogo->setGeometry(wGroup->width()/2 - 150,17,300,230);

    lIpDisplay->setGeometry(wGroup->width()/2 - 65, lLogo->y() + 95, 130, 25);

    eUsername->setGeometry(wGroup->width()/2 - 120, lIpDisplay->y() + 150, 240, 28);

    ePassword->setGeometry(eUsername->x(), eUsername->y() + 40, eUsername->width(), 28);

    tbViewPassword->setGeometry(ePassword->x() + ePassword->width() - 28,ePassword->y(), 28,28);

    cbTypeConection->setGeometry(ePassword->x(), ePassword->y() + 40, ePassword->width(), 28);

    eIP->setGeometry(ePassword->x(), cbTypeConection->y() + 40, ePassword->width(), 28);

    ePort->setGeometry(ePassword->x(), eIP->y() + 40, ePassword->width(), 28);

    eDir->setGeometry(ePort->x(), ePort->y() + 40, ePort->width(), 28);

    pbConnect->setGeometry(eDir->x() + 63, eDir->y() + 50, 115, 35);
}

void ConfNetwork::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void ConfNetwork::connections()
{
    connect(pbConnect, SIGNAL(clicked()), this, SLOT(onClick_pbConnect()));

    connect(ePassword, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged_ePassword()));

    connect(ePort, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged_ePort()));

    connect(cbTypeConection, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChanged_cbTypeConnection(int)));        

    connect(aShowPassword,SIGNAL(triggered()),this,SLOT(triggered_aShowPassword()));

    connect(aHidePassword,SIGNAL(triggered()),this,SLOT(triggered_aHidePassWord()));

    connect(eDir, SIGNAL(returnPressed()), this, SLOT(returnPressed_ePort()));
}

void ConfNetwork::returnPressed_ePort(){
    onClick_pbConnect();
}

void ConfNetwork::onCurrentIndexChanged_cbTypeConnection(int index){
    if (index == 2){
        eIP->setText("localhost");
        eIP->setEnabled(false);
    }else{
        eIP->setText(eIP->valueStandart);
        eIP->setEnabled(true);
    }
}

void ConfNetwork::onClick_pbConnect()
{
    // emit gotoNext(1);

    bool canTest = true;

    if (eUsername->isEmpty()){
        canTest = false;

        eUsername->setFocus();
    }else if (ePassword->isEmpty()){
        canTest = false;

        ePassword->setFocus();
    }else if (eIP->isEmpty()){
        canTest = false;

        eIP->setFocus();
    }else if (ePort->isEmpty()){
        canTest = false;

        ePort->setFocus();
    }

    if (! canTest){
        QMessageBox::warning(this, tr("Erro"), tr("Por favor, preencha todos os dados."), QMessageBox::Ok);

        return;
    }

    if (cbTypeConection->currentIndex() == 0){
        QMessageBox::warning(this,tr("Erro"),
                             tr("Por favor, selecione se essa máquina será cliente ou servidor."),
                             QMessageBox::Ok);
        cbTypeConection->setFocus();

        return;
    }

    int cInput = testConnectionInput();

    if (cInput == ConnectionFull){
        // agora ele salvará automaticamente os dados.
        saveData(eUsername->text(), ePassword->text(), eIP->text(), ePort->text().toInt());

        QMessageBox::information(this, tr("Conexão"), tr("Conexão testada com sucesso.\nOs dados foram salvos."), QMessageBox::Ok);

        // aqui ele tem que chamar o sistema

        emit connected();

        return;

    }else if (cInput == ConnectionNoBank){

        qDebug() << "aqui ele deve criar o banco";

    }else{

        qDebug() << "dados de conexão inválidos";

        QString message = tr("Conexão não realizada.");

        message += tr("\nDados do erro para informar ao administrador do banco:\n");

        message += sdb.lastError().text();

        QMessageBox::warning(this, tr("Dados inválidos"), message, QMessageBox::Ok);
    }
}

void ConfNetwork::onTextChanged_ePort(){
    if (!ePort->isEmpty())
        if (!isValid(ePort->text()))
            ePort->setText(ePort->text().mid(0,ePort->text().length() - 1));
}

QString ConfNetwork::getIP(){
    bool b = true;
    bool check = false;

    for (int count = 0; count < QNetworkInterface::allAddresses().count(); ++count)
    {
        for (int letras = 0; letras < QNetworkInterface::allAddresses().value(count).toString().count(); ++letras)
            if(QNetworkInterface::allAddresses().value(count).toString().mid(letras,1) == ":")
                b = false;
        if(b){
            if(!check)
            {
                check = true;
                return QNetworkInterface::allAddresses().value(count).toString();
            }
        }
        else
            b = true;
    }

    return "";
}

void ConfNetwork::triggered_aShowPassword()
{
    ePassword->setEchoMode(Edit::Normal);

    aShowPassword->setChecked(true);
    aShowPassword->setEnabled(false);

    aHidePassword->setChecked(false);
    aHidePassword->setEnabled(true);
}

void ConfNetwork::triggered_aHidePassWord()
{
    if (ePassword->isEmpty())
        return;

    ePassword->setEchoMode(Edit::Password);

    aHidePassword->setChecked(true);
    aHidePassword->setEnabled(false);

    aShowPassword->setChecked(false);
    aShowPassword->setEnabled(true);
}

void ConfNetwork::onTextChanged_ePassword()
{
    if(ePassword->text() == tr("Senha") && aHidePassword->isChecked() == true)
        triggered_aShowPassword();
    else
        triggered_aHidePassWord();
}

bool ConfNetwork::createDatabase(QSqlDatabase sdb)
{
    QSqlQuery query(sdb);

    QString sql = "create database " + dbName;

    bool exec = query.exec(sql);

    qDebug() << "Criação do banco:" << exec;

    return exec;
}

bool ConfNetwork::createTables(QSqlDatabase sdb)
{
    sdb.close();

    sdb.setDatabaseName(dbName);

    if (sdb.open()){

        QSqlQuery query(sdb);

        qDebug() << "Arquivo de criação do banco:" << QDir::currentPath() + "/banco.sql";
        QFile file(QDir::currentPath() + "/banco.sql");
        if (file.open(QIODevice::ReadOnly)){

            QTextStream in(&file);

            QString sql = in.readAll();

            if (! query.exec(sql)){

                QMessageBox::warning(this, "Erro na criação do banco", "O sistema não pode realizar a criação do banco de dados.\nErro para informar ao administrador: " + query.lastError().text());

                return false;
            }
            else
                return true;

        }else{
            QMessageBox::warning(this, "Erro na abertura do arquivo", "Erro na abertura do arquivo com os dados da criação do banco.");

            return false;
        }
    }else{
        qDebug() << "Embora conectado, banco não foi aberto para escrita das tabelas.";

        qDebug() << sdb.lastError().text();

        return false;
    }
}
