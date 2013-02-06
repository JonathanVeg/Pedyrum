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


 #include "Interface/clientinterface.h"
#include <QDebug>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QList>
#include <QAction>

ClientInterface::ClientInterface(QWidget *parent) :
    QWidget(parent)
{
    createComponent();
    statics();
    connections();
}

void ClientInterface::createComponent()
{
    client = new Client();

    lTop = new QLabel(this);

    lAdd = new Label(this,true,100);
    lRemove = new Label(this,true,100);
    lProfile = new Label(this,true,100);    
    textCountClient = new QTextBrowser(this);

    searchClient = new Search(this,tr("Pesquisar Clientes"));

    wSpace = new QWidget(this);
    lBackgroundSpace = new QLabel(wSpace);

    tableClient = new TableView(this);
    splitter = new QSplitter(Qt::Vertical,this);
    swPanels = new QStackedWidget(wSpace);

    swClientType = new QStackedWidget(wSpace);

    pbSelectPeople = new QPushButton(this);
    pbSelectBusiness = new QPushButton(this);

    wGroupClient = new QWidget(wSpace);
    lBackgroundPlus = new QLabel(wGroupClient);
    ePhone = new phoneEdit(wGroupClient,true,tr("Telefone"), 14);
    eCep = new Edit(wGroupClient,true,tr("Cep"), 9);
    eName = new Edit(wGroupClient,true,tr("Nome"), 60);
    eStreet = new Edit(wGroupClient,true,tr("Rua"), 50);
    eNumber = new Edit(wGroupClient,true,tr("Nº"), 5);
    eComplement = new Edit(wGroupClient,true,tr("Complemento"), 50);
    eDistrict = new Edit(wGroupClient,true,tr("Bairro"), 30);
    DSBShipping = new DoubleSpinBox(wGroupClient);
    teNotes = new QTextEdit(wGroupClient);
    pbConfirm = new QPushButton(wGroupClient);
    pbLeave = new QPushButton(wGroupClient);

    wGroupBusiness = new QWidget(wSpace);
    lBgPlusBusiness = new QLabel(wGroupBusiness);
    ePhoneBusiness = new phoneEdit(wGroupBusiness,true,tr("Telefone"), 14);
    eCepBusiness = new Edit(wGroupBusiness,true,tr("Cep"), 9);
    eNameContactBusiness = new Edit(wGroupBusiness,true,tr("Responsável"), 60);
    eNameBusiness = new Edit(wGroupBusiness,true,tr("Razão Social"), 150);
    eFantasyNameBusiness = new Edit(wGroupBusiness,true,tr("Nome Fantasia"), 100);
    eCNPJBusiness = new Edit(wGroupBusiness,true,tr("CNPJ"), 25);
    eStreetBusiness = new Edit(wGroupBusiness,true,tr("Rua"), 50);
    eNumberBusiness = new Edit(wGroupBusiness,true,tr("Nº"), 5);
    eComplementBusiness = new Edit(wGroupBusiness,true,tr("Complemento"), 50);
    eDistrictBusiness = new Edit(wGroupBusiness,true,tr("Bairro"), 30);
    DSBShippingBusiness = new DoubleSpinBox(wGroupBusiness);
    teNotesBusiness = new QTextEdit(wGroupBusiness);
    pbConfirmBusiness = new QPushButton(wGroupBusiness);
    pbLeaveBusiness = new QPushButton(wGroupBusiness);

    wGroupProfile = new QWidget(wSpace);
    tbProfile = new QTextBrowser(wGroupProfile);

    panelEffects = new GraphicEffects(this);

    actionPhone = new QAction(tr("Pesquisar por telefone"),this);
    actionName = new QAction(tr("Pesquisar por nome"),this);
}

void ClientInterface::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Delete){
        if (typeSelected == NORMAL)
            onClick_lRemove();
        else
            onClick_lRemoveBusiness();
    }

    switch (event->modifiers()){

    case Qt::ControlModifier:{
        if (event->key() == Qt::Key_S){
            if (typeSelected == NORMAL)
                onClick_pbConfirm();
            else
                onClick_pbConfirmBusiness();
        }

        if (event->key() == Qt::Key_L){
            onClick_pbLeave();
            onClick_pbLeaveBusiness();
        }

        if (event->key() == Qt::Key_P)
            onClick_lProfile();

        if (event->key() == Qt::Key_Tab)
            emit goToPanel(6);

        break;
    }
    }

    if (event->key() == Qt::Key_Backtab)
        emit goToPanel(4);

    if (event->key() == Qt::Key_F11)
        emit fullScreen();
}

void ClientInterface::input()
{
    sizePanel = 300;

    lTop->setPixmap(QPixmap(":/subMenu/background"));
    lTop->setScaledContents(true);

    lAdd->setImage(":/subMenu/buttonAdd");
    lAdd->setHover(":/subMenu/buttonAdd-hover");
    lAdd->setAlternateImage(lAdd->imageHover);
    lAdd->setToolTip(tr("Adicionar"));

    lRemove->setImage(":/subMenu/buttonRemove");
    lRemove->setHover(":/subMenu/buttonRemove-hover");
    lRemove->setAlternateImage(lRemove->imageHover);
    lRemove->setToolTip(tr("Remover cliente selecionado"));

    lProfile->setImage(":/subMenu/buttonProfile");
    lProfile->setHover(":/subMenu/buttonProfile-hover");
    lProfile->setAlternateImage(lProfile->imageHover);
    lProfile->setToolTip(tr("Visualizar Perfil"));

    QFont f;

#if defined(Q_WS_X11)
    f = QFont("Sans Serif",9);
#endif

#if defined(Q_WS_WIN)
    f = QFont("MS Shell Dlq 2", 11);
#endif

    f.setBold(true);

    textCountClient->setText(client->countClients());
    textCountClient->setObjectName("textCountClient");
    textCountClient->setToolTip(tr("Número de clientes"));
    textCountClient->setFont(f);
    textCountClient->setAlignment(Qt::AlignHCenter);
    textCountClient->setStyleSheet(this->styleSheet());

    lBackgroundSpace->setStyleSheet("background-color: rgba(123, 31, 31, 255);");

    swPanels->addWidget(swClientType);
    swPanels->addWidget(wGroupProfile);

    swClientType->addWidget(wGroupClient);
    swClientType->addWidget(wGroupBusiness);

    pbSelectPeople->setText(tr("Pessoa"));
    pbSelectPeople->setCursor(Qt::PointingHandCursor);

    pbSelectBusiness->setText(tr("Empresa"));
    pbSelectBusiness->setCursor(Qt::PointingHandCursor);

    onClick_pbSelectPeople();

    lBackgroundPlus->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.498,"
                                   "y1:0, x2:0.494636, y2:1, stop:0 rgba(255, 255, 255, 210),"
                                   "stop:0.982955 rgba(255, 255, 255, 170));"
                                   "border-radius: 5px;border: 1px solid rgba(255,255,255,255);");

    lBgPlusBusiness->setStyleSheet(lBackgroundPlus->styleSheet());

    DSBShipping->setPrefix(tr("Frete R$ "));
    DSBShipping->setFont(this->font());
    DSBShipping->setMaximum(99999999);

    teNotes->setStyleSheet("background-color: qlineargradient"
                           "(spread:pad, x1:0, y1:1, x2:0.165045,"
                           "y2:0, stop:0 rgba(254, 255, 180, 255),"
                           "stop:0.721591 rgba(255, 250, 205, 255));"
                           "border-radius: 10px;border: 1px solid #C0C0C0;");

    teNotes->setToolTip(tr("Anotações"));

    teNotesBusiness->setStyleSheet(teNotes->styleSheet());
    teNotesBusiness->setToolTip(tr("Anotações"));

    pbConfirm->setText(tr("Adicionar"));
    pbConfirm->setFont(this->font());

    pbLeave->setText(tr("Limpar"));
    pbLeave->setFont(this->font());

    splitter->insertWidget(1,wSpace);
    splitter->insertWidget(2,tableClient);
    splitter->show();
    splitter->setHandleWidth(1);
    splitter->setStretchFactor(1,2);

    eCep->setInputMask("     -   ");

    eCepBusiness->setInputMask("     -   ");

    eCNPJBusiness->setInputMask("  .   .   /    -  ");

    DSBShippingBusiness->setPrefix(tr("Frete R$ "));
    DSBShippingBusiness->setFont(this->font());
    DSBShippingBusiness->setMaximum(9999999);

    pbConfirmBusiness->setText(tr("Confirmar"));
    pbConfirmBusiness->setFont(this->font());

    pbLeaveBusiness->setText(tr("Limpar"));
    pbLeaveBusiness->setFont(this->font());

    // funções ligadas a classe client
    isAdd = true; // inicialmente, ele tem que add... caso esteja editando, esse valor muda para false
    isAddBusiness = true; // inicialmente, ele tem que add... caso esteja editando, esse valor muda para false
    //eCep->setCompleter(client->bd.getCompleter("cep"));
    //ePhone->setCompleter(client->bd.getCompleter("phone")); // não precisa disso aqui, já que não é para busca
    tableClient->setModel(client->bd.getModel("client"));

    tableClient->hideColumn(0); // oculta o ID do cliente
    tableClient->hideColumn(9); // oculta o frete do cliente - desnecessário mostrar isso na tabela

    ePhone->setUpperCase(true);
    eCep->setUpperCase(true);
    eName->setUpperCase(true);
    eStreet->setUpperCase(true);
    eNumber->setUpperCase(true);
    eComplement->setUpperCase(true);
    eDistrict->setUpperCase(true);

    ePhoneBusiness->setUpperCase(true);
    eCepBusiness->setUpperCase(true);
    eNameContactBusiness->setUpperCase(true);
    eNameBusiness->setUpperCase(true);
    eFantasyNameBusiness->setUpperCase(true);
    eCNPJBusiness->setUpperCase(true);
    eStreetBusiness->setUpperCase(true);
    eNumberBusiness->setUpperCase(true);
    eComplementBusiness->setUpperCase(true);
    eDistrictBusiness->setUpperCase(true);

    ePhone->setNextComponent(eCep);
    eCep->setNextComponent(eName);
    eName->setNextComponent(eStreet);
    eStreet->setNextComponent(eNumber);
    eNumber->setNextComponent(eComplement);
    eComplement->setNextComponent(eDistrict);
    eDistrict->setNextComponent(DSBShipping);
    DSBShipping->setNextComponent(teNotes);

    ePhoneBusiness->setNextComponent(eCepBusiness);
    eCepBusiness->setNextComponent(eNameContactBusiness);
    eNameContactBusiness->setNextComponent(eNameBusiness);
    eNameBusiness->setNextComponent(eFantasyNameBusiness);
    eFantasyNameBusiness->setNextComponent(eCNPJBusiness);
    eCNPJBusiness->setNextComponent(eStreetBusiness);
    eStreetBusiness->setNextComponent(eNumberBusiness);
    eNumberBusiness->setNextComponent(eComplementBusiness);
    eComplementBusiness->setNextComponent(eDistrictBusiness);
    eDistrictBusiness->setNextComponent(DSBShippingBusiness);
    DSBShippingBusiness->setNextComponent(teNotesBusiness);

    wSpace->setGeometry(0,0,0,0);

    searchClient->setCompleter(client->bd.getCompleter("phone"));

    tableClient->hideColumn(0); // oculta o ID do cliente
    tableClient->hideColumn(9); // oculta o frete do cliente - desnecessário mostrar isso na tabela

    actionPhone->setCheckable(true);
    actionPhone->setChecked(true);
    actionPhone->setEnabled(false);

    actionName->setCheckable(true);
    actionName->setChecked(false);
    actionName->setEnabled(true);

    searchClient->setAction(actionPhone);
    searchClient->setAction(actionName);

    ePhone->setAutomaticallyNextComponent(true);
    eCep->setAutomaticallyNextComponent(true);

    ePhoneBusiness->setAutomaticallyNextComponent(true);
    eCepBusiness->setAutomaticallyNextComponent(true);
    eCNPJBusiness->setAutomaticallyNextComponent(true);

    typeSelected = NORMAL;

    client->setTypeClient(NORMAL);

    tbProfile->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.498,"
                             "y1:0, x2:0.494636, y2:1, stop:0 rgba(255, 255, 255, 210),"
                             "stop:0.982955 rgba(255, 255, 255, 170));"
                             "border-radius: 5px;border: 1px solid rgba(255,255,255,255);");

    createProfile();
}

void ClientInterface::connections()
{
    connect(splitter, SIGNAL(splitterMoved(int, int)), SLOT(adjustLayout()));

    connect(lAdd, SIGNAL(onClick()), SLOT(onClick_lAdd()));

    connect(lRemove, SIGNAL(onClick()), SLOT(onClick_lRemove()));

    connect(lProfile, SIGNAL(onClick()), SLOT(onClick_lProfile()));

    connect(pbConfirm, SIGNAL(clicked()), SLOT(onClick_pbConfirm()));

    connect(pbLeave, SIGNAL(clicked()), SLOT(onClick_pbLeave()));

    connect(pbLeaveBusiness, SIGNAL(clicked()), SLOT(onClick_pbLeaveBusiness()));

    connect(client,  SIGNAL(count(QString)),  SLOT(updateCountClients(QString)));

    connect(tableClient,  SIGNAL(doubleClicked(QModelIndex)),  SLOT(onDoubleClick_tbClient(QModelIndex)));

    connect(tableClient, SIGNAL(clicked(QModelIndex)), SLOT(onClick_tbClient(QModelIndex)));

    connect(tableClient, SIGNAL(rowChanged(int)), SLOT(rowChanged_tbClient(int)));

    connect(swPanels, SIGNAL(currentChanged(int)), SLOT(adjustLayout()));

    //    connect(ePhone, SIGNAL(textChanged(QString)), SLOT(getDataClient()));

    //    connect(ePhoneBusiness, SIGNAL(textChanged(QString)), SLOT(getDataClientBusiness()));

    connect(searchClient, SIGNAL(textSearch(QString)), SLOT(getSearchClient(QString)));

    connect(pbSelectPeople,SIGNAL(clicked()),SLOT(onClick_pbSelectPeople()));

    connect(pbSelectBusiness,SIGNAL(clicked()),SLOT(onClick_pbSelectBusiness()));

    connect(pbConfirmBusiness, SIGNAL(clicked()), SLOT(onClick_pbConfirmBusiness()));

    connect(eCNPJBusiness, SIGNAL(textChanged(QString)), SLOT(onTextChanged_eCNPJ(QString)));

    connect(actionPhone,SIGNAL(triggered()),SLOT(onTriggered_actionPhone()));

    connect(actionName,SIGNAL(triggered()),SLOT(onTriggered_actionName()));

    //    connect(ePhone, SIGNAL(textChanged(QString)), SLOT(onTextChanged_ePhone(QString)));

    //    connect(ePhone, SIGNAL(inFocus(bool)), SLOT(inFocus_ePhone(bool)));

    //    connect(ePhone, SIGNAL(editingFinished()), SLOT(editingFinished_ePhone()));

    //    //    connect(ePhoneBusiness, SIGNAL(textChanged(QString)), SLOT(onTextChanged_ePhoneBusiness(QString)));

    //    connect(ePhoneBusiness, SIGNAL(inFocus(bool)), SLOT(inFocus_ePhoneBusiness(bool)));

    //    connect(ePhoneBusiness, SIGNAL(editingFinished()), SLOT(editingFinished_ePhoneBusiness()));
}

void ClientInterface::adjustLayout()
{
    lTop->setGeometry(0,0,this->width(),50);

    searchClient->setGeometry(this->width() - 220,11,200,28);

    splitter->setGeometry(0,lTop->height(),this->width(),this->height() - lTop->height());

    textCountClient->setGeometry(lProfile->width() + lProfile->x() + 12,11,textCountClientSize(),28);

    pbSelectPeople->setGeometry(textCountClient->width() + textCountClient->x() + 12,11,50,28);
    pbSelectBusiness->setGeometry(pbSelectPeople->x() + pbSelectPeople->width() + 2,11,50,28);

    wSpace->setGeometry(0,0,this->width(),wSpace->height());

    lBackgroundSpace->setGeometry(0,0,this->width(),wSpace->height());

    swPanels->setGeometry(0,0,wSpace->width(),wSpace->height());

    swClientType->setGeometry(0,0,this->width(),wSpace->height());

    wGroupClient->setGeometry(this->width()/2 - 300,wSpace->height()/2 - 132,600,265);

    wGroupBusiness->setGeometry(this->width()/2 - 300,wSpace->height()/2 - 180,600,360);

    wGroupProfile->setGeometry(this->width()/2 - 300,wSpace->height()/2 - 132,600,265);

    lBackgroundPlus->setGeometry(0,0,wGroupClient->width(),wGroupClient->height());

    lBgPlusBusiness->setGeometry(0,0,wGroupBusiness->width(),wGroupBusiness->height());

    tableClient->setGeometry(0,wSpace->y() + wSpace->height(),this->width(),
                             this->height() - (wSpace->height() + lTop->height()));

    if(typeSelected == NORMAL)
    {
        tableClient->setColumnWidth(1,tableClient->width() * 0.15);
        tableClient->setColumnWidth(2,tableClient->width() * 0.1);
        tableClient->setColumnWidth(3,tableClient->width() * 0.26);
        tableClient->setColumnWidth(4,tableClient->width() * 0.07);
        tableClient->setColumnWidth(5,tableClient->width() * 0.1);
        tableClient->setColumnWidth(6,tableClient->width() * 0.16);
        tableClient->setColumnWidth(7,tableClient->width() * 0.08);
        tableClient->setColumnWidth(8,tableClient->width() * 0.08);
    }
    else
    {
        tableClient->setColumnWidth(1,tableClient->width() * 0.14);
        tableClient->setColumnWidth(2,tableClient->width() * 0.14);
        tableClient->setColumnWidth(3,tableClient->width() * 0.14);
        tableClient->setColumnWidth(4,tableClient->width() * 0.08);
        tableClient->setColumnWidth(5,tableClient->width() * 0.08);
        tableClient->setColumnWidth(6,tableClient->width() * 0.24);
        tableClient->setColumnWidth(7,tableClient->width() * 0.09);
        tableClient->setColumnWidth(8,tableClient->width() * 0.09);
    }
}

void ClientInterface::statics()
{
    lAdd->setGeometry(12,11,30,28);
    lRemove->setGeometry(lAdd->width() + lAdd->x() + 12,11,30,28);
    lProfile->setGeometry(lRemove->width() + lRemove->x() + 12,11,30,28);

    // ----------------------- Plus e Edit -----------------------

    wGroupClient->setMinimumSize(600,265);
    wGroupClient->setMaximumSize(600,265);

    ePhone->setGeometry(20,22,100,25);
    eCep->setGeometry(ePhone->x() + ePhone->width() + 5,22,95,25);
    eName->setGeometry(20,ePhone->y() + ePhone->height() + 7,200,25);
    eStreet->setGeometry(20,eName->y() + eName->height() + 7,200,25);
    eNumber->setGeometry(20,eStreet->y() + eStreet->height() + 7,80,25);
    eComplement->setGeometry(eNumber->x() + eNumber->width() + 5,eNumber->y(),115,25);
    eDistrict->setGeometry(20,eNumber->y() + eNumber->height() + 7,200,25);
    DSBShipping->setGeometry(20,eDistrict->y() + eDistrict->height() + 7,200,25);
    pbConfirm->setGeometry(20,DSBShipping->y() + DSBShipping->height() + 7,97,30);
    pbLeave->setGeometry(pbConfirm->width() + pbConfirm->x() + 6,pbConfirm->y(),97,30);

    teNotes->setGeometry(230,22,wGroupClient->width() - 250, pbConfirm->y() + pbConfirm->height() - 22);

    wGroupBusiness->setMinimumSize(600,360);
    wGroupBusiness->setMaximumSize(600,360);

    ePhoneBusiness->setGeometry(20,22,100,25);
    eCepBusiness->setGeometry(ePhoneBusiness->x() + ePhoneBusiness->width() + 5,22,95,25);
    eNameContactBusiness->setGeometry(20,eCepBusiness->y() + eCepBusiness->height() + 7,200,25);
    eNameBusiness->setGeometry(20,eNameContactBusiness->y() + eNameContactBusiness->height() + 7,200,25);
    eFantasyNameBusiness->setGeometry(20,eNameBusiness->y() + eNameBusiness->height() + 7,200,25);
    eCNPJBusiness->setGeometry(20,eFantasyNameBusiness->y() + eFantasyNameBusiness->height() + 7,200,25);
    eStreetBusiness->setGeometry(20,eCNPJBusiness->y() + eCNPJBusiness->height() + 7,200,25);
    eNumberBusiness->setGeometry(20,eStreetBusiness->y() + eStreetBusiness->height() + 7,80,25);
    eComplementBusiness->setGeometry(eNumberBusiness->x() + eNumberBusiness->width() + 5,eNumberBusiness->y(),115,25);
    eDistrictBusiness->setGeometry(20,eNumberBusiness->y() + eNumberBusiness->height() + 7,200,25);
    DSBShippingBusiness->setGeometry(20,eDistrictBusiness->y() + eDistrictBusiness->height() + 7,200,25);

    pbConfirmBusiness->setGeometry(20,DSBShippingBusiness->y() + DSBShippingBusiness->height() + 7,97,30);
    pbLeaveBusiness->setGeometry(pbConfirmBusiness->width() + pbConfirmBusiness->x() + 6,pbConfirmBusiness->y(),97,30);

    teNotesBusiness->setGeometry(230,22,wGroupClient->width() - 250, pbConfirmBusiness->y() + pbConfirmBusiness->height() - 22);

    // ----------------------- Profile -----------------------

    wGroupProfile->setMinimumSize(600,265);
    wGroupProfile->setMaximumSize(600,265);

    tbProfile->setGeometry(0,0,600,265);

}

void ClientInterface::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void ClientInterface::hidePanel(bool b)
{
    if(typeSelected == NORMAL)
        sizePanel = 300;
    else
        sizePanel = 400;

    if(b)
    {
        animaUp = panelEffects->createAnimation(wSpace,QRect(wSpace->x(),
                                                             wSpace->y(), wSpace->width(), 0),800,QEasingCurve::InBack);

        animaDown = panelEffects->createAnimation(tableClient,QRect(tableClient->x(),
                                                                    0, tableClient->width(), this->height() - lTop->height()),800,QEasingCurve::InBack);
    }
    else
    {
        animaDown  = panelEffects->createAnimation(wSpace,QRect(wSpace->x(),
                                                                wSpace->y(), wSpace->width(), sizePanel),800,QEasingCurve::OutBack);

        animaUp  = panelEffects->createAnimation(tableClient,QRect(tableClient->x(),sizePanel,
                                                                   tableClient->width(),this->height() - (lTop->height() + sizePanel)),800,QEasingCurve::OutBack);
    }

    connect(animaDown,SIGNAL(valueChanged(QVariant)),SLOT(adjustSplitter()));

    connect(animaUp,SIGNAL(valueChanged(QVariant)),SLOT(adjustSplitter()));

    animaUp->start();

    animaDown->start();
}

void ClientInterface::gotoPanel(int index)
{
    bool b = (wSpace->height() > 0)? true:false;

    nextPanelIndex = index;

    if(index == swPanels->currentIndex())
    {
        hidePanel(b);
    }
    else
    {
        if(b)
        {
            hidePanel(b);

            connect(animaUp,SIGNAL(finished()),SLOT(nextPanel()));
        }
        else
        {
            swPanels->setCurrentIndex(index);

            hidePanel(false);
        }
    }

    switch(index)
    {
    case 0:{
        lAdd->setAlternate(!b);
        break;
    }

    case 1:{
        lProfile->setAlternate(!b);
        break;
    }

    }
}

void ClientInterface::nextPanel()
{
    disconnect(animaUp,SIGNAL(finished()),this,SLOT(nextPanel()));

    switch(swPanels->currentIndex())
    {
    case 0:lAdd->setAlternate(false);
        break;

    case 1:lProfile->setAlternate(false);
        break;
    }

    switch(nextPanelIndex)
    {
    case 0:lAdd->setAlternate(true);
        break;

    case 1:lProfile->setAlternate(true);
        break;
    }

    hidePanel(false);

    swPanels->setCurrentIndex(nextPanelIndex);
}

void ClientInterface::adjustSplitter()
{
    QList<int> list;

    list << wSpace->height() << tableClient->height();

    splitter->setSizes(list);

    adjustLayout();
}

void ClientInterface::onClick_lAdd()
{
    if(swPanels->currentIndex() == 0)
        onClick_pbLeave();

    gotoPanel(0);
}

void ClientInterface::onClick_lRemove()
{
    if (typeSelected == NORMAL){
        QModelIndex index = tableClient->currentIndex();

        int row = index.row();

        if (row >= 0){

            QString name = tableClient->model()->index(index.row(),1).data().toString();

            QString message = "Tem certeza que deseja remover o(a) cliente: " + name;

            if (QMessageBox::question(this, tr("Deletar cliente"),message, tr("Remover"), tr("Cancelar")) == 0){
                client->setTypeClient(1);
                client->remove(tableClient->model()->index(index.row(),0).data().toInt());

                client->refreshModel();

                onClick_lAdd();
            }
        }else{
            QMessageBox::warning(this, tr("Selecione um cliente"), tr("Selecione um cliente para excluir."), tr("Ok"));
        }
    }else
        onClick_lRemoveBusiness();
}

void ClientInterface::onClick_lRemoveBusiness()
{
    QModelIndex index = tableClient->currentIndex();

    int row = index.row();

    if (row >= 0){

        QString name = tableClient->model()->index(index.row(),1).data().toString();

        QString message = "Tem certeza que deseja remover o(a) cliente: " + name;

        if (QMessageBox::question(this, tr("Deletar cliente"),message, tr("Remover"), tr("Cancelar")) == 0){
            client->setTypeClient(2);

            client->remove(tableClient->model()->index(index.row(),0).data().toInt());

            client->refreshModel();

            onClick_lAdd();
        }
    }else{
        QMessageBox::warning(this, tr("Selecione um cliente"), tr("Selecione um cliente para excluir."), tr("Ok"));
    }
}

void ClientInterface::onClick_lProfile()
{
    sizePanel = 300;

    if (tableClient->currentIndex().row() >= 0){
        if (typeSelected == NORMAL)
            client->loadData(tableClient->model()->index(tableClient->currentIndex().row(), 0).data().toInt());
        else
            client->loadDataBusiness(tableClient->model()->index(tableClient->currentIndex().row(), 0).data().toInt());

        createProfile(client);
    }else
        createProfile();

    gotoPanel(1);
}

void ClientInterface::onClick_pbConfirm()
{
    if (isAvailable())
    {
        client->setTypeClient(1);
        client->setPhone(ePhone->text());
        client->setName(eName->text());
        client->setCEP(eCep->value());
        client->setStreet(eStreet->text());
        client->setNumber(eNumber->text());
        client->setComplement(eComplement->value());
        client->setDistrict(eDistrict->text());
        client->setShipping(DSBShipping->value());
        client->setNote(teNotes->toPlainText());

        if (isAdd)
            client->insert();
        else{

            client->update(idClientUpdate);
        }

        this->onClick_pbLeave();

        client->refreshModel();
    }
}

void ClientInterface::onClick_pbConfirmBusiness(){

    if (isAvailableBusiness())
    {
        client->setTypeClient(2);
        client->setPhone(ePhoneBusiness->text());
        client->setNameRespondible(eNameContactBusiness->text());
        client->setCEP(eCepBusiness->value());
        client->setStreet(eStreetBusiness->text());
        client->setNumber(eNumberBusiness->text());
        client->setComplement(eComplementBusiness->value());
        client->setDistrict(eDistrictBusiness->text());
        client->setShipping(DSBShippingBusiness->value());
        client->setNote(teNotesBusiness->toPlainText());

        client->setCorporateName(eNameBusiness->text());
        client->setCNPJ(eCNPJBusiness->text());
        client->setFantasyName(eFantasyNameBusiness->text());

        if (isAddBusiness)
            client->insert();
        else{
            QModelIndex index = tableClient->currentIndex();
            client->update(idClientUpdate); // onde ta esse 1, vc tem q mandar o id do client
        }

        this->onClick_pbLeaveBusiness();

        client->refreshModel();
    }
}

void ClientInterface::onClick_pbLeave()
{
    isAdd = true;

    tableClient->clearSelection();

    pbConfirm->setText(tr("Confirmar"));
    pbLeave->setText("Limpar");

    ePhone->Clear();
    eCep->Clear();
    eName->Clear();
    eStreet->Clear();
    eNumber->Clear();
    eComplement->Clear();
    eDistrict->Clear();

    DSBShipping->setValue(0);

    teNotes->clear();

    teNotes->setFocus();
}

void ClientInterface::onClick_pbLeaveBusiness()
{
    isAddBusiness = true;
    tableClient->clearSelection();

    pbConfirmBusiness->setText(tr("Confirmar"));
    pbLeaveBusiness->setText(tr("Limpar"));

    ePhoneBusiness->Clear();
    eCepBusiness->Clear();
    eNameContactBusiness->Clear();
    eNameBusiness->Clear();
    eFantasyNameBusiness->Clear();
    eCNPJBusiness->Clear();
    eStreetBusiness->Clear();
    eNumberBusiness->Clear();
    eComplementBusiness->Clear();
    eDistrictBusiness->Clear();

    DSBShippingBusiness->setValue(0);
    teNotesBusiness->clear();
    teNotesBusiness->setFocus();
}

void ClientInterface::updateCountClients(QString qnt)
{
    QFont f;

#if defined(Q_WS_X11)
    f = QFont("Sans Serif",9);
#endif

#if defined(Q_WS_WIN)
    f = QFont("MS Shell Dlq 2", 11);
#endif

    f.setBold(true);

    textCountClient->setText(qnt);
    textCountClient->setFont(f);
    textCountClient->setAlignment(Qt::AlignHCenter);

    textCountClient->setGeometry(textCountClient->x(),textCountClient->y(),
                                 textCountClientSize(),textCountClient->height());

    adjustLayout();
}

void ClientInterface::getDataClient(QString phone){
    // mesmo esquema do CEP, botarei so qnd tiver 8 digitos de telefone, para evitar buscas atoa
    // não padronizei com 8, ja que sp tera numeros de 9 digitos
    if (client->getDataClient(phone)){

        ePhone->setText(phone);

        eName->setText(client->getName());

        if (client->getCEP().isEmpty())
            eCep->setText(eCep->valueStandart);
        else
            eCep->setText(client->getCEP());

        eStreet->setText(client->getStreet());
        eDistrict->setText(client->getDistrict());
        eNumber->setText(client->getNumber());

        if (client->getComplement().isEmpty())
            eComplement->setText(eComplement->valueStandart);
        else
            eComplement->setText(client->getComplement());

        DSBShipping->setValue(client->getShipping());
        teNotes->setText(client->getNote());

        for (int i = 0; i < tableClient->model()->rowCount(); i++){
            if (tableClient->model()->index(i,2).data().toString() == ePhone->text()){
                tableClient->setCurrentIndex(tableClient->model()->index(i,0));
                break;
            }
        }

        isAdd = false;

        pbConfirm->setText(tr("Editar"));
        pbLeave->setText(tr("Cancelar"));

        return;
    }

    // limpa tudo
    eName->setText(eName->valueStandart);
    eCep->setText(eCep->valueStandart);
    eNumber->setText(eNumber->valueStandart);
    eComplement->setText(eComplement->valueStandart);
    eStreet->setText(eStreet->valueStandart);
    eDistrict->setText(eDistrict->valueStandart);
    DSBShipping->setValue(0);
    teNotes->clear();

    isAdd = true;

    pbConfirm->setText(tr("Confirmar"));
    pbLeave->setText(tr("Limpar"));

    client->clearFilter();

    tableClient->clearSelection();
}

void ClientInterface::getDataClientBusiness(QString phoneBusiness){
    if (client->getDataClientBusiness(phoneBusiness)){
        eNameContactBusiness->setText(client->getName());

        ePhoneBusiness->setText(phoneBusiness);

        if (client->getCEP().isEmpty())
            eCepBusiness->setText(eCepBusiness->valueStandart);

        else
            eCepBusiness->setText(client->getCEP());

        eStreetBusiness->setText(client->getStreet());
        eDistrictBusiness->setText(client->getDistrict());
        eNumberBusiness->setText(client->getNumber());

        if (client->getComplement().isEmpty())
            eComplementBusiness->setText(eComplementBusiness->valueStandart);
        else
            eComplementBusiness->setText(client->getComplement());

        DSBShippingBusiness->setValue(client->getShipping());
        teNotesBusiness->setText(client->getNote());

        for (int i = 0; i < tableClient->model()->rowCount(); i++){
            if (tableClient->model()->index(i,2).data().toString() == ePhoneBusiness->text()){
                tableClient->setCurrentIndex(tableClient->model()->index(i,0));
                break;
            }
        }

        eNameBusiness->setText(client->getCorporateName());
        eFantasyNameBusiness->setText(client->getFantasyName());
        eCNPJBusiness->setText(client->getCNPJ());

        isAdd = false;

        pbConfirmBusiness->setText(tr("Editar"));
        pbLeaveBusiness->setText(tr("Cancelar"));

        return;
    }

    // limpa os campos
    eNameContactBusiness->setText(eNameContactBusiness->valueStandart);
    eCepBusiness->setText(eCepBusiness->valueStandart);
    eNumberBusiness->setText(eNumberBusiness->valueStandart);
    eComplementBusiness->setText(eComplementBusiness->valueStandart);
    eStreetBusiness->setText(eStreetBusiness->valueStandart);
    eDistrictBusiness->setText(eDistrictBusiness->valueStandart);
    DSBShippingBusiness->setValue(0);
    eNameBusiness->setText(eNameBusiness->valueStandart);
    eFantasyNameBusiness->setText(eFantasyNameBusiness->valueStandart);
    eCNPJBusiness->setText(eCNPJBusiness->valueStandart);

    teNotesBusiness->clear();

    isAdd = true;

    pbConfirmBusiness->setText(tr("Confirmar"));
    pbLeaveBusiness->setText(tr("Limpar"));

    client->clearFilterBusiness();

    tableClient->clearSelection();
}

void ClientInterface::onClick_tbClient(QModelIndex index){
}

void ClientInterface::rowChanged_tbClient(int row){
    if (client->getDataClient(tableClient->model()->index(row, 2).data().toString())){
        createProfile(client);
    }
}

void ClientInterface::onDoubleClick_tbClient(QModelIndex index)
{
    if (typeSelected == NORMAL){
        getDataClient(tableClient->model()->index(index.row(), 2).data().toString());

        isAdd = false;

        pbConfirm->setText(tr("Editar"));
        pbLeave->setText(tr("Cancelar"));
    }else{
        getDataClientBusiness(tableClient->model()->index(index.row(), 5).data().toString());

        isAddBusiness = false;

        pbConfirmBusiness->setText(tr("Editar"));
        pbLeaveBusiness->setText(tr("Cancelar"));
    }

    if (tableClient->currentIndex().row() >= 0){
        if (typeSelected == NORMAL)
            client->loadData(tableClient->model()->index(tableClient->currentIndex().row(), 0).data().toInt());
        else
            client->loadDataBusiness(tableClient->model()->index(tableClient->currentIndex().row(), 0).data().toInt());

        createProfile(client);
    }else
        createProfile();

    idClientUpdate = tableClient->model()->index(index.row(),0).data().toInt();

    bool b = (wSpace->height() > 0)? true:false;

    if (! b)
        gotoPanel(0);
}

bool ClientInterface::isAvailable()
{
    if (ePhone->isEmpty()){
        QMessageBox::warning(this, tr("Telefone inválido"), tr("Por favor, digite um número de telefone."), tr("Ok"));

        ePhone->setFocus();

        return false;
    }else{

        if(client->bd.search(QString("select * from client where phone = '%1' and idclient != '%2'").arg(ePhone->value(), QString::number(idClientUpdate)))){
            QMessageBox::warning(this, tr("Telefone inválido"), tr("Esse telefone já está cadastrado no sistema."), tr("Ok"));

            ePhone->setFocus();

            return false;
        }
    }

    if (eName->isEmpty()){
        QMessageBox::warning(this, tr("Nome inválido"), tr("Por favor, digite um nome para o cliente."), tr("Ok"));

        eName->setFocus();

        return false;

    }

    if (eStreet->isEmpty()){
        QMessageBox::warning(this, tr("Rua inválido"), tr("Por favor, digite uma rua para o cliente."), tr("Ok"));

        eStreet->setFocus();

        return false;

    }

    if (eNumber->isEmpty()){
        QMessageBox::warning(this, tr("Número inválido"), tr("Por favor, digite um número."), tr("Ok"));

        eNumber->setFocus();

        return false;
    }

    return true;
}

bool ClientInterface::isAvailableBusiness(){
    if (ePhoneBusiness->isEmpty()){
        QMessageBox::warning(this, tr("Telefone inválido"), tr("Por favor, digite um número de telefone."), tr("Ok"));

        ePhoneBusiness->setFocus();

        return false;
    }else{
        if(client->bd.search(QString("select * from clientbusiness where phone = '%1' and idclient != '%2'").arg(ePhoneBusiness->value(), QString::number(idClientUpdate)))){
            QMessageBox::warning(this, tr("Telefone inválido"), tr("Esse telefone já está cadastrado no sistema."), tr("Ok"));

            ePhone->setFocus();

            return false;
        }
    }

    if (eNameContactBusiness->isEmpty()){
        QMessageBox::warning(this, tr("Nome inválido"), tr("Por favor, digite um nome para o cliente."), tr("Ok"));

        eNameContactBusiness->setFocus();

        return false;

    }

    if (eNameBusiness->isEmpty()){
        QMessageBox::warning(this, tr("Razão social inválida"), tr("Por favor, digite a razão social."), tr("Ok"));

        eNameBusiness->setFocus();

        return false;
    }

    if (eFantasyNameBusiness->isEmpty()){
        QMessageBox::warning(this, tr("Nome fantasia inválido"), tr("Por favor, digite no,e fantasia."), tr("Ok"));

        eFantasyNameBusiness->setFocus();

        return false;
    }

    if (eCNPJBusiness->isEmpty()){
        QMessageBox::warning(this, tr("CNPJ inválido"), tr("Por favor, digite um CNPJ válido."), tr("Ok"));

        eCNPJBusiness->setFocus();

        return false;
    }

    if (eStreetBusiness->isEmpty()){
        QMessageBox::warning(this, tr("Rua inválido"), tr("Por favor, digite uma rua para o cliente."), tr("Ok"));

        eStreetBusiness->setFocus();

        return false;

    }

    if (eNumberBusiness->isEmpty()){
        QMessageBox::warning(this, tr("Número inválido"), tr("Por favor, digite um número."), tr("Ok"));

        eNumberBusiness->setFocus();

        return false;
    }


    return true;
}

void ClientInterface::getSearchClient(QString text)
{
    //    ePhone->Clear();
    //    ePhoneBusiness->Clear();
    //    onClick_pbLeave();
    //    onClick_pbLeaveBusiness();

    if (actionPhone->isChecked()){
        if (text.isEmpty())
            client->clearFilter();
        else
            client->setFilter("lower(phone) like lower('" + text + "%')");
    }else{
        if (text.isEmpty())
            client->clearFilter();
        else{
            if (typeSelected == NORMAL)
                client->setFilter("lower(name) like lower('" + text + "%')");
            else
                client->setFilter("lower(fantasyname) like lower('" + text + "%')");
        }
    }
}

void ClientInterface::onClick_pbSelectPeople()
{
    if(wSpace->height() == 0)
        sizePanel = 0;
    else
        sizePanel = 300;

    wSpace->setGeometry(wSpace->x(),wSpace->y(),wSpace->width(),sizePanel);

    QString white = "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            "stop: 0 rgba(255, 255, 255, 50), stop: 1 rgba(0, 0, 0, 50));"
            "color: rgb(50,50,50);"
            "border: 1px solid rgba(0, 0, 0, 50);"
            "border-radius: 5px;";

    QString red = "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            "stop: 1 rgba(165, 47, 31, 240), stop: 0 rgba(159, 45, 29, 240));"
            "color: rgb(230,230,230);"
            "border: 1px solid rgba(123, 31, 31, 255);"
            "border-radius: 5px;";

    pbSelectPeople->setStyleSheet(red);

    pbSelectBusiness->setStyleSheet(white);

    swClientType->setCurrentIndex(0);

    tableClient->setModel(client->bd.getModel("client"));

    searchClient->setCompleter(client->bd.getCompleter("phone"));

    typeSelected = NORMAL;

    client->setTypeClient(NORMAL);

    updateCountClients(textCountClient->toPlainText());

    adjustSplitter();

}

void ClientInterface::onClick_pbSelectBusiness()
{
    if(wSpace->height() == 0)
        sizePanel = 0;
    else
        sizePanel = 400;

    wSpace->setGeometry(wSpace->x(),wSpace->y(),wSpace->width(),sizePanel);

    QString white = "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            "stop: 0 rgba(255, 255, 255, 50), stop: 1 rgba(0, 0, 0, 50));"
            "color: rgb(50,50,50);"
            "border: 1px solid rgba(0, 0, 0, 50);"
            "border-radius: 5px;";

    QString red = "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            "stop: 1 rgba(165, 47, 31, 240), stop: 0 rgba(159, 45, 29, 240));"
            "color: rgb(230,230,230);"
            "border: 1px solid rgba(123, 31, 31, 255);"
            "border-radius: 5px;";

    pbSelectBusiness->setStyleSheet(red);

    pbSelectPeople->setStyleSheet(white);

    swClientType->setCurrentIndex(1);

    tableClient->setModel(client->bd.getModel("clientBusiness"));

    searchClient->setCompleter(client->bd.getCompleter("phoneBusiness"));

    typeSelected = BUSINESS;

    client->setTypeClient(BUSINESS);

    adjustLayout();

    adjustSplitter();
}

void ClientInterface::onTextChanged_ePhone(QString text){

    QString phone = ePhone->text().replace("-", "").replace("(", "").replace(")", ""); // valor sem máscara

    if (phone.length() == 4){
        phone += "-";
        ePhone->setText(phone);
        return;
    }

    if (phone.length() > 4 && phone.length() <= 9){
        phone = phone.mid(0, 4) + "-" + phone.mid(4);
        ePhone->setText(phone);
        return;
    }

    if (phone.length() > 9){
        phone = "(" + phone.mid(0, 2) + ")" + phone.mid(2, 4) + "-" + phone.mid(6);
        ePhone->setText(phone);
        return;
    }
}

void ClientInterface::onTextChanged_ePhoneBusiness(QString text){
    if (ePhoneBusiness->text().length() <= 10)
        ePhoneBusiness->setInputMask("    -      ");
    else
        ePhoneBusiness->setInputMask("(  )    -    ");
}

void ClientInterface::onTextChanged_eCNPJ(QString text){
    if (text.length() == 18){
        if (tools.validateCNPJ(text.replace(".", "").replace("/", "").replace("-", ""))){
            eCNPJBusiness->setToolTip("CNPJ válido");

            eCNPJBusiness->setStyleSheet(CSS_VALID_EDIT_CLIENT);

            eStreetBusiness->setFocus();
        }else{
            eCNPJBusiness->setToolTip("CNPJ inválido");

            eCNPJBusiness->setStyleSheet(CSS_INVALID_EDIT_CLIENT);
        }
    }else{
        eCNPJBusiness->setStyleSheet(CSS_VALID_EDIT_CLIENT);
    }
}

void ClientInterface::onTriggered_actionPhone()
{
    actionName->setChecked(false);
    actionName->setEnabled(true);

    actionPhone->setChecked(true);
    actionPhone->setEnabled(false);

    if (typeSelected == NORMAL)
        client->bd.getCompleter("phone")->setCompletionColumn(2);
    else
        client->bd.getCompleter("phoneBusiness")->setCompletionColumn(5);
}

void ClientInterface::onTriggered_actionName()
{
    actionName->setChecked(true);
    actionName->setEnabled(false);

    actionPhone->setChecked(false);
    actionPhone->setEnabled(true);

    if (typeSelected == NORMAL)
        client->bd.getCompleter("phone")->setCompletionColumn(1);
    else
        client->bd.getCompleter("phoneBusiness")->setCompletionColumn(3);
}

void ClientInterface::inFocus_ePhone(bool focus){
    if (! ePhone->isEmpty())
        ePhone->setText(ePhone->text().replace("(", "").replace(")", "").replace("-", ""));
}

void ClientInterface::editingFinished_ePhone(){
    if (! ePhone->isEmpty()){
        QString phone = ePhone->text().replace("(", "").replace(")", "").replace("-", "");
        QString nPhone;

        if (phone.length() <= 9){
            nPhone = phone.mid(0, 4) + "-" + phone.mid(4);

            ePhone->setText(nPhone);

            return;
        }

        nPhone = "(" + phone.mid(0, 2) + ")" + phone.mid(2, 4) + "-" + phone.mid(6);

        ePhone->setText(nPhone);
    }
}

void ClientInterface::inFocus_ePhoneBusiness(bool focus){
    if (! ePhoneBusiness->isEmpty())
        ePhoneBusiness->setText(ePhoneBusiness->text().replace("(", "").replace(")", "").replace("-", ""));
}

void ClientInterface::editingFinished_ePhoneBusiness(){
    if (! ePhoneBusiness->isEmpty()){
        QString phone = ePhoneBusiness->text().replace("(", "").replace(")", "").replace("-", "");
        QString nPhone;

        if (phone.length() <= 9){
            nPhone = phone.mid(0, 4) + "-" + phone.mid(4);

            ePhoneBusiness->setText(nPhone);

            return;
        }

        nPhone = "(" + phone.mid(0, 2) + ")" + phone.mid(2, 4) + "-" + phone.mid(6);

        ePhoneBusiness->setText(nPhone);
    }
}

void ClientInterface::createProfile(Client *c)
{
    QString html;

    QString temp;

    temp.setNum(c->getShipping());

    temp == tools.convertMoney(temp);

    QString address;

    if(c->complement == "")
        address = c->street + ", " + c->number + ", "  + c->district;
    else
        address = c->street + ", " + c->number + ", " + c->complement + ", " + c->district;

    if(c->cep != "")
        address += ", "+ tr("CEP: ") + c->cep;

    QString note = "";

    if(c->getNote() != "")
        note = "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p>"
                "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; color:#0f0f0f;\">" + tr("Anotação") + "</span></p>"
                "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:12pt; color:#0f0f0f;\"></p>"
                "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; color:#8c8c8c;\">" + c->getNote() + "</span></p>"
                "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:12pt; color:#8c8c8c;\"></p>";

    if(typeSelected == NORMAL)
    {
        html =  "<html><head></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">"
                "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:20pt;\"></p>"
                "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:20pt;\">" + c->getName() + "</span></p>"
                "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:20pt;\"></p>"
                "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p>"
                "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">" + tr("Telefone: ") +  "</span><span style=\" font-size:12pt;"
                "color:#8a8a8a;\">" + c->getPhone() + " - </span><span style=\" font-size:12pt;\">" + tr("Frete: ") + "</span><span style=\" font-size:12pt; color:#8a8a8a;\">" + temp + "</span></p>"
                "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:12pt; color:#8a8a8a;\"></p>"
                "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:12pt; color:#8a8a8a;\"></p>"
                "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; color:#8c8c8c;\">" + address + "</span></p>"
                "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p>";

        html += note;
        html += "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p></body></html>";

    }
    else
    {

        html =  "<html><head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">"
                "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p>"
                "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:20pt;\">" + c->getFantasyName() + "</span></p>"
                "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:20pt;\"></p>"
                "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">" + tr("Responsável:") +  "</span><span style=\" font-size:12pt; color:#8a8a8a;\">" + c->getNameRespondible()  + "</span></p>"
                "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">" + tr("Razão social:") + "</span><span style=\" font-size:12pt; color:#8a8a8a;\">" + c->getCorporateName() +  "</span></p>"
                "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">" + tr("Telefone: ") + "</span><span style=\" font-size:12pt; color:#8a8a8a;\">" + c->getPhone() + "</span></p>"
                "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">" + tr("CNPJ: ") + "</span><span style=\" font-size:12pt; color:#8a8a8a;\">" + c->getCNPJ() + "</span></p>"
                "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">" + tr("Frete: ") + "</span><span style=\" font-size:12pt; color:#8a8a8a;\">" + temp + "</span></p>"
                "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:12pt; color:#8a8a8a;\"></p>"
                "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; color:#8c8c8c;\">" + address + "</span></p>"
                "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:12pt; color:#8c8c8c;\"></p>"
                "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p>";

        html += note;
        html += "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p></body></html>";
    }

    tbProfile->setHtml(html);
}

void ClientInterface::createProfile()
{
    QString html;

    QString text = tr("Selecione um cliente");

    html += "<html><head></head><body><br><br>"
            "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px;"
            " margin-left:0px; margin-right:0px; -qt-block-indent:0;"
            "text-indent:0px;\"><span style=\" font-size:20pt;\">" + text + "</span></p>"
            "</body></html>";

    tbProfile->setHtml(html);
}

int ClientInterface::textCountClientSize()
{
    short int pointSize;

#if defined(Q_WS_X11)
    pointSize = 9;
#endif

#if defined(Q_WS_WIN)
    pointSize = 11;
#endif

    int value;

    if(textCountClient->toPlainText().length() <= 1)
        value = 30;
    else
        value = 12 + textCountClient->toPlainText().length() * pointSize;

    return value;
}

void ClientInterface::logout(){
    onClick_pbLeave();
    onClick_pbLeaveBusiness();
}
