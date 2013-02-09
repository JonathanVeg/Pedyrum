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
#include <QDir>
#include <QDesktopServices>
#include <QUrl>

#ifdef WINDOWS
	#include <windows.h>
#endif

ConfNetwork::ConfNetwork(QWidget *parent) :
    QWidget(parent)
{
    createComponent();    
    connections();
    adjustLayout();
    statics();
    input();
}

void ConfNetwork::createComponent()
{
    wGroup = new QWidget(this);

    lBackground = new QLabel(wGroup);

    lLogo = new QLabel(wGroup);

    lIpDisplay = new QLabel(wGroup);

    eUsername = new Edit(wGroup, true, tr("Usuário do banco"));

    ePassword = new Edit(wGroup, false, tr("Senha do banco"));

    aShowPassword = new QAction(tr("Exibir senha"),this);

    tbViewPassword = new QToolButton(wGroup);

    cbTypeConection = new ComboBox(wGroup);

    eIP = new Edit(wGroup, true, tr("Endereço IP do servidor"));

    ePort = new Edit(wGroup, true, tr("Porta do firewall, exemplo: 5432"));

    eDir = new Edit(wGroup, true, tr("Diretório do banco de dados"));

    pbConnect = new QPushButton(tr("Entrar"),wGroup);

    lDownloadManual = new Label(wGroup);

    swPanels = new QStackedWidget(this);

    wSplash = new QWidget(this);

    lSplash = new QLabel(wSplash);

    cdThread = new CreateDatabaseThread();
}

void ConfNetwork::input()
{
    dbName = "bdpedyrum";

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

    tbViewPassword->setAutoRaise(true);
    tbViewPassword->setPopupMode(QToolButton::MenuButtonPopup);
    tbViewPassword->addAction(aShowPassword);
    tbViewPassword->setIcon(QIcon(":/Extras/question"));
    tbViewPassword->setIconSize(QSize(28,28));

    cbTypeConection->addItem(tr("Usar como (cliente/servidor)"));
    cbTypeConection->addItem(tr("Cliente"));
    cbTypeConection->addItem(tr("Servidor"));
    cbTypeConection->setFont(f);

    lDownloadManual->setAlignment(Qt::AlignHCenter);
    lDownloadManual->setText(tr("Download do Manual"));
    lDownloadManual->setStyleSheet("color: rgba(0,90,127,200);");

    eUsername->setNextComponent(ePassword);
    ePassword->setNextComponent(cbTypeConection);
    //    ePassword->setNextComponent(ePort);
    cbTypeConection->setNextComponent(eIP);
    eIP->setNextComponent(ePort);
    ePort->setNextComponent(eDir);

    swPanels->addWidget(wSplash);
    swPanels->addWidget(wGroup);

    lSplash->setScaledContents(true);
    lSplash->setPixmap(QPixmap(":/Extras/splash"));
}

void ConfNetwork::testConnectionFile()
{
    QString username;
    QString password;
    QString ip;
    QString portS;

    int port;

    QFile file(QDir::currentPath() + "/config.txt");
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        username = in.readLine();
        password = in.readLine();
        ip = in.readLine();
        portS = in.readLine();
    }

    file.close();

    if ((username == "") && (password == "") && (ip == "") && (portS == ""))
    {
        emit noConnected();
        return;
    }

    username = e.decrypt(username);
    password = e.decrypt(password);
    ip = e.decrypt(ip);
    port = e.decrypt(portS).toInt();

    sdb = QSqlDatabase::addDatabase("QPSQL", "testeConnectionFile");

    sdb.setUserName(username);
    sdb.setPassword(password);
    sdb.setHostName(ip);
    sdb.setPort(port);
    sdb.setDatabaseName(dbName);

    if (sdb.open()){ // se estiver conectado
        // libera o firewall
        //system("netsh advfirewall firewall add portopening UDP 5432 blah enable subnet");

        createNewTables();

        emit connected();

        return;
    }

    sdb.setDatabaseName("");

    if (sdb.open())
    {
        createDatabase();
    }
    else
    {
        QString errorMessage = QString("Dados digitados inválidos.\n\nCaso todos os dados estejam certos e mesmo assim não conecta, informe a mensagem abaixo ao suporte:\n").append(sdb.lastError().text());

        QMessageBox::warning(this, tr("Dados inválidos"), errorMessage, QMessageBox::Ok);

        emit noConnected();
    }
}

void ConfNetwork::testConnectionInput()
{
    sdb = QSqlDatabase::addDatabase("QPSQL","testConnectionInput");

    sdb.setHostName(eIP->text());
    sdb.setUserName(eUsername->text());
    sdb.setPassword(ePassword->text());
    sdb.setPort(ePort->text().toInt());
    sdb.setDatabaseName(dbName);

    if (sdb.open())
    {
        saveData(sdb.userName(), sdb.password(), sdb.hostName(), sdb.port());

        createNewTables();

        if(cbTypeConection->currentIndex() == 2)
        {
            if(configPostgres(eDir->text()))
            {
                QMessageBox::about(this,tr("Reinicie seu Computador"),tr("Para que a rede funcione corretamente, reinicie seu computador."));
            }
            else
            {
                if(QMessageBox::question(this, tr("Diretório do Banco"),
                                         tr("Informe corretamente o diretório do Postgres. "
                                            "Seguindo o exemplo diretório/postgres/[Versão atual]/data "
                                            "Esse processo é fundamental para que a rede funcione. "
                                            "Deseja continuar, e Instalar sem Rede ?"), QMessageBox::Yes,QMessageBox::No) == QMessageBox::No)
                {
                    return;
                }
            }
        }

        emit connected();

        return;
    }

    sdb.setDatabaseName("");

    if (sdb.open())
    {
        saveData(eUsername->value(), ePassword->value(), eIP->value(), ePort->value().toInt());

        createDatabase();

        emit goToSplash();
    }
    else
    {
        QString errorMessage = QString("Não foi possível conectar ao banco de dados.\n\nErro para informar ao suporte:\n").append(sdb.lastError().text());

        QMessageBox::warning(this, tr("Banco não criado"), errorMessage, QMessageBox::Ok);
    }
}

void ConfNetwork::saveData(QString username, QString password, QString ip, int port){
    QFile file(QDir::currentPath() + "/config.txt");

    if (file.open(QIODevice::WriteOnly)){

        QTextStream stream(&file);

        stream << e.encrypt(username) + "\n";

        stream << e.encrypt(password) + "\n";

        stream << e.encrypt(ip) + "\n";

        stream << e.encrypt(QString::number(port));
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
    if(eDir->isVisible())
        swPanels->setGeometry(this->width()/2 - 190,this->height()/2 - 288,380,590);
    else
        swPanels->setGeometry(this->width()/2 - 190,this->height()/2 - 288,380,550);

    wGroup->setGeometry(0,0,swPanels->width(),swPanels->height());

    wSplash->setGeometry(0,swPanels->height()/2 - 140,380,240);
}

void ConfNetwork::statics()
{
    lBackground->setGeometry(0,0,wGroup->width(), wGroup->height());

    lLogo->setGeometry(wGroup->width()/2 - 150,17,300,230);

    lIpDisplay->setGeometry(wGroup->width()/2 - 75, lLogo->y() + 95, 150, 25);

    eUsername->setGeometry(wGroup->width()/2 - 120, lIpDisplay->y() + 150, 240, 28);

    ePassword->setGeometry(eUsername->x(), eUsername->y() + 40, eUsername->width(), 28);

    tbViewPassword->setGeometry(ePassword->x() + ePassword->width() - 28,ePassword->y(), 28,28);

    cbTypeConection->setGeometry(ePassword->x(), ePassword->y() + 40, ePassword->width(), 28);

    eIP->setGeometry(ePassword->x(), cbTypeConection->y() + 40, ePassword->width(), 28);

    ePort->setGeometry(ePassword->x(), eIP->y() + 40, ePassword->width(), 28);

    if(eDir->isVisible())
    {
        eDir->setGeometry(ePort->x(), ePort->y() + 40, ePort->width(), 28);
        pbConnect->setGeometry(eDir->x() + 63, eDir->y() + eDir->height() + 15, 115, 35);
    }
    else
    {
        eDir->setGeometry(ePort->x(), ePort->y() + 40, ePort->width(), 0);
        pbConnect->setGeometry(ePort->x() + 63, ePort->y() + ePort->height() + 15, 115, 35);
    }

    lDownloadManual->setGeometry(pbConnect->x(), pbConnect->y() + 50, 115, 35);

    lSplash->setGeometry(0,0,380,240);
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

    connect(aShowPassword,SIGNAL(triggered()),this,SLOT(onTriggered_aShowPassword()));

    connect(lDownloadManual,SIGNAL(onClick()),this,SLOT(onClick_lDownloadManual()));

    connect(swPanels,SIGNAL(currentChanged(int)),SLOT(adjustLayout()));

    connect(this,SIGNAL(connected()),SLOT(hideSplash()));

    connect(this,SIGNAL(noConnected()),SLOT(hideSplash()));

    connect(this,SIGNAL(goToSplash()),SLOT(showSplash()));
}

void ConfNetwork::onClick_lDownloadManual()
{
    QDesktopServices::openUrl(QUrl("http://www.jaguati.com.br/download/manual_pedyrum_3.php"));
}

void ConfNetwork::onCurrentIndexChanged_cbTypeConnection(int index){
    if (index == 2)
    {
        eIP->setText("localhost");
        eIP->setEnabled(false);

        eDir->setVisible(true);

        ePort->setFocus();

        disconnect(eDir, SIGNAL(returnPressed()), this, SLOT(onClick_pbConnect()));

        connect(ePort, SIGNAL(returnPressed()), this, SLOT(onClick_pbConnect()));

    }else{
        eIP->Clear();
        eIP->setEnabled(true);

        eDir->setVisible(false);

        eDir->Clear();

        connect(eDir, SIGNAL(returnPressed()), this, SLOT(onClick_pbConnect()));

        disconnect(ePort, SIGNAL(returnPressed()), this, SLOT(onClick_pbConnect()));
    }

    adjustLayout();
    statics();
}


bool ConfNetwork::fieldVerification()
{
    if(eUsername->value().isEmpty())
        return false;

    if(ePassword->value().isEmpty())
        return false;

    if(ePort->value().isEmpty())
        return false;

    if(eIP->value().isEmpty())
        return false;

    if(cbTypeConection->currentIndex() == 0)
        return false;

    return true;
}

void ConfNetwork::onClick_pbConnect()
{
    if(fieldVerification())
        testConnectionInput();
    else
        QMessageBox::warning(this, "Dados Incorreto", "Preencha corretamente todos os campos.");
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

void ConfNetwork::onTriggered_aShowPassword()
{
    if(!aShowPassword->isChecked())
    {
        ePassword->setEchoMode(Edit::Password);

        aShowPassword->setChecked(false);
    }
    else
    {
        ePassword->setEchoMode(Edit::Normal);

        aShowPassword->setChecked(true);
    }
}

void ConfNetwork::onTextChanged_ePassword()
{
    if(aShowPassword->isChecked() == false && ePassword->echoMode() == Edit::Normal)
        ePassword->setEchoMode(Edit::Password);

    if(ePassword->text() == ePassword->valueStandart)
    {
        if(ePassword->echoMode() == Edit::Password)
            ePassword->setEchoMode(Edit::Normal);
    }
}

void ConfNetwork::createDatabase()
{
    QFile file(QDir::currentPath() + "/banco.sql");
    QFile file2(QDir::currentPath() + "/procedures.sql");


    if ((file.open(QIODevice::ReadOnly)) && (file2.open(QIODevice::ReadOnly))){

        QTextStream in(&file);
        QTextStream in2(&file2);

        cdThread->setDatabase(dbName);

        cdThread->setSqlDatabase(in.readAll());

        cdThread->setSqlProcedure(in2.readAll());

        QThread *thread = new QThread();

        cdThread->moveToThread(thread);

        connect(thread, SIGNAL(started()), cdThread, SLOT(createDatabase()));

        connect(cdThread, SIGNAL(databaseCreated(bool, QString)), this, SLOT(databaseC(bool,QString)));

        connect(cdThread, SIGNAL(tablesCreated(bool, QString)), this, SLOT(tablesC(bool,QString)));

        connect(cdThread, SIGNAL(proceduresCreated(bool, QString)), this, SLOT(proceduresC(bool,QString)));

        connect(cdThread, SIGNAL(allCreated()), this, SLOT(allC()));

        thread->start(QThread::HighPriority);

        file.close();

        file2.close();
    }else{

        QMessageBox::warning(this, "Erro na abertura do arquivo", "Erro na abertura do arquivo com os dados da criação do banco.");

    }
}

bool ConfNetwork::configPostgres(QString filename)
{
    QDir dir;

    if(!dir.cd(filename))
        return false;

    QString postgresFile = dir.currentPath() + "/pg_hba.conf";

    QFile file2(postgresFile); //Origem

    QFile file(filename + "/pg_hba.conf"); // Destino

    if(!file.open(QIODevice::WriteOnly))
        return false;

    QTextStream stream(&file);

    if(!file2.open(QFile::ReadOnly))
        return false;

    QString texto(QLatin1String(file2.readAll()));

    stream.operator <<(texto);

    file2.close();

    file.close();

    // abre e fecha o postgres

    /*
    system("net stop postgresql-9.1");
    system("net stop postgresql-x64-9.1");
    system("net start postgresql-9.1");
    system("net start postgresql-x64-9.1");
    */

    return true;
}

void ConfNetwork::hideSplash()
{
    swPanels->setCurrentIndex(1);
}

void ConfNetwork::databaseC(bool b, QString s)
{
    if (! b){
        QString errorMessage = QString(tr("Banco de dados não criado.\nVerifique os dados de conexão.\n\nCaso tenha certeza de que estao corretos, informe este erro ao suporte:\n").arg(s));

        QMessageBox::warning(this, tr("Base de dados não criada."), errorMessage, QMessageBox::Ok);
    }
}

void ConfNetwork::tablesC(bool b, QString s)
{
    if (! b){
        QString errorMessage = QString(tr("Banco de dados não criado corretamente.\n\nErro para informar ao suporte:\n").arg(s));

        QMessageBox::warning(this, tr("Base de dados não criada corretamente"), errorMessage, QMessageBox::Ok);
    }
}

void ConfNetwork::proceduresC(bool b, QString s)
{
    if (! b){
        QString errorMessage = QString(tr("Banco de dados não criado corretamente.\n\nErro para informar ao suporte:\n").arg(s));

        QMessageBox::warning(this, tr("Base de dados não criada corretamente"), errorMessage, QMessageBox::Ok);
    }
}

void ConfNetwork::allC()
{
    emit connected();
}

void ConfNetwork::showSplash()
{
    swPanels->setCurrentIndex(0);
}

void ConfNetwork::createNewTables()
{
    QFile file(QDir::currentPath() + "/banco.sql");

    if (file.open(QIODevice::ReadOnly))
    {
        QString bodyFile(QLatin1String(file.readAll()));

        QSqlQuery query(sdb);

        bool b = query.exec(bodyFile);

        qDebug() << b << query.lastError().text();
    }

    file.close();
}


