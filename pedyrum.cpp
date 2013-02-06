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


 #include "pedyrum.h"
#include "ui_pedyrum.h"
#include <QFile>
#include <QPropertyAnimation>
#include <QDebug>
#include <QSequentialAnimationGroup>
#include <QDateTime>
#include <Object/messageobject.h>

typedef unsigned short int usint;

Pedyrum::Pedyrum(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Pedyrum)
{
    ui->setupUi(this);

    QFile file(":/QSS/standard");
    file.open(QFile::ReadOnly);

    wState = MAXIMIZED;

    styleSheet = QLatin1String(file.readAll());
    this->setStyleSheet(styleSheet);

    this->resize(1000,720);

#if defined(Q_WS_X11)
    {
        QFont f("Sans Serif", 6);
        this->setFont(f);
    }
#endif

#if defined(Q_WS_WIN)
    {
        QFont f("MS Shell Dlq 2", 9);
        this->setFont(f);
    }
#endif

    createComponent();

    connections();

    input();

    this->showMaximized();

    wConfNetwork->testConnectionFile();
}

Pedyrum::~Pedyrum()
{
    bd.sdb.removeDatabase("mainConnection");
    bd.sdb.close();

    delete ui;
}

void Pedyrum::createComponent()
{
    wTopMenu = new topMenu(this);
    wSideBar = new sideBar(this);

    swPanels = new QStackedWidget(this);
    swFinance = new QStackedWidget(this);

    wConfNetwork = new ConfNetwork(this);
    wFirstAdminInterface = new FirstAdminInterface(this);
    wLoginInterface = new LoginInterface(this);    
    wTreatmentInterface = new TreatmentInterface(this);
    wClientInterface = new ClientInterface(this);
    wProductInterface = new ProductInterface(this);
    wOrderedInterface = new OrderedInterface(this);
    wFunctionaryInterface = new FunctionaryInterface(this);
    wAlertInterface = new AlertInterface(this);
    wBillFinanceInterface = new AccountFinanceInterface(this);
    wCahsInterface = new PayBoxInterface(this);
    wConfigInterface = new ConfigInterface(this);
    wJaguatiInterface = new JaguatiInterface(this);
    wAds = new Ads(this);

    animationMenu = new GraphicEffects(this);

    trayIcon = new QSystemTrayIcon(this);

    wUserMenu = new UserMenu(this);

    wMessage = new MessageAlert(this); // sempre tem que ser o último declaro no createComponent

    timeLogout = new QTimer(this);
}

void Pedyrum::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void Pedyrum::connections()
{
    connect(wUserMenu, SIGNAL(fullScreen()), wTopMenu, SLOT(onClick_fullScreen()));

    connect(wProductInterface, SIGNAL(fullScreen()), wTopMenu, SLOT(onClick_fullScreen()));

    connect(wTreatmentInterface->wCounter->wPaymentView, SIGNAL(fullScreen()), wTopMenu, SLOT(onClick_fullScreen()));

    connect(wCahsInterface, SIGNAL(fullScreen()), wTopMenu, SLOT(onClick_fullScreen()));

    connect(wOrderedInterface, SIGNAL(fullScreen()), wTopMenu, SLOT(onClick_fullScreen()));

    connect(wAlertInterface, SIGNAL(fullScreen()), wTopMenu, SLOT(onClick_fullScreen()));

    connect(wClientInterface, SIGNAL(fullScreen()), wTopMenu, SLOT(onClick_fullScreen()));

    connect(wConfigInterface, SIGNAL(fullScreen()), wTopMenu, SLOT(onClick_fullScreen()));

    connect(wFunctionaryInterface, SIGNAL(fullScreen()), wTopMenu, SLOT(onClick_fullScreen()));

    connect(wJaguatiInterface, SIGNAL(fullScreen()), wTopMenu, SLOT(onClick_fullScreen()));

    connect(wLoginInterface, SIGNAL(fullScreen()), wTopMenu, SLOT(onClick_fullScreen()));

    connect(wTreatmentInterface->wDelivery, SIGNAL(fullScreen()), wTopMenu, SLOT(onClick_fullScreen()));

    connect(wTreatmentInterface->wCounter, SIGNAL(fullScreen()), wTopMenu, SLOT(onClick_fullScreen()));

    connect(wBillFinanceInterface, SIGNAL(fullScreen()), wTopMenu, SLOT(onClick_fullScreen()));

    connect(wConfNetwork, SIGNAL(connected()), SLOT(connected()));

    connect(wConfNetwork,SIGNAL(gotoNext(int)),swPanels,SLOT(setCurrentIndex(int)));

    connect(wFirstAdminInterface,SIGNAL(gotoNext(int)),swPanels,SLOT(setCurrentIndex(int)));

    connect(wTopMenu->menuButton,SIGNAL(mouseOnImage(bool)),SLOT(onFocus_Menu(bool)));

    connect(wTopMenu,SIGNAL(FullScreen(bool)),this,SLOT(setFullScreen(bool)));

    connect(this,SIGNAL(finishedInput()),wLoginInterface,SLOT(input()));

    connect(this, SIGNAL(finishedInput()), wTreatmentInterface, SLOT(input()));

    connect(this, SIGNAL(finishedInput()), wClientInterface, SLOT(input()));

    connect(this, SIGNAL(finishedInput()), wProductInterface, SLOT(input()));

    connect(this, SIGNAL(finishedInput()), wOrderedInterface, SLOT(input()));

    connect(this, SIGNAL(finishedInput()), wFunctionaryInterface, SLOT(input()));

    connect(this, SIGNAL(finishedInput()), wAlertInterface, SLOT(input()));

    connect(this, SIGNAL(finishedInput()), wBillFinanceInterface, SLOT(input()));

    connect(this, SIGNAL(finishedInput()), wCahsInterface, SLOT(input()));

    connect(this, SIGNAL(finishedInput()), wConfigInterface, SLOT(input()));

    connect(this, SIGNAL(finishedInput()), wJaguatiInterface, SLOT(input()));

    connect(wSideBar, SIGNAL(goToPanel(int)),this,SLOT(goToPanel(int)));

    connect(wTreatmentInterface, SIGNAL(goToPanel(int)),this,SLOT(goToPanel(int)));

    connect(wTreatmentInterface->wCounter->wPaymentView, SIGNAL(goToPanel(int)),this,SLOT(goToPanel(int)));

    connect(wClientInterface, SIGNAL(goToPanel(int)),this,SLOT(goToPanel(int)));

    connect(wProductInterface, SIGNAL(goToPanel(int)),this,SLOT(goToPanel(int)));

    connect(wOrderedInterface, SIGNAL(goToPanel(int)),this,SLOT(goToPanel(int)));

    connect(wFunctionaryInterface, SIGNAL(goToPanel(int)),this,SLOT(goToPanel(int)));

    connect(wAlertInterface, SIGNAL(goToPanel(int)),this,SLOT(goToPanel(int)));

    connect(wBillFinanceInterface, SIGNAL(goToPanel(int)),this,SLOT(goToPanel(int)));

    connect(wCahsInterface, SIGNAL(goToPanel(int)),this,SLOT(goToPanel(int)));

    connect(wConfigInterface, SIGNAL(goToPanel(int)),this,SLOT(goToPanel(int)));

    connect(wJaguatiInterface, SIGNAL(goToPanel(int)),this,SLOT(goToPanel(int)));

    connect(swPanels, SIGNAL(currentChanged(int)), this, SLOT(adjustLayout()));

    connect(wTreatmentInterface,SIGNAL(toPanel(int)),swPanels,SLOT(setCurrentIndex(int)));

    connect(wOrderedInterface,SIGNAL(gotoPanel(int,int,int)),SLOT(indexLoadOrdered(int,int,int)));

    connect(wBillFinanceInterface,SIGNAL(gotoPayBox(int)),swFinance,SLOT(setCurrentIndex(int)));

    connect(wCahsInterface,SIGNAL(gotoAccount(int)),swFinance,SLOT(setCurrentIndex(int)));

    connect(wAlertInterface->alert, SIGNAL(newsAlerts(int,bool)), wSideBar, SLOT(setAlert(int, bool)));

    connect(wSideBar, SIGNAL(showNewAlert()), this, SLOT(showNewAlert()));

    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(trayIcon_MessageCliked()));

    connect(wConfigInterface, SIGNAL(finishedRestore()), wBillFinanceInterface->bill, SLOT(refreshModel()));

    connect(wConfigInterface, SIGNAL(finishedRestore()), wAlertInterface->alert, SLOT(refreshModels()));

    connect(wConfigInterface, SIGNAL(finishedRestore()), wClientInterface->client, SLOT(refreshModel()));

    connect(wConfigInterface, SIGNAL(finishedRestore()), wOrderedInterface->ordered, SLOT(refreshModel()));

    connect(wConfigInterface, SIGNAL(finishedRestore()), wCahsInterface->cash, SLOT(refreshModel()));

    connect(wConfigInterface, SIGNAL(finishedRestore()), wProductInterface->product, SLOT(refreshModels()));

    connect(wConfigInterface, SIGNAL(finishedRestore()), wFunctionaryInterface->employee,SLOT(refreshModel()));

    connect(wTreatmentInterface->wDelivery, SIGNAL(newOrder()), wOrderedInterface->ordered, SLOT(refreshModel()));

    connect(wTreatmentInterface->wCounter, SIGNAL(newOrder()), wOrderedInterface->ordered, SLOT(refreshModel()));

    connect(wMessage,SIGNAL(hideMessage()),this,SLOT(hideAlert()));

    connect(this,SIGNAL(restoreLayoutScreen()),wSideBar,SLOT(adjustLayout()));

    connect(wAlertInterface, SIGNAL(plusAlert()), wSideBar, SLOT(plusAlert()));

    connect(wAlertInterface, SIGNAL(subAlert()), wSideBar, SLOT(subAlert()));

    connect(wLoginInterface,SIGNAL(openSystem(QString,QString,QList<bool>)),SLOT(openSystem(QString,QString,QList<bool>)));

    connect(wTopMenu,SIGNAL(userClick()),wUserMenu,SLOT(showMenu()));

    connect(wAlertInterface->alert, SIGNAL(newAlertMessage(MessageObject*)), this, SLOT(setMessageObject(MessageObject*)));

    connect(wConfigInterface,SIGNAL(reloadOrdered()),wOrderedInterface->makeHTML,SLOT(reload()));

    connect(wUserMenu,SIGNAL(configure()),SLOT(configureUser()));

    connect(wUserMenu,SIGNAL(logout()),SLOT(logout()));

    connect(wUserMenu,SIGNAL(logout()), wClientInterface, SLOT(logout()));

    connect(wUserMenu,SIGNAL(logout()), wTreatmentInterface, SLOT(logout()));

    connect(wUserMenu,SIGNAL(logout()), wProductInterface, SLOT(logout()));

    connect(wUserMenu,SIGNAL(logout()), wFunctionaryInterface, SLOT(logout()));

    connect(wUserMenu,SIGNAL(logout()), wAlertInterface, SLOT(logout()));

    connect(wUserMenu,SIGNAL(logout()), wBillFinanceInterface, SLOT(logout()));

    connect(wUserMenu,SIGNAL(logout()), wCahsInterface, SLOT(logout()));

    connect(timeLogout,SIGNAL(timeout()),SLOT(timeLogoutUpdate()));

    connect(swPanels,SIGNAL(currentChanged(int)),SLOT(adjustLayout()));
}

void Pedyrum::input()
{
    wSideBar->setAlert(0, false);

    wSideBar->setEnabled(false);
    wTopMenu->setEnabled(false);

    delayLogout = 0;

    wConfNetwork->setFont(this->font());
    wFirstAdminInterface->setFont(this->font());
    wLoginInterface->setFont(this->font());

    wClientInterface->setFont(this->font());
    wProductInterface->setFont(this->font());
    wFunctionaryInterface->setFont(this->font());
    wTreatmentInterface->setFont(this->font());
    wOrderedInterface->setFont(this->font());
    wAlertInterface->setFont(this->font());
    wBillFinanceInterface->setFont(this->font());
    wCahsInterface->setFont(this->font());
    wConfigInterface->setFont(this->font());
    wJaguatiInterface->setFont(this->font());

    wTreatmentInterface->setStyleSheet(styleSheet);

    swPanels->addWidget(wConfNetwork);
    swPanels->addWidget(wFirstAdminInterface);
    swPanels->addWidget(wLoginInterface);    
    swPanels->addWidget(wTreatmentInterface);
    swPanels->addWidget(wClientInterface);
    swPanels->addWidget(wProductInterface);
    swPanels->addWidget(wOrderedInterface);
    swPanels->addWidget(wFunctionaryInterface);
    swPanels->addWidget(wAlertInterface);
    swPanels->addWidget(swFinance);
    swPanels->addWidget(wConfigInterface);
    swPanels->addWidget(wJaguatiInterface);
    swPanels->addWidget(wAds);

    swFinance->addWidget(wBillFinanceInterface);
    swFinance->addWidget(wCahsInterface);

    trayIcon->setIcon(QIcon(":/Icon/pedyrum-icon"));
    trayIcon->show();

    wMessage->setGeometry(340,-80,280,80);

    timeLogout->setInterval(1000);
}

void Pedyrum::adjustLayout()
{
    wTopMenu->setGeometry(0,0,this->width(),28);

    if(wSideBar->height() == 0)
    {
        wSideBar->setGeometry(0,wTopMenu->height(),60,0);

        swPanels->setGeometry(0,wTopMenu->height(),this->width(),this->height() - 27);
    }
    else
    {
        wSideBar->setGeometry(0,wTopMenu->height(),60,this->height() - wTopMenu->height());

        swPanels->setGeometry(wSideBar->width(),wTopMenu->height(),this->width() - wSideBar->width(),
                              this->height() - wTopMenu->height());
    }

    wConfNetwork->setGeometry(0,0,swPanels->width(), swPanels->height());

    wFirstAdminInterface->setGeometry(0,0,swPanels->width(), swPanels->height());

    wLoginInterface->setGeometry(0,0,swPanels->width(), swPanels->height());

    wTreatmentInterface->setGeometry(0,0,swPanels->width(), swPanels->height());

    wClientInterface->setGeometry(0,0,swPanels->width(), swPanels->height());

    wProductInterface->setGeometry(0,0,swPanels->width(), swPanels->height());

    wOrderedInterface->setGeometry(0,0,swPanels->width(), swPanels->height());

    wFunctionaryInterface->setGeometry(0,0,swPanels->width(), swPanels->height());

    wAlertInterface->setGeometry(0,0,swPanels->width(), swPanels->height());

    swFinance->setGeometry(0,0,swPanels->width(), swPanels->height());

    wBillFinanceInterface->setGeometry(0,0,swFinance->width(), swFinance->height());

    wCahsInterface->setGeometry(0,0,swFinance->width(), swFinance->height());

    wConfigInterface->setGeometry(0,0,swPanels->width(), swPanels->height());

    wJaguatiInterface->setGeometry(0,0,swPanels->width(), swPanels->height());

    wMessage->setGeometry(this->width() - 340,wMessage->y(),280,80);

    wUserMenu->setGeometry(this->width() - 156,wTopMenu->height(),150,75);
}

void Pedyrum::onFocus_Menu(bool b)
{
    QPropertyAnimation *animation1;
    QPropertyAnimation *animation2;

    if(b)
    {
        if(wSideBar->height() == 0)
        {
            animation1 = animationMenu->createAnimation(wSideBar,QRect(0,wTopMenu->height(),60,0),QRect(0,wTopMenu->height(),
                                                                                                        60,this->height() - wTopMenu->height()),300,QEasingCurve::InCirc);

            animation2 = animationMenu->createAnimation(swPanels,QRect(0,wTopMenu->height(),this->width(),this->height() - wTopMenu->height()),
                                                        QRect(wSideBar->width(),wTopMenu->height(),this->width() - wSideBar->width(),
                                                              this->height() - wTopMenu->height()),300,QEasingCurve::InCirc);

            animationMenu->setGroupAnimation(animation2,animation1);
        }
        else
        {
            animation1 = animationMenu->createAnimation(wSideBar,QRect(0,wTopMenu->height(),60,this->height() - wTopMenu->height()),
                                                        QRect(0,wTopMenu->height(),60,0),300,QEasingCurve::OutCirc);

            animation2 = animationMenu->createAnimation(swPanels,QRect(wSideBar->width(),wTopMenu->height(),this->width() - wSideBar->width(),
                                                                       this->height() - wTopMenu->height()),QRect(0,wTopMenu->height(),this->width(),
                                                                                                                  this->height() - wTopMenu->height()),300,QEasingCurve::OutCirc);

            animationMenu->setGroupAnimation(animation1,animation2);
        }
    }
}

void Pedyrum::goToPanel(int currentIndex)
{
    swPanels->setCurrentIndex(currentIndex);

    reloadModels(currentIndex);
}

void Pedyrum::setFullScreen(bool b)
{
    if(b) // vai por em full
    {
        if (this->isMaximized()){
            wState = MAXIMIZED;
        }else{
            H_ = this->height();
            W_ = this->width();
            X_ = this->x();
            Y_ = this->y();

            wState = NORMAL;
        }

        this->hide();
        this->showFullScreen();

        MessageObject *message = new MessageObject(this);
        message->setText(tr("Entrando na tela cheia, precione ESC ou F11 para sair..."));
        message->setTitle(tr("Tela Cheia"));
        message->setImage(":/Extras/logo-fullscreen");
        message->setDelay(3000);
        wMessage->setMessage(message);
        showAlert();

    }
    else
    {
        if (wState == NORMAL){
            this->hide();

            this->setGeometry(this->width()/2 - W_/2, this->height()/2 - H_/2, W_, H_);

            this->show();
        }else{
            this->hide();
            this->showMaximized();
        }

        emit restoreLayoutScreen();

        hideAlert();
    }
}

void Pedyrum::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F11)
        wTopMenu->onClick_fullScreen();
}

void Pedyrum::indexLoadOrdered(int index, int index2, int idOrder)
{
    swPanels->setCurrentIndex(index);
    wTreatmentInterface->swPanel->setCurrentIndex(index2);

    if (idOrder != 0){
        if (index2 == 2){
            wTreatmentInterface->wCounter->wPaymentView->order->loadDataOrder(idOrder);
            qDebug() << "________BALCÃO________";
        }else{
            wTreatmentInterface->wDelivery->order->loadDataOrder(idOrder);
            qDebug() << "________DELIVERY________";
        }
    }

}

void Pedyrum::openLogin()
{
    swPanels->setCurrentIndex(2);
}

void Pedyrum::connected()
{
    bd.start("bdpedyrum", "mainConnection");

    QSqlDatabase *database;

    database = &bd.sdb;

    wTreatmentInterface->wTable->wGroupDesk->wTreatmentTable->order->start(database);
    wTreatmentInterface->wTable->wGroupDesk->wTreatmentTable->wPizzaMixed->pizza->start(database);
    wProductInterface->product->start(database);
    wClientInterface->client->start(database);
    wTreatmentInterface->wDelivery->order->start(database);
    wTreatmentInterface->wDelivery->wPizzaMixed->pizza->start(database);
    wTreatmentInterface->wDelivery->html.start(database);
    wTreatmentInterface->wCounter->html.start(database);
    wTreatmentInterface->wCounter->order->start(database);
    wTreatmentInterface->wCounter->wPizzaMixed->pizza->start(database);
    wTreatmentInterface->wCounter->wPaymentView->order->start(database);
    wTreatmentInterface->wTable->wPaymentView->order->start(database);
    wOrderedInterface->ordered->start(database);
    wOrderedInterface->makeHTML->start(database);
    wConfigInterface->config->start(database);
    wAlertInterface->alert->start(database);
    wBillFinanceInterface->bill->start(database);
    wFunctionaryInterface->employee->start(database);
    wCahsInterface->cash->start(database);
    wLoginInterface->login->start(database);
    wConfigInterface->wAccountUser->config->start(database);
    wTreatmentInterface->wTable->order->start(database);
    wTreatmentInterface->wTable->wGroupDesk->wTreatmentTable->order->start(database);

    emit finishedInput();

    openLogin();
}

void Pedyrum::showNewAlert()
{
    trayIcon->showMessage(tr("Novo alerta"), tr("Você possui um novo alerta.\nClique aqui para ir à tela de alertas."));
}

void Pedyrum::trayIcon_MessageCliked()
{
    swPanels->setCurrentIndex(9);
}

void Pedyrum::showAlert()
{
    QPropertyAnimation* show;

    show = new QPropertyAnimation(wMessage, "geometry");

    show->setStartValue(QRect(this->width() - 340,-80,280,80));
    show->setEndValue(QRect(this->width() - 340,60,280,80));
    show->setEasingCurve(QEasingCurve::OutSine);
    show->setDuration(1250);
    show->start();

    connect(show,SIGNAL(finished()),wMessage,SLOT(startDelay()));
}

void Pedyrum::hideAlert()
{
    if(wMessage->y() > -80)
    {
        QPropertyAnimation* hide;

        hide = new QPropertyAnimation(wMessage, "geometry");

        hide->setStartValue(QRect(this->width() - 340,60,280,80));
        hide->setEndValue(QRect(this->width() - 340,-80,280,80));
        hide->setEasingCurve(QEasingCurve::InSine);
        hide->setDuration(1250);
        hide->start();
    }
}

void Pedyrum::setMessageObject(MessageObject *message)
{
    wMessage->setMessage(message);
    showAlert();
}

void Pedyrum::openSystem(QString name, QString id, QList<bool> permissions)
{
    wSideBar->setEnabled(true);
    wTopMenu->setEnabled(true);

    wSideBar->adjustLayout();

    wTopMenu->setNickName(name);

    idCurrentSystemUser = id;

    swPanels->setCurrentIndex(12);

    wConfigInterface->config->CurrentIdSystemUser = idCurrentSystemUser;

    wConfigInterface->wAccountUser->config->CurrentIdSystemUser = idCurrentSystemUser;

    timeLogout->stop(); /// verificar nas configs.

    /*

    NÃO apagar esse comentário... ele pode ser útil para manutenção.

    0 cantreatment boolean,
    1 canclient boolean,
    2 canproduct boolean,
    3 canordered boolean,
    4 canemployee boolean,
    5 canalert boolean,'
    6 canbill boolean,
    7 cancash boolean,
    8 cancreateuser boolean,
    9 canalteraccess boolean,

    */

    wSideBar->setVisibleItens(0, permissions.value(0));
    wSideBar->setVisibleItens(1, permissions.value(1));
    wSideBar->setVisibleItens(2, permissions.value(2));
    wSideBar->setVisibleItens(3, permissions.value(3));
    wSideBar->setVisibleItens(4, permissions.value(4));
    wSideBar->setVisibleItens(5, permissions.value(5));


    wSideBar->setVisibleItens(6, true);

    if(permissions.value(6))
        wCahsInterface->pbBill->setVisible(true);
    else
        wCahsInterface->pbBill->setVisible(false);

    if(permissions.value(7))
        wBillFinanceInterface->pbPayBox->setVisible(true);
    else
        wBillFinanceInterface->pbPayBox->setVisible(false);

    if ((permissions.value(6)) && (!permissions.value(7)))
        swFinance->setCurrentIndex(0);

    if ((permissions.value(7)) && (!permissions.value(6)))
        swFinance->setCurrentIndex(1);

    if((!permissions.value(6)) && (!permissions.value(7)))
        wSideBar->setVisibleItens(6, false);

    wSideBar->adjustLayout();
    wBillFinanceInterface->adjustLayout();
    wCahsInterface->adjustLayout();
}

void Pedyrum::logout()
{
    timeLogout->stop(); /// verificar nas configs.

    swPanels->setCurrentIndex(2);

    wTopMenu->setNickName("");

    wSideBar->setEnabled(false);
    wTopMenu->setEnabled(false);

    wSideBar->setVisibleItens(0,true);
    wSideBar->setVisibleItens(1,true);
    wSideBar->setVisibleItens(2,true);
    wSideBar->setVisibleItens(3,true);
    wSideBar->setVisibleItens(4,true);
    wSideBar->setVisibleItens(5,true);
    wSideBar->setVisibleItens(6,true);
    wSideBar->setVisibleItens(7,true);
    wSideBar->setVisibleItens(8,true);

    wSideBar->adjustLayout();
}

void Pedyrum::configureUser()
{
    swPanels->setCurrentIndex(10);
    wConfigInterface->onClick_itemAccount();

    /// carrega o nome do usuário para edição.
    //wConfigInterface->wAccountUser->configureAccount("kkkk");
}

void Pedyrum::timeLogoutUpdate()
{
    int value = 60; //pegar do banco o tempo de delay;

    int segs = value * 60;

    QPoint point = this->mapFromGlobal(QCursor::pos());

    if(oldPoint == point)
        delayLogout += 1;
    else
        delayLogout = 0;

    if(segs == delayLogout)
        logout();

    oldPoint = point;
}

void Pedyrum::reloadModels(int index)
{
    switch(index)
    {

    // atendimento
    case 4:
    {
        wTreatmentInterface->wDelivery->order->bd.refreshModel("product");
        wTreatmentInterface->wDelivery->order->bd.refreshModel("client");
        wTreatmentInterface->wDelivery->order->bd.refreshModel("clientBusiness");

        wTreatmentInterface->wCounter->order->bd.refreshModel("product");
        wTreatmentInterface->wCounter->order->bd.refreshModel("client");
        wTreatmentInterface->wCounter->order->bd.refreshModel("clientBusiness");

        wTreatmentInterface->wDelivery->cbPaymentForm->clear();
        wTreatmentInterface->wCounter->wPaymentView->cbPaymentForm->clear();

        foreach(QString form, wConfigInterface->config->getPaymentTypes())
        {
            wTreatmentInterface->wCounter->wPaymentView->cbPaymentForm->add(form);
            wTreatmentInterface->wDelivery->cbPaymentForm->add(form);
        }

        break;
    }

    // client
    case 5:
    {
        wClientInterface->client->bd.refreshModel("client");
        wClientInterface->client->bd.refreshModel("clientBusiness");

        if(wClientInterface->client->typeClient == 1)
            wClientInterface->updateCountClients(QString::number(bd.countRecord("client")));
        else
            wClientInterface->updateCountClients(QString::number(bd.countRecord("clientBusiness")));

        break;
    }

    // produto
    case 6:
    {
        wProductInterface->product->bd.refreshModel("size");
        wProductInterface->product->bd.refreshModel("category");
        wProductInterface->product->bd.refreshQueryModel("product");
        break;
    }

    // pedido
    case 7:
    {
        wOrderedInterface->ordered->bd.refreshModel("ordered");
        wOrderedInterface->ordered->bd.refreshModel("statusEmployee");

        break;
    }

    // funcionario
    case 8:
    {
        wFunctionaryInterface->employee->bd.refreshModel("employee");
        break;
    }

    // alerta
    case 9:
    {
        wAlertInterface->alert->bd.refreshModel("alert");
        wAlertInterface->alert->bd.refreshModel("taskSimple");
        break;
    }

    // caixa e financeiro
    case 10:
    {
        wCahsInterface->cash->bd.refreshModel("movementCash");
        wBillFinanceInterface->bill->bd.refreshModel("bill");
        break;
    }

    }
}

void Pedyrum::closeEvent(QCloseEvent *e)
{
    //    if(QMessageBox::question(this,tr("Fechar Pedyrum"),tr("Deseja sair do Pedyrum ?"),tr("Sim"),tr("Não")) == 0)
    //    {
    delete trayIcon;

    if (bd.sdb.hostName() == "localhost")
        bd.exec(QString("update statusServer set status = '%1'").arg(encryption.encrypt("false")));

    e->accept();
    //    }
    //    else
    //        e->ignore();
}
