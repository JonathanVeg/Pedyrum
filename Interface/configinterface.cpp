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


 #include "configinterface.h"
#include <QMessageBox>
#include <QNetworkInterface>

ConfigInterface::ConfigInterface(QWidget *parent) :
    QWidget(parent)
{
    createComponent();
    connections();
}

void ConfigInterface::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Delete){
        if (listFormPayment->hasFocus())
            onClick_pbRemovePaymentForm();
    }


    switch (event->modifiers()){

    case Qt::ControlModifier:{

        if (event->key() == Qt::Key_Tab)
            emit goToPanel(12);

        break;
    }
    }

    if (event->key() == Qt::Key_Backtab)
        emit goToPanel(10);

    if (event->key() == Qt::Key_F11)
        emit fullScreen();
}

void ConfigInterface::input()
{
    oldProgress = 0;

    lBar->setStyleSheet("background-color: rgba(0,0,0,180);"
                        "border: 1px solid rgba(255, 255, 255, 10);");

    QFont f;

    f.setPointSize(13);

    itemFormPayment->setFont(f);
    itemFormPayment->setHover("background-color: rgba(255,255,255,30);");
    itemFormPayment->setStandard("background-color: rgba(255,255,255,20);");
    itemFormPayment->setFontColor("color: rgba(255,255,255,100);");

    itemOrdered->setFont(f);
    itemOrdered->setHover("background-color: rgba(255,255,255,30);");
    itemOrdered->setStandard("background-color: rgba(255,255,255,10);");
    itemOrdered->setFontColor("color: rgba(255,255,255,100);");

    itemBusiness->setFont(f);
    itemBusiness->setHover("background-color: rgba(255,255,255,30);");
    itemBusiness->setStandard("background-color: rgba(255,255,255,20);");
    itemBusiness->setFontColor("color: rgba(255,255,255,100);");

    itemAlerts->setFont(f);
    itemAlerts->setHover("background-color: rgba(255,255,255,30);");
    itemAlerts->setStandard("background-color: rgba(255,255,255,10);");
    itemAlerts->setFontColor("color: rgba(255,255,255,100);");

    itemDB->setFont(f);
    itemDB->setHover("background-color: rgba(255,255,255,30);");
    itemDB->setStandard("background-color: rgba(255,255,255,20);");
    itemDB->setFontColor("color: rgba(255,255,255,100);");

    itemAccount->setFont(f);
    itemAccount->setHover("background-color: rgba(255,255,255,30);");
    itemAccount->setStandard("background-color: rgba(255,255,255,10);");
    itemAccount->setFontColor("color: rgba(255,255,255,100);");

    itemNetwork->setFont(f);
    itemNetwork->setHover("background-color: rgba(255,255,255,30);");
    itemNetwork->setStandard("background-color: rgba(255,255,255,20);");
    itemNetwork->setFontColor("color: rgba(255,255,255,100);");

    // Forma de pagamento

    lFormPayment->setStyleSheet(lBar->styleSheet() + "border-radius: 15px; border: 1px solid rgba(255,255,255,255);");

    listFormPayment->setStyleSheet("background-color: rgba(255,255,255,240);");

    listFormPayment->setFrameShape(QFrame::NoFrame);

    listFormPayment->setModel(modelPaymentForm);

    listFormPayment->setAlternatingRowColors(true);

    // pedido

    lOrdered->setStyleSheet(lBar->styleSheet() + "border-radius: 15px; border: 1px solid rgba(255,255,255,255);");

    sbNumberCopy->setPrefix(tr("Nº de Copias "));
    sbNumberCopy->setMinimum(1);
    sbNumberCopy->setMaximum(9999999);

    cbFont->addItem(tr("Tamanho da fonte"));
    cbFont->addItem(tr("Pequeno"));
    cbFont->addItem(tr("Médio"));
    cbFont->addItem(tr("Grande"));

    cbOptionNumber->addItem(tr("Tipo de Numeração"));
    cbOptionNumber->addItem(tr("Padrão"));
    cbOptionNumber->addItem(tr("Por data"));

    sbCurrentNumber->setPrefix(tr("Nº atual "));
    sbCurrentNumber->setMaximum(9999999);

    tbNumberDescription->setStyleSheet("background-color: rgba(0,0,0,0);"
                                       "color: silver;");

    tbNumberDescription->setText("Selecione sua opção de "
                                 "numeração.");

    tbNumberDescription->setFont(f);

    tbNumberDescription->setEnabled(false);


    // Empresa

    lBusiness->setStyleSheet(lOrdered->styleSheet());

    cbStateBusiness->addItem(tr("Estado"));

    for (int count = 0; count < 27; ++count)
    {
        cbStateBusiness->addItem(tools->getIndexState(count));
    }

    // Alertas

    lAlert->setStyleSheet(lOrdered->styleSheet());

    f.setPointSize(f.pointSize() - 2);

    lTextAOrderedTime->setText(tr("Avisar-me dos pedidos agendados"));
    lTextAOrderedTime->setStyleSheet("background-color: rgba(0,0,0,0);"
                                     "color: silver;");

    lTextAOrderedTime->setAlignment(Qt::AlignCenter);
    lTextAOrderedTime->setFont(f);
    lTextAOrderedTime->setEnabled(false);

    sbDaysAlertOrder->setSuffix(tr(" minuto(s) antes da entrega"));
    sbDaysAlertOrder->setMinimum(1);
    sbDaysAlertOrder->setMaximum(9999999);

    // Backup e Restore

    lBackupRestore->setStyleSheet(lOrdered->styleSheet());
    lLastBackup->setStyleSheet(lTextAOrderedTime->styleSheet());
    lLastBackup->setAlignment(Qt::AlignCenter);
    lLastBackup->setFont(f);
    lLastBackup->setEnabled(false);

    f.setPointSize(f.pointSize() + 2);

    lOperation->setText(tr("Faça o Backup ou Restaure"));
    lOperation->setStyleSheet(lLastBackup->styleSheet());
    lOperation->setAlignment(Qt::AlignCenter);
    lOperation->setFont(f);

    f.setPointSize(f.pointSize() + 5);

    lTextValueProgress->setText("0%");
    lTextValueProgress->setAlignment(Qt::AlignHCenter);
    lTextValueProgress->setStyleSheet(lLastBackup->styleSheet());
    lTextValueProgress->setFont(f);

    progressOperation->setMaximum(100);
    progressOperation->setMinimum(0);
    progressOperation->setValue(0);
    progressOperation->setTextVisible(false);
    progressOperation->setStyleSheet("QProgressBar"
                                     " {"
                                     "border: 1px solid grey;"
                                     "border-radius: 5px;"
                                     "background-color: qlineargradient(spread:pad, x1:0.612, y1:0.278136,"
                                     "x2:0.506, y2:1, stop:0 rgba(255, 254, 254, 255), stop:1 rgba(236, 236, "
                                     "236, 255));"
                                     "}"

                                     "QProgressBar::chunk "
                                     "{"
                                     "background-color:qlineargradient(x1: 1, y1: 0, x2: 0, y2: 0,"
                                     "stop: 1 rgba(255,99,71,255), stop: 0 rgba(255,0,0,255));"
                                     "}");

    timerOperationDB->setInterval(1000);
    backup->start();

    // ---------------- Rede ---------------------

    lNetwork->setStyleSheet(lOrdered->styleSheet());

    f.setPixelSize(16);

    lIP->setFont(f);
    lIP->setAlignment(Qt::AlignCenter);
    lIP->setText(tr("IP: ") + getIP());
    lIP->setStyleSheet("color: rgba(255,255,255,240);");


    f.setPixelSize(12);

    aShowPasswordNetwork->setCheckable(true);
    aShowPasswordNetwork->setChecked(false);

    tbViewPasswordNetwork->setAutoRaise(true);
    tbViewPasswordNetwork->setPopupMode(QToolButton::MenuButtonPopup);
    tbViewPasswordNetwork->addAction(aShowPasswordNetwork);
    tbViewPasswordNetwork->setIcon(QIcon(":/Extras/question"));
    tbViewPasswordNetwork->setIconSize(QSize(28,28));

    cbTypeConnection->addItem(tr("Usar como (cliente/servidor)"));
    cbTypeConnection->addItem(tr("Cliente"));
    cbTypeConnection->addItem(tr("Servidor"));
    cbTypeConnection->setFont(f);

    eDir->setVisible(false);

    swPanels->addWidget(wFormPayment);
    swPanels->addWidget(wOrdered);
    swPanels->addWidget(wBusiness);
    swPanels->addWidget(wAlert);
    swPanels->addWidget(wBackupRestore);
    swPanels->addWidget(wAccountUser);
    swPanels->addWidget(wNetwork);

    eNameBusinnes->setNextComponent(ePhoneBusiness);
    ePhoneBusiness->setNextComponent(eAddressBusiness);
    eAddressBusiness->setNextComponent(eDistrictBusiness);
    eDistrictBusiness->setNextComponent(eCityBusiness);
    eCityBusiness->setNextComponent(cbStateBusiness);

    sbNumberCopy->setNextComponent(cbFont);

    listFormPayment->setModel(modelPaymentForm);

    onClick_itemFormPayment(); // para ter o load inicial
}

void ConfigInterface::createComponent()
{
    config = new Config();

    tools = new Tools(this);

    wBar = new QWidget(this);

    lBar = new QLabel(wBar);

    itemFormPayment = new Item(wBar,":/Config-Icon/paymentForm",tr("Formas de Pagamento"));

    itemOrdered = new Item(wBar,":/Config-Icon/ordered",tr("Pedido"));

    itemBusiness = new Item(wBar,":/Config-Icon/business",tr("Empresa"));

    itemDB = new Item(wBar,":/Config-Icon/backupRestore",tr("Backup e Restore"));

    itemAccount = new Item(wBar,":/Config-Icon/account",tr("Contas de Usuário"));

    itemAlerts = new Item(wBar,":/Config-Icon/alerts",tr("Alertas"));

    itemNetwork = new Item(wBar,":/Config-Icon/network",tr("Rede"));

    swPanels = new QStackedWidget(this);

    wFormPayment = new QWidget(this);
    lFormPayment = new QLabel(wFormPayment);
    listFormPayment = new QListView(wFormPayment);

    QList<QString> header;
    header << "Forma de pagamento";
    modelPaymentForm = new DataModel(wFormPayment, header);

    eNamePaymentForm = new Edit(wFormPayment,true,tr("Forma de pagamento"), 30);
    pbAddPaymentForm = new QPushButton(tr("Adicionar"),wFormPayment);
    pbRemovePaymentForm = new QPushButton(tr("Remover"),wFormPayment);
    pbSavePaymentForm = new QPushButton(tr("Salvar"),wFormPayment);

    wOrdered = new QWidget(this);
    lOrdered = new QLabel(wOrdered);
    sbNumberCopy = new SpinBox(wOrdered);
    cbFont = new QComboBox(wOrdered);
    cbOptionNumber = new QComboBox(wOrdered);
    sbCurrentNumber = new SpinBox(wOrdered);
    tbNumberDescription = new QTextBrowser(wOrdered);
    pbSaveOrdered = new QPushButton(tr("Salvar"),wOrdered);

    wBusiness = new QWidget(this);
    lBusiness = new QLabel(wBusiness);
    eNameBusinnes = new Edit(wBusiness,true,tr("Nome Fantasia"), 50);
    ePhoneBusiness = new phoneEdit(wBusiness,true,tr("Telefone"), 14);
    eAddressBusiness = new Edit(wBusiness,true,tr("Endereço"), 100);
    eDistrictBusiness = new Edit(wBusiness,true,tr("Bairro"), 30);
    eCityBusiness = new Edit(wBusiness,true,tr("Cidade"), 30);
    cbStateBusiness = new QComboBox(wBusiness);
    pbSaveBusiness = new QPushButton(tr("Salvar"),wBusiness);

    wAlert = new QWidget(this);
    lAlert = new QLabel(wAlert);
    lTextAOrderedTime = new QLabel(wAlert);
    sbDaysAlertOrder = new SpinBox(wAlert);
    pbSaveAlert = new QPushButton(tr("Salvar"),wAlert);

    wBackupRestore = new QWidget(this);
    lBackupRestore = new QLabel(wBackupRestore);
    lLastBackup = new QLabel(tr("Nenhum backup foi feito até hoje"),wBackupRestore);
    pbBackup = new QPushButton(tr("Criar Backup"),wBackupRestore);
    pbRestore = new QPushButton(tr("Restaurar"),wBackupRestore);
    lOperation = new QLabel(wBackupRestore);
    lTextValueProgress = new QLabel(wBackupRestore);
    progressOperation = new QProgressBar(wBackupRestore);
    timerOperationDB = new QTimer(this);
    backup = new Backup(this);

    wAccountUser = new Config_ManageUser(this);

    wNetwork = new QWidget(this);
    lNetwork = new QLabel(wNetwork);
    lIP = new QLabel(wNetwork);
    eUsernameNetwork = new Edit(wNetwork, true, tr("Usuário do banco"));
    ePasswordNetwork = new Edit(wNetwork, false, tr("Senha do banco"));
    aShowPasswordNetwork = new QAction(tr("Exibir senha"),this);
    tbViewPasswordNetwork = new QToolButton(wNetwork);
    cbTypeConnection = new QComboBox(wNetwork);
    eIPNetwork = new Edit(wNetwork, true, tr("Endereço IP do servidor"));
    ePortNetwork = new Edit(wNetwork, true, tr("Porta do firewall, exemplo: 5432"));
    eDir = new Edit(wNetwork, true, tr("Diretório do banco de dados"));
    pbSaveNetwork = new QPushButton(tr("Salvar"),wNetwork);

    bThread = new BackupThread();

    rThread = new RestoreThread();
}

void ConfigInterface::adjustLayout()
{
    wBar->setGeometry(0,0,250,this->height());

    lBar->setGeometry(0,0,wBar->width(),wBar->height());

    itemFormPayment->setGeometry(0,0,wBar->width(),40);

    itemOrdered->setGeometry(0,itemFormPayment->y() + itemFormPayment->height(),wBar->width(),40);

    itemBusiness->setGeometry(0,itemOrdered->y() + itemOrdered->height(),wBar->width(),40);

    itemAlerts->setGeometry(0,itemBusiness->y() + itemBusiness->height(),wBar->width(),40);

    itemDB->setGeometry(0,itemAlerts->y() + itemAlerts->height(),wBar->width(),40);

    itemAccount->setGeometry(0,itemDB->y() + itemDB->height(),wBar->width(),40);

    itemNetwork->setGeometry(0,itemAccount->y() + itemAccount->height(),itemAccount->width(),40);

    swPanels->setGeometry(wBar->width(),0,this->width() - wBar->width(), this->height());

    // ---------------- Forma de pagamento ----------------

    wFormPayment->setGeometry(0,0,swPanels->width(), swPanels->height());

    lFormPayment->setGeometry(wFormPayment->width()/2 - 150,50,300,370);

    listFormPayment->setGeometry(lFormPayment->x() + 20, lFormPayment->y() + 90, lFormPayment->width() - 40, lFormPayment->height() - 140);

    eNamePaymentForm->setGeometry(lFormPayment->x() + 20, lFormPayment->y() + 10, lFormPayment->width() - 40, 28);

    pbAddPaymentForm->setGeometry(lFormPayment->x() + 20, eNamePaymentForm->y() + eNamePaymentForm->height() + 14, (lFormPayment->width() - 40)/2 - 10, 28);

    pbRemovePaymentForm->setGeometry(pbAddPaymentForm->x() + pbAddPaymentForm->width() + 20, pbAddPaymentForm->y(), pbAddPaymentForm->width(), 28);

    pbSavePaymentForm->setGeometry(lFormPayment->x() + (lFormPayment->width() /2 - 45), listFormPayment->y() + listFormPayment->height() + 10, 90,30);

    // ---------------- Ordered -------------------

    wOrdered->setGeometry(0,0,swPanels->width(), swPanels->height());

    lOrdered->setGeometry(wOrdered->width()/2 - 150,50,300,340);

    sbNumberCopy->setGeometry(wOrdered->width()/2 - 100,75,200,30);

    cbFont->setGeometry(sbNumberCopy->x(),sbNumberCopy->y() + sbNumberCopy->height() + 15, sbNumberCopy->width(),30);

    cbOptionNumber->setGeometry(cbFont->x(),cbFont->y() + cbFont->height() + 15, cbFont->width(),30);

    sbCurrentNumber->setGeometry(cbOptionNumber->x(),cbOptionNumber->y() + cbOptionNumber->height() + 15, cbOptionNumber->width(),30);

    tbNumberDescription->setGeometry(sbCurrentNumber->x(),sbCurrentNumber->y() + sbCurrentNumber->height() + 15, sbCurrentNumber->width(),50);

    pbSaveOrdered->setGeometry(tbNumberDescription->x() + 55,tbNumberDescription->y() + tbNumberDescription->height() + 10,90,30);

    // ---------------- Empresa ---------------------

    wBusiness->setGeometry(0,0,swPanels->width(), swPanels->height());

    lBusiness->setGeometry(wOrdered->width()/2 - 150,50,300,350);

    eNameBusinnes->setGeometry(wBusiness->width()/2 - 100,75,200,30);

    ePhoneBusiness->setGeometry(eNameBusinnes->x(),eNameBusinnes->y() + eNameBusinnes->height() + 15, eNameBusinnes->width(),30);

    eAddressBusiness->setGeometry(ePhoneBusiness->x(),ePhoneBusiness->y() + ePhoneBusiness->height() + 15, ePhoneBusiness->width(),30);

    eDistrictBusiness->setGeometry(eAddressBusiness->x(),eAddressBusiness->y() + eAddressBusiness->height() + 15, eAddressBusiness->width(),30);

    eCityBusiness->setGeometry(eDistrictBusiness->x(),eDistrictBusiness->y() + eDistrictBusiness->height() + 15, eDistrictBusiness->width(),30);

    cbStateBusiness->setGeometry(eCityBusiness->x(),eCityBusiness->y() + eCityBusiness->height() + 15, eCityBusiness->width(),30);

    pbSaveBusiness->setGeometry(cbStateBusiness->x() + 55,cbStateBusiness->y() + cbStateBusiness->height() + 15,90,30);

    // --------------------- Alertas --------------------

    wAlert->setGeometry(0,0,swPanels->width(), swPanels->height());

    lAlert->setGeometry(wAlert->width()/2 - 150,50,300,200);

    lTextAOrderedTime->setGeometry(wAlert->width()/2 - 150,75,300,30);

    sbDaysAlertOrder->setGeometry(wAlert->width()/2 - 100,lTextAOrderedTime->y() + lTextAOrderedTime->height() + 15, 200,30);

    pbSaveAlert->setGeometry(sbDaysAlertOrder->x() + 55,sbDaysAlertOrder->y() + sbDaysAlertOrder->height() + 15,90,30);

    // --------------------- Backup e Restore --------------------------

    wBackupRestore->setGeometry(0,0,swPanels->width(), swPanels->height());

    lBackupRestore->setGeometry(wBackupRestore->width()/2 - 200,50,400,220);

    lLastBackup->setGeometry(wBackupRestore->width()/2 - 200,75,400,30);

    pbBackup->setGeometry(lBackupRestore->x() + 66,lLastBackup->y() + lLastBackup->height() + 15,100,30);

    pbRestore->setGeometry(pbBackup->x() + pbBackup->width() + 66, pbBackup->y(), 100,30);

    lOperation->setGeometry(wBackupRestore->width()/2 - 100,pbRestore->y() + pbRestore->height() + 15, 200,30);

    lTextValueProgress->setGeometry(wBackupRestore->width()/2 - 100,lOperation->y() + lOperation->height() + 15, 200,30);

    progressOperation->setGeometry(wBackupRestore->width()/2 - 175,lTextValueProgress->y() + lTextValueProgress->height() + 5, 350,10);

    // -------------------- Usuario -------------------

    wAccountUser->setGeometry(swPanels->width()/2 - 300,50,600,460);

    // -------------------- Network --------------------

    if(eDir->isVisible())
        wNetwork->setGeometry(swPanels->width()/2 - 175,50,350,350);
    else
        wNetwork->setGeometry(swPanels->width()/2 - 175,50,350,322);

    lNetwork->setGeometry(0,0,wNetwork->width(),wNetwork->height());

    lIP->setGeometry(lNetwork->width()/2 - 150,20,300,20);

    eUsernameNetwork->setGeometry(wNetwork->width()/2 - 120, lIP->y() + lIP->height() + 10, 240, 28);

    ePasswordNetwork->setGeometry(eUsernameNetwork->x(), eUsernameNetwork->y() + 40, eUsernameNetwork->width(), 28);

    tbViewPasswordNetwork->setGeometry(ePasswordNetwork->x() + ePasswordNetwork->width() - 28,ePasswordNetwork->y(), 28,28);

    cbTypeConnection->setGeometry(ePasswordNetwork->x(), ePasswordNetwork->y() + 40, ePasswordNetwork->width(), 28);

    eIPNetwork->setGeometry(ePasswordNetwork->x(), cbTypeConnection->y() + 40, ePasswordNetwork->width(), 28);

    ePortNetwork->setGeometry(ePasswordNetwork->x(), eIPNetwork->y() + 40, ePasswordNetwork->width(), 28);

    if(eDir->isVisible())
    {
        eDir->setGeometry(ePortNetwork->x(), ePortNetwork->y() + 40, ePortNetwork->width(), 28);
        pbSaveNetwork->setGeometry(wNetwork->width()/2 - 45, eDir->y() + 48, 90, 30);
    }
    else
        pbSaveNetwork->setGeometry(ePortNetwork->x() + 63, ePortNetwork->y() + ePortNetwork->height() + 15, 115, 35);

}

void ConfigInterface::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void ConfigInterface::connections()
{
    connect(itemFormPayment,SIGNAL(onClick()),SLOT(onClick_itemFormPayment()));

    connect(itemOrdered,SIGNAL(onClick()),SLOT(onClick_itemOrdered()));

    connect(itemBusiness,SIGNAL(onClick()),SLOT(onClick_itemBusiness()));

    connect(itemDB,SIGNAL(onClick()),SLOT(onClick_itemDB()));

    connect(itemAccount,SIGNAL(onClick()),SLOT(onClick_itemAccount()));

    connect(itemAlerts,SIGNAL(onClick()),SLOT(onClick_itemAlerts()));

    connect(itemNetwork,SIGNAL(onClick()),SLOT(onClick_itemNetwork()));

    connect(pbSaveBusiness, SIGNAL(clicked()), SLOT(onClick_pbSaveBusiness()));

    connect(sbCurrentNumber, SIGNAL(returnPressed()), SLOT(onClick_pbSaveOrdered()));

    connect(pbSaveOrdered, SIGNAL(clicked()), SLOT(onClick_pbSaveOrdered()));

    connect(pbSaveAlert, SIGNAL(clicked()), SLOT(onClick_pbSaveAlert()));

    connect(swPanels,SIGNAL(currentChanged(int)),SLOT(adjustLayout()));

    connect(pbBackup,SIGNAL(clicked()),SLOT(onClick_pbBackup()));

    connect(pbRestore,SIGNAL(clicked()),SLOT(onClick_pbRestore()));

    connect(timerOperationDB,SIGNAL(timeout()),this,SLOT(deadLineProgress()));

    connect(bThread, SIGNAL(finishedBackup()),this,SLOT(finished_Backup()));

    connect(rThread, SIGNAL(finishedRestore()),this,SLOT(finished_Restore()));

    connect(progressOperation,SIGNAL(valueChanged(int)),this,SLOT(setPercentTextValue()));

    connect(bThread,SIGNAL(currentPositionBackup(int)),progressOperation,SLOT(setValue(int)));

    connect(bThread,SIGNAL(totalRecordsBackup(int)),progressOperation,SLOT(setMaximum(int)));

    connect(rThread,SIGNAL(currentPositionRestore(int)),progressOperation,SLOT(setValue(int)));

    connect(rThread,SIGNAL(totalRecordsRestore(int)),progressOperation,SLOT(setMaximum(int)));

    connect(cbOptionNumber,SIGNAL(currentIndexChanged(int)),SLOT(oncurrentIndexChanged_cbOptionNumber(int)));

    connect(pbAddPaymentForm,SIGNAL(clicked()),SLOT(onClick_pbAddPaymentForm()));

    connect(pbRemovePaymentForm,SIGNAL(clicked()),SLOT(onClick_pbRemovePaymentForm()));

    connect(pbSavePaymentForm,SIGNAL(clicked()),SLOT(onClick_pbSavePaymentForm()));

    connect(pbSaveNetwork,SIGNAL(clicked()),SLOT(onClick_pbSaveNetwork()));

    connect(eNamePaymentForm, SIGNAL(returnPressed()), SLOT(onClick_pbAddPaymentForm()));

    connect(aShowPasswordNetwork,SIGNAL(triggered()),SLOT(onTriggered_aShowPasswordBusiness()));

    connect(ePasswordNetwork,SIGNAL(textChanged(QString)),SLOT(onTextChanged_ePasswordBusiness()));

    connect(cbTypeConnection,SIGNAL(currentIndexChanged(int)),SLOT(onCurrentIndexChanged_cbTypeConnection(int)));

}

void ConfigInterface::onClick_itemFormPayment()
{
    swPanels->setCurrentIndex(0);

    QList<QString> list;

    modelPaymentForm->clear();

    foreach(QString form, config->getPaymentTypes()){
        list.clear();

        list << form;

        modelPaymentForm->add(list);
    }
}

void ConfigInterface::onClick_itemOrdered()
{
    config->loadDataOrder();

    sbNumberCopy->setValue(config->numberOfCopies);

    cbFont->setCurrentIndex(config->fontSize);

    cbOptionNumber->setCurrentIndex(config->typeNumering);

    sbCurrentNumber->setValue(config->currentNumber);

    swPanels->setCurrentIndex(1);

    emit reloadOrdered();
}

void ConfigInterface::onClick_itemBusiness()
{
    config->loadDataBusiness();

    if (! config->bName.isEmpty()){
        eNameBusinnes->setText(config->bName);
        ePhoneBusiness->setText(config->bPhone);
        eAddressBusiness->setText(config->bAddress);
        eDistrictBusiness->setText(config->bDistrict);
        eCityBusiness->setText(config->bCity);

        int i = 0;

        for (int index = 0; index < cbStateBusiness->count(); ++index) {
            cbStateBusiness->setCurrentIndex(index);

            if(cbStateBusiness->currentText() == config->bState)
                i = index;
        }

        cbStateBusiness->setCurrentIndex(i);
    }else{
        eNameBusinnes->Clear();
        ePhoneBusiness->Clear();
        eAddressBusiness->Clear();
        eDistrictBusiness->Clear();
        eCityBusiness->Clear();

        cbStateBusiness->setCurrentIndex(0);
    }

    swPanels->setCurrentIndex(2);
}

void ConfigInterface::onClick_itemDB()
{
    swPanels->setCurrentIndex(4);

    lLastBackup->setText(config->dateLastBackup());
}

void ConfigInterface::onClick_itemAccount()
{
    swPanels->setCurrentIndex(5);

    wAccountUser->getAllUsers();
}

void ConfigInterface::onClick_itemAlerts()
{
    config->loadDataAlertOrder();

    sbDaysAlertOrder->setValue(config->daysAlertOrder.toInt());

    swPanels->setCurrentIndex(3);
}

void ConfigInterface::onClick_pbSaveBusiness()
{
    if (! isAvailableBusiness()){


        return;
    }

    config->bPhone = ePhoneBusiness->text();
    config->bName = eNameBusinnes->text();
    config->bAddress = eAddressBusiness->text();
    config->bDistrict = eDistrictBusiness->text();
    config->bCity = eCityBusiness->text();

    if (cbStateBusiness->currentIndex() > 0)
        config->bState = cbStateBusiness->currentText();
    else
        config->bState = "";

    if (config->insertDataBusiness())
        QMessageBox::information(this, tr("Sucesso"), tr("Dados da empresa salvos com sucesso."), QMessageBox::Ok);
    else
        QMessageBox::warning(this, tr("Erro"), tr("Dados da empresa não foram salvos com sucesso. Verifique os dados."), QMessageBox::Ok);
}

void ConfigInterface::onClick_pbSaveOrdered()
{
    if (! isAvailableOrder())
        return;

    config->numberOfCopies = sbNumberCopy->value();
    config->fontSize = cbFont->currentIndex();
    config->typeNumering = cbOptionNumber->currentIndex();
    config->currentNumber = sbCurrentNumber->value();

    if (config->insertDataOrder())
    {
        QMessageBox::information(this, tr("Sucesso"), tr("Dados salvos com sucesso."), QMessageBox::Ok);
        emit reloadOrdered();
    }
    else
        QMessageBox::warning(this, tr("Erro"), tr("Dados não foram salvos com sucesso. Verifique os dados."), QMessageBox::Ok);

}

bool ConfigInterface::isAvailableOrder()
{
    if (cbFont->currentIndex() == 0){
        QMessageBox::warning(this, tr("Dados incompletos"), tr("O tamanho da fonte é um campo obrigatório."), QMessageBox::Ok);

        cbFont->setFocus();

        return false;
    }

    if (cbOptionNumber->currentIndex() == 0){
        QMessageBox::warning(this, tr("Dados incompletos"), tr("O tipo da numeração é um campo obrigatório."), QMessageBox::Ok);

        cbOptionNumber->setFocus();

        return false;
    }

    return true;
}

bool ConfigInterface::isAvailableBusiness()
{
    if (! eNameBusinnes->isEmpty()){

        if (ePhoneBusiness->isEmpty() || eAddressBusiness->isEmpty() || eDistrictBusiness->isEmpty() || eCityBusiness->isEmpty() || cbStateBusiness->currentIndex() == 0){

            QMessageBox::warning(this, tr("Dados inválidos"), tr("Todos os campos são de preenchimento obrigatório.\nCaso não queira salvar os dados da empresa, deixe o primeiro campo em branco."));

            return false;
        }

        return true;
    }

    QMessageBox::warning(this, tr("Dados inválidos"), tr("Todos os campos são de preenchimento obrigatório.\nCaso não queira salvar os dados da empresa, deixe o primeiro campo em branco."));

    return false;
}

void ConfigInterface::onClick_pbSaveAlert()
{
    config->daysAlertOrder = QString::number(sbDaysAlertOrder->value());

    if (config->insertDataAlertOrder())
        QMessageBox::information(this, tr("Sucesso"), tr("Dados salvos com sucesso."), QMessageBox::Ok);
    else
        QMessageBox::warning(this, tr("Erro"), tr("Dados não foram salvos com sucesso. Verifique os dados."), QMessageBox::Ok);
}

void ConfigInterface::setPercentTextValue()
{
    lTextValueProgress->setText(progressOperation->text());
}

void ConfigInterface::onClick_pbBackup()
{
    if (QMessageBox::question(this,tr("Criar backup"),tr("Deseja fazer o backup e salvar os dados? Essa operação pode levar alguns minutos."),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)

    {

        fileNameDB.clear();

        fileNameDB = QFileDialog::getSaveFileName(this,
                                                  tr("Salvar Backup"), "Backup",
                                                  tr("Salvar arquivo, Files (*.ped)"));
        if(!fileNameDB.isEmpty())
        {
            QList<QString> tables;

            tables << "client";
            tables << "clientBusiness";
            tables << "configAlert";
            tables << "generalConfig";
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
            tables << "paymenttypes";
            tables << "datecloseorder";
            tables << "usersystem";
            tables << "accesslevel";
            tables << "buyaccess";
            tables << "statusServer";

            bThread->setTables(tables);

            bThread->setFilename(fileNameDB);

            isBackup = true;

            bThread->setDatabase("bdpedyrum");

            QThread *thread = new QThread;
            bThread->moveToThread(thread);

            connect(thread, SIGNAL(started()), bThread, SLOT(makeBackup()));

            thread->start(QThread::NormalPriority);
        }
    }
}

void ConfigInterface::onClick_pbRestore()
{
    if (QMessageBox::question(this,tr("Abrir backup"),tr("Deseja restaurar o backup? Essa operação pode levar alguns minutos e não deve ser interrompida."),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)

    {


        fileNameDB.clear();

        fileNameDB = QFileDialog::getOpenFileName(this,
                                                  tr("Open Backup"), "Backup",
                                                  tr("Abrir arquivo de backup, Files (*.ped*)"));

        if(!fileNameDB.isEmpty())
        {

            QList<QString> tables;

            tables << "statusServer";
            tables << "buyaccess";
            tables << "accesslevel";
            tables << "usersystem";
            tables << "datecloseorder";
            tables << "paymenttypes";
            tables << "configorder";
            tables << "business";
            tables << "tasksimple";
            tables << "schedulealert";
            tables << "schedule";
            tables << "alert";
            tables << "paymentorder";
            tables << "productitem";
            tables << "itemorder";
            tables << "employeeorder";
            tables << "ordered";
            tables << "movementcash";
            tables << "bill";
            tables << "productvalue";
            tables << "size";
            tables << "product";
            tables << "subcategory";
            tables << "category";
            tables << "statusemployee";
            tables << "employee";
            tables << "generalconfig";
            tables << "configalert";
            tables << "clientbusiness";
            tables << "client";

            rThread->setTables(tables);

            rThread->setFileName(fileNameDB);

            isBackup = false;

            rThread->setDatabase("bdpedyrum");

            QThread *thread = new QThread;
            rThread->moveToThread(thread);

            connect(thread, SIGNAL(started()), rThread, SLOT(makeRestore()));

            thread->start(QThread::HighPriority);
        }
    }
}

void ConfigInterface::deadLineProgress()
{
    int valueOld = oldProgress;
    int newValue = progressOperation->value();
    int variation = 0;
    int NSegunds = 0;
    int valueEnd = progressOperation->maximum();

    variation = newValue - valueOld;

    if (variation != 0)
        NSegunds = valueEnd / variation;

    if(isBackup)
        lOperation->setText(tr("Salvado em ") + tools->secondsToTime(NSegunds,false));
    else
        lOperation->setText(tr("Restaurando em ") + tools->secondsToTime(NSegunds,false));

    oldProgress = progressOperation->value();
}

void ConfigInterface::finished_Backup()
{
    timerOperationDB->stop();

    QMessageBox::information(this,tr("Operação bem sucedida"),tr("Backup concluido"),QMessageBox::Ok);

    lOperation->setText(tr("Faça o Backup ou Restaure"));

    progressOperation->setValue(0);

    oldProgress = 0;

    if (config->bd.search("select * from generalconfig"))
        config->bd.exec("update generalconfig set datelastbackup = now()");
    else
        config->bd.exec("insert into generalconfig (datelastbackup) values (now())");

    lLastBackup->setText(config->dateLastBackup());

}

void ConfigInterface::finished_Restore()
{
    timerOperationDB->stop();

    QMessageBox::information(this,tr("Operação bem sucedida"),tr("Restore concluido"),QMessageBox::Ok);

    lOperation->setText(tr("Faça o Backup ou Restaure"));

    progressOperation->setValue(0);

    oldProgress = 0;

    emit finishedRestore();
}

void ConfigInterface::oncurrentIndexChanged_cbOptionNumber(int index)
{
    if(index == 0)
        tbNumberDescription->setText("Selecione sua opção de "
                                     "numeração.");
    if(index == 1)
        tbNumberDescription->setText("Numeração padrão.");

    if(index == 2)
        tbNumberDescription->setText("Numeração baseada em data "
                                     "e números");
}

void ConfigInterface::onClick_pbSavePaymentForm()
{
    QModelIndex index;

    for (int i = 0; i < modelPaymentForm->rowCount(index); i++){
        config->paymentTypes << modelPaymentForm->index(i, 0).data().toString();
    }

    if (config->insertDataPayementTypes())
        QMessageBox::information(this, tr("Dados inseridos com sucesso"), tr("Formas de pagamento inseridas com sucesso."), QMessageBox::Ok);
    else
        QMessageBox::warning(this, tr("Dados não inseridos"), tr("Formas de pagamento não puderam ser inseridas."), QMessageBox::Ok);


}

void ConfigInterface::onClick_pbAddPaymentForm()
{
    if (eNamePaymentForm->isEmpty()){
        QMessageBox::warning(this, tr("Forma de pagamento inválido"), tr("Digite uma forma de pagamento para adicioná-la."), QMessageBox::Ok);

        return;
    }

    modelPaymentForm->addText(eNamePaymentForm->value());

    eNamePaymentForm->setText("");

    eNamePaymentForm->setFocus();
}

void ConfigInterface::onClick_pbRemovePaymentForm()
{
    int row = listFormPayment->currentIndex().row();

    if (row >= 0)
        modelPaymentForm->removeRow(row);
}

void ConfigInterface::onClick_itemNetwork()
{
    wAccountUser->setIdRoot(config->idRoot());

    if (wAccountUser->getIdRoot() != config->CurrentIdSystemUser){

        QMessageBox::warning(this, tr("Acesso negado."), tr("As configurações de rede só podem ser alteradas pelo administrador do sistema."), QMessageBox::Ok);

        return;

    }

    if (config->loadDataNetwork()){
        eUsernameNetwork->setText(config->username);
        ePasswordNetwork->setText(config->password);
        eIPNetwork->setText(config->ip);
        ePortNetwork->setText(config->port);

        if (config->ip == "localhost")
            cbTypeConnection->setCurrentIndex(2);
        else
            cbTypeConnection->setCurrentIndex(1);

    }else{
        eUsernameNetwork->Clear();
        ePasswordNetwork->Clear();
        eIPNetwork->Clear();
        ePortNetwork->Clear();
        cbTypeConnection->setCurrentIndex(0);
    }

    swPanels->setCurrentIndex(6);
}


QString ConfigInterface::getIP()
{
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

void ConfigInterface::onClick_pbSaveNetwork()
{
    int op = QMessageBox::question(this, tr("Alteração dos dados"), tr("Tem certeza que deseja alterar os dados de conexão ao banco de dados?"), tr("Sim"), tr("Não"));

    if (op == 0)
        if (config->saveDataAccessDatabase(eUsernameNetwork->value(), ePasswordNetwork->value(), eIPNetwork->value(), ePortNetwork->value().toInt()))
            QMessageBox::information(this, tr("Dados alterados com sucesso"), tr("Dados de conexão alterados com sucesso."), QMessageBox::Ok);
}

void ConfigInterface::onTriggered_aShowPasswordBusiness()
{
    if(!aShowPasswordNetwork->isChecked())
    {
        ePasswordNetwork->setEchoMode(Edit::Password);

        aShowPasswordNetwork->setChecked(false);
    }
    else
    {
        ePasswordNetwork->setEchoMode(Edit::Normal);

        aShowPasswordNetwork->setChecked(true);
    }
}

void ConfigInterface::onTextChanged_ePasswordBusiness()
{
    if(aShowPasswordNetwork->isChecked() == false && ePasswordNetwork->echoMode() == Edit::Normal)
        ePasswordNetwork->setEchoMode(Edit::Password);

    if(ePasswordNetwork->text() == ePasswordNetwork->valueStandart)
    {
        if(ePasswordNetwork->echoMode() == Edit::Password)
            ePasswordNetwork->setEchoMode(Edit::Normal);
    }
}


void ConfigInterface::onCurrentIndexChanged_cbTypeConnection(int index)
{
    if (index == 2)
    {
        eIPNetwork->setText("localhost");
        eIPNetwork->setEnabled(false);

        QDir dir;

        if(dir.exists("C:/Program Files/PostgreSQL"))
            eDir->setText("C:/Program Files/PostgreSQL");
        else
            eDir->setVisible(true);

        ePortNetwork->setFocus();

        disconnect(eDir, SIGNAL(returnPressed()), this, SLOT(onClick_pbSaveNetwork()));

        connect(ePortNetwork, SIGNAL(returnPressed()), this, SLOT(onClick_pbSaveNetwork()));

    }else{
        eIPNetwork->Clear();
        eIPNetwork->setEnabled(true);

        eDir->setVisible(false);

        eDir->Clear();

        connect(eDir, SIGNAL(returnPressed()), this, SLOT(onClick_pbSaveNetwork()));

        disconnect(ePortNetwork, SIGNAL(returnPressed()), this, SLOT(onClick_pbSaveNetwork()));
    }

    adjustLayout();
}
