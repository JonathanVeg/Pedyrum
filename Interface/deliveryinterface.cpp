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


 #include "Interface/deliveryinterface.h"
#include <QDebug>

DeliveryInterface::DeliveryInterface(QWidget *parent) :
    QWidget(parent)
{
    createComponent();
    statics();
    connections();

    totalValueOrder = 0;

    idClient = 0;

    isAdd = true;
}

void DeliveryInterface::createComponent()
{
    wScheduleX = 0;

    order = new Order("connectionDelivery");

    wProductNotes = new QWidget(this);
    lProductNotes = new QLabel(wProductNotes);
    lTextNotesProduct = new QLabel(wProductNotes);
    teProductNotes = new QTextEdit(wProductNotes);

    wClientNotes = new QWidget(this);
    lClientNotes = new QLabel(wClientNotes);
    lTextNotesClient = new QLabel(wClientNotes);
    teClientNotes = new QTextEdit(wClientNotes);

    wClient = new QWidget(this);
    lClient = new QLabel(wClient);

    wProduct = new QWidget(this);
    lProduct = new QLabel(wProduct);

    lNoteProduct = new Label(wProduct,true,100);

    lNoteClient = new Label(wClient,true,100);

    pbSelectPeople = new QPushButton(wClient);
    pbSelectBusiness = new QPushButton(wClient);

    swSelectClient = new QStackedWidget(wClient);

    wSelectPeople = new QWidget(wClient);
    wSelectBusiness = new QWidget(wClient);

    ePhone = new phoneEdit(wSelectPeople,true,tr("Telefone"), 14);
    eCep = new Edit(wSelectPeople,true,tr("Cep"), 9);
    eName = new Edit(wSelectPeople,true,tr("Nome"), 60);
    eStreet = new Edit(wSelectPeople,true,tr("Rua"), 50);
    eNumber = new Edit(wSelectPeople,true,tr("Nº"), 5);
    eComplement = new Edit(wSelectPeople,true,tr("Complemento"), 50);
    eDistrict = new Edit(wSelectPeople,true,tr("Bairro"), 30);

    ePhoneBusiness = new phoneEdit(wSelectBusiness,true,tr("Telefone"), 14);
    eCepBusiness = new Edit(wSelectBusiness,true,tr("Cep"), 9);
    eNameContactBusiness = new Edit(wSelectBusiness,true,tr("Responsável"), 60);
    eNameBusiness = new Edit(wSelectBusiness,true,tr("Razão Social"), 150);
    eFantasyNameBusiness = new Edit(wSelectBusiness,true,tr("Nome Fantasia"), 100);
    eCNPJBusiness = new Edit(wSelectBusiness,true,tr("CNPJ"), 25);
    eStreetBusiness = new Edit(wSelectBusiness,true,tr("Rua"), 50);
    eNumberBusiness = new Edit(wSelectBusiness,true,tr("Nº"), 5);
    eComplementBusiness = new Edit(wSelectBusiness,true,tr("Complemento"), 50);
    eDistrictBusiness = new Edit(wSelectBusiness,true,tr("Bairro"), 30);

    pbLeaveClient = new QPushButton(wClient);

    searchProduct = new Search(wProduct,tr("Pesquisar Produtos"));
    productPreview = new QTextBrowser(wProduct);
    DSBValueProduct = new DoubleSpinBox(wProduct);
    cbSize = new ComboBox(wProduct);
    sbAmount = new SpinBox(wProduct);
    lPizza = new Label(wProduct,true,100);

    pbConfirmProduct = new QPushButton(wProduct);
    pbLeaveProduct = new QPushButton(wProduct);

    wItens = new QWidget(this);
    lItens = new QLabel(wItens);
    pbSaveOrdered = new QPushButton(wItens);
    pbRemoveItem = new QPushButton(wItens);
    pbClearOrdered = new QPushButton(wItens);
    pbCancelOrdered = new QPushButton(wItens);
    tableItem = new TableView(wItens);

    wValues = new QWidget(this);

    lValues = new QLabel(wValues);
    lValuesField = new QLabel(wValues);

    swValuesFields = new QStackedWidget(wValues);

    wValuesFields = new QWidget(wValues);
    wPaymentForm = new QWidget(wValues);

    pbNumberPayment = new QPushButton(wValues);

    lSubtotalOrdered = new QLabel(wValuesFields);
    DSBShipping = new DoubleSpinBox(wValuesFields);
    lTotalOrdered = new QLabel(wValuesFields);
    isPaid = new QCheckBox(wValuesFields);
    DSBPaid = new DoubleSpinBox(wValuesFields);
    lTransshipment = new  QLabel(wValuesFields);

    lPaymentFormTotal = new QLabel(wPaymentForm);
    cbPaymentForm = new ComboBox(wPaymentForm);
    DSBValuePaymentForm = new DoubleSpinBox(wPaymentForm);

    pbAddPaymentForm = new QPushButton(wPaymentForm);
    pbRemovePaymentForm = new QPushButton(wPaymentForm);

    tablePaymentForm =  new TableView(wPaymentForm);

    transition = new GraphicEffects(this);

    hide = new QPropertyAnimation();
    show = new QPropertyAnimation();

    wSchedule = new QWidget(this);
    lSchedule = new Label(wSchedule,true,100);
    lScheduleBg = new QLabel(wSchedule);
    eDateDelivery = new Edit(wSchedule,true,tr("Data"));
    eHourDelivery = new Edit(wSchedule,true,tr("Hora"));

    QList<QString> headers;

    headers << tr("Nome") << tr("Tamanho") << tr("Quantidade") << tr("Valor") << tr("Anotação");
    dataModelItems = new DataModel(this, headers);

    headers.clear();

    headers << tr("Forma de pagamento") << tr("Valor");
    dataModelPayment = new DataModel(this, headers);

    wPizzaMixed = new PizzaMixed();

    actionName = new QAction(tr("Pesquisar por nome"),this);
    actionNickName = new QAction(tr("Pesquisar por Apelido"),this);
}

void DeliveryInterface::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Delete)
        onClick_pbRemoveItem();

    if (event->key() == Qt::Key_F11)
        emit fullScreen();

    switch (event->modifiers()){

    case Qt::ControlModifier:{
        if (event->key() == Qt::Key_S)
            onClick_pbSaveOrdered();

        if (event->key() == Qt::Key_L)
            onClick_pbClearOrdered();

        if (event->key() == Qt::Key_C)
            onClick_pbCancelOrdered();

        if (event->key() == Qt::Key_M)
            onClick_lPizza();

        if (event->key() == Qt::Key_D)
            onClick_lSchedule();

        break;
    }
    }
}

void DeliveryInterface::statics()
{
    lClient->setGeometry(0,0,350,310);
    lProduct->setGeometry(0,0,350,310);

    pbSelectPeople->setGeometry(125,15,50,23);
    pbSelectBusiness->setGeometry(pbSelectPeople->x() + pbSelectPeople->width(),pbSelectPeople->y(),50,23);

    swSelectClient->setGeometry(0,53,350,250);

    wSelectPeople->setGeometry(0,0,swSelectClient->width(),swSelectClient->height());
    wSelectBusiness->setGeometry(0,0,swSelectClient->width(),swSelectClient->height());

    ePhone->setGeometry(50,0,140,26);
    eCep->setGeometry(ePhone->width() + ePhone->x() + 10,ePhone->y(),100,26);
    eName->setGeometry(50,ePhone->y() + 40, 250,26);
    eStreet->setGeometry(50,eName->y() + 40, 250,26);
    eNumber->setGeometry(50,eStreet->y() + 40, 70,26);
    eComplement->setGeometry(eNumber->width() + eNumber->x() + 10,eNumber->y(), 170,26);
    eDistrict->setGeometry(50,eNumber->y() + 40, 250,26);

    ePhoneBusiness->setGeometry(25,0,160,26);
    eCepBusiness->setGeometry(ePhoneBusiness->width() + ePhoneBusiness->x() + 10,ePhoneBusiness->y(),130,26);

    eNameContactBusiness->setGeometry(25,ePhoneBusiness->y() + 40,145,26);
    eNameBusiness->setGeometry(eNameContactBusiness->x() + eNameContactBusiness->width() + 10,eNameContactBusiness->y(), 145,26);

    eFantasyNameBusiness->setGeometry(25,eNameContactBusiness->y() + 40,145,26);
    eCNPJBusiness->setGeometry(eFantasyNameBusiness->x() + eFantasyNameBusiness->width() + 10,eFantasyNameBusiness->y(), 145,26);

    eStreetBusiness->setGeometry(25,eFantasyNameBusiness->y() + 40,220,26);
    eNumberBusiness->setGeometry(eStreetBusiness->x() + eStreetBusiness->width() + 10,eStreetBusiness->y(), 70,26);

    eComplementBusiness->setGeometry(25,eStreetBusiness->y() + 40,100,26);
    eDistrictBusiness->setGeometry(eComplementBusiness->x() + eComplementBusiness->width() + 10,eComplementBusiness->y(), 190,26);

    pbLeaveClient->setGeometry(130,275,90,28);

    searchProduct->setGeometry(25,15,300,28);

    productPreview->setGeometry(10,searchProduct->height() + 50,210,170);
    cbSize->setGeometry(productPreview->width() + 15,70,110,26);
    DSBValueProduct->setGeometry(cbSize->x(),cbSize->y() + 40, cbSize->width(), 26);
    sbAmount->setGeometry(DSBValueProduct->x(),DSBValueProduct->y() + 40, DSBValueProduct->width(), 26);
    lPizza->setGeometry(sbAmount->x() + 10,190,40,40);

    lNoteProduct->setGeometry(lPizza->x() + lPizza->width() + 10,190,40,40);

    lNoteClient->setGeometry(300,5,40,40);

    pbLeaveProduct->setGeometry(57,275,90,28);
    pbConfirmProduct->setGeometry(203,275,90,28);

    pbNumberPayment->setGeometry(75,5,150,28);

    lSubtotalOrdered->setGeometry(50,20,250,28);

    DSBShipping->setGeometry(50,lSubtotalOrdered->y() + lSubtotalOrdered->height() + 12,200,28);

    lTotalOrdered->setGeometry(50,DSBShipping->y() + DSBShipping->height() + 12,250,28);

    isPaid->setGeometry(50,lTotalOrdered->y() + lTotalOrdered->height() + 12,250,28);

    DSBPaid->setGeometry(50,isPaid->y() + isPaid->height() + 12,200,28);

    lTransshipment->setGeometry(50,DSBPaid->y() + DSBPaid->height() + 12,250,28);

    lPaymentFormTotal->setGeometry(50,25,250,26);

    cbPaymentForm->setGeometry(20,lPaymentFormTotal->y() + lPaymentFormTotal->height() + 15,125,28);

    DSBValuePaymentForm->setGeometry(cbPaymentForm->x() + cbPaymentForm->width() + 10,cbPaymentForm->y(),125,28);

    pbAddPaymentForm->setGeometry(cbPaymentForm->x(), cbPaymentForm->y() + 43,125,28);

    pbRemovePaymentForm->setGeometry(pbAddPaymentForm->x() + pbAddPaymentForm->width() + 10,pbAddPaymentForm->y(),125,28);

    lProductNotes->setGeometry(0,0,350,310);
    lTextNotesProduct->setGeometry(0,10,350,25);
    teProductNotes->setGeometry(15,35,320,260);

    lClientNotes->setGeometry(0,0,350,310);
    lTextNotesClient->setGeometry(0,10,350,25);
    teClientNotes->setGeometry(15,35,320,260);

    lSchedule->setGeometry(0,0,50,50);
    lScheduleBg->setGeometry(lSchedule->width(),0,280,50);
    eDateDelivery->setGeometry(lSchedule->width() + 15,12,120,26);
    eHourDelivery->setGeometry(eDateDelivery->x() + eDateDelivery->width() + 15,12,120,26);

}

void DeliveryInterface::connections()
{
    connect(DSBValueProduct, SIGNAL(valueChanged(double)), SLOT(valueChanged_DSBValueProduct(double)));

    connect(this,SIGNAL(setInput()),wPizzaMixed,SLOT(input()));

    connect(pbSelectPeople, SIGNAL(clicked()), SLOT(onClick_pbSelectPeople()));

    connect(pbSelectBusiness, SIGNAL(clicked()), SLOT(onClick_pbSelectBusiness()));

    connect(pbLeaveClient, SIGNAL(clicked()), SLOT(onClick_pbLeaveClient()));

    connect(lPizza, SIGNAL(onClick()), SLOT(onClick_lPizza()));

    connect(pbConfirmProduct, SIGNAL(clicked()), SLOT(onClick_pbConfirmProduct()));

    connect(pbLeaveProduct, SIGNAL(clicked()), SLOT(onClick_pbLeaveProduct()));

    connect(pbSaveOrdered, SIGNAL(clicked()), SLOT(onClick_pbSaveOrdered()));

    connect(pbRemoveItem, SIGNAL(clicked()), SLOT(onClick_pbRemoveItem()));

    connect(pbClearOrdered, SIGNAL(clicked()), SLOT(onClick_pbClearOrdered()));

    connect(pbCancelOrdered, SIGNAL(clicked()), SLOT(onClick_pbCancelOrdered()));

    connect(pbNumberPayment, SIGNAL(clicked()), SLOT(onClick_pbNumberPayment()));

    connect(lNoteProduct, SIGNAL(onClick()), SLOT(onClick_lNoteProduct()));

    connect(lNoteClient, SIGNAL(onClick()), this, SLOT(onClick_lNoteClient()));

    connect(lSchedule, SIGNAL(onClick()), this, SLOT(onClick_lSchedule()));

    connect(ePhone, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged_ePhone(QString)));
    connect(ePhoneBusiness, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged_ePhoneBusiness(QString)));

    connect(eCep, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged_eCep(QString)));
    connect(eCepBusiness, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged_eCepBusiness(QString)));

    connect(searchProduct, SIGNAL(textSearch(QString)), SLOT(getSearchProduct(QString)));

    connect(sbAmount, SIGNAL(valueChanged(int)), SLOT(valueChanged_sbAmount(int)));

    connect(cbSize, SIGNAL(currentIndexChanged(int)), SLOT(currentIndexChanged_cbSize(int)));

    connect(tableItem, SIGNAL(doubleClicked(QModelIndex)), SLOT(doubleClicked_tableItem(QModelIndex)));

    connect(DSBShipping, SIGNAL(valueChanged(double)), SLOT(valueChanged_dsbDeliveryRate(double)));

    connect(DSBPaid, SIGNAL(valueChanged(double)), SLOT(valueChanged_dsbPaid(double)));

    connect(wPizzaMixed, SIGNAL(emitPizzaMista(QList<int>,QList<QString>,QList<QString>,QString,int,double,QString,bool,int)), this, SLOT(slotPizzaMista(QList<int>,QList<QString>,QList<QString>,QString,int,double,QString,bool,int)));

    connect(this, SIGNAL(editPizzaMixed(QList<int>,QList<QString>,QList<QString>,QString,int,double,QString,int)), wPizzaMixed, SLOT(editPizzaMixed(QList<int>,QList<QString>,QList<QString>,QString,int,double,QString,int)));

    connect(pbAddPaymentForm, SIGNAL(clicked()), SLOT(onClick_pbAddPaymentForm()));

    connect(pbRemovePaymentForm, SIGNAL(clicked()), SLOT(onClick_pbRemovePaimentForm()));

    connect(DSBValuePaymentForm, SIGNAL(returnPressed()), SLOT(onClick_pbAddPaymentForm()));

    connect(eHourDelivery, SIGNAL(returnPressed()), SLOT(onClick_lSchedule()));

    connect(actionName,SIGNAL(triggered()),SLOT(onTriggered_actionName()));

    connect(actionNickName,SIGNAL(triggered()),SLOT(onTriggered_actionNickName()));

    connect(sbAmount, SIGNAL(returnPressed()), SLOT(onClick_pbConfirmProduct()));

    connect(order, SIGNAL(loadComplete()), SLOT(loadDataOrder()));

}

void DeliveryInterface::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void DeliveryInterface::adjustLayout()
{
    if(isSchedule)
        wScheduleX = this->width() - 50;
    else
        wScheduleX = this->width() - 330;


    wSchedule->setGeometry(wScheduleX,130,330,50);

    short int space = (this->width() - 700) / 3;

    if((isNoteClient == true) && (isNoteProduct == true)) // sem anotação
    {
        wClient->setGeometry(space,15,350,310);
        wProduct->setGeometry(space * 2 + 350,15,350,310);

        wClientNotes->setGeometry(wClient->geometry());
        wProductNotes->setGeometry(wProduct->geometry());

    }
    else
    {
        if(isNoteProduct  == false)
        {
            wProduct->setGeometry(space * 2 + 350,15,350,310);

            wClient->setGeometry(wProduct->geometry());

            wProductNotes->setGeometry(wProduct->x() - 360,wProduct->y(),
                                       wProduct->width(),wProduct->height());

            wClientNotes->setGeometry(wClient->geometry());

        }
        else
        {
            wClient->setGeometry(space,15,350,310);

            wProduct->setGeometry(wClient->geometry());

            wClientNotes->setGeometry(wClient->x() + 360,wClient->y(),
                                      wClient->width(),wClient->height());

            wProductNotes->setGeometry(wProduct->geometry());
        }
    }

    short int tempY = wClient->y() * 2 + wClient->height() + 25;

    wItens->setGeometry(0,tempY,this->width() - 300,this->height() - tempY);
    lItens->setGeometry(0,0,wItens->width(),wItens->height());

    pbSaveOrdered->setGeometry(wItens->width()/2 - 195,5,90,28);
    pbRemoveItem->setGeometry(pbSaveOrdered->x() + pbSaveOrdered->width() + 10,5,90,28);
    pbClearOrdered->setGeometry(pbRemoveItem->x() + pbRemoveItem->width() + 10,5,90,28);
    pbCancelOrdered->setGeometry(pbClearOrdered->x() + pbClearOrdered->width() + 10,5,90,28);

    tableItem->setGeometry(10,38,wItens->width() -  20,wItens->height() - 43);

    tableItem->setColumnWidth(0,(wItens->width() - 20 ) * 0.25 );
    tableItem->setColumnWidth(1,(wItens->width() - 20 ) * 0.15 );
    tableItem->setColumnWidth(2,(wItens->width() - 20 ) * 0.15 );
    tableItem->setColumnWidth(3,(wItens->width() - 20 ) * 0.15 );
    tableItem->setColumnWidth(4,(wItens->width() - 20 ) * 0.3  - 1);

    wValues->setGeometry(wItens->x() + wItens->width(),tempY,300,this->height() - tempY);

    lValues->setGeometry(0,0,wValues->width(), wValues->height());

    lValuesField->setGeometry(5,38,lValues->width() - 10, lValues->height() - 43);

    swValuesFields->setGeometry(0,38,wValues->width(), wValues->height() - 43);

    wValuesFields->setGeometry(0,0,swValuesFields->width(), swValuesFields->height());

    wPaymentForm->setGeometry(0,0,swValuesFields->width(), swValuesFields->height());

    tablePaymentForm->setGeometry(20,pbAddPaymentForm->y() + 43,260,wPaymentForm->height() - (pbAddPaymentForm->y() + 60));    
    tablePaymentForm->setColumnWidth(0,tablePaymentForm->width() * 0.65 -1);
    tablePaymentForm->setColumnWidth(1,tablePaymentForm->width() * 0.35 -1);

    wPizzaMixed->setGeometry(this->width()/2 - 300,this->height()/2 - 190,600,380);
}

void DeliveryInterface::input()
{
    isNoteClient = true;
    isNoteProduct = true;

    isSchedule = true;

    wScheduleX = 0;

    eCep->setInputMask("     -   ");

    eCepBusiness->setInputMask("     -   ");

    lSchedule->setImage(":/treatment/schedule");
    lSchedule->setHover(":/treatment/schedule-hover");

    lScheduleBg->setPixmap(QPixmap(":/treatment/scheduleBG"));
    lScheduleBg->setScaledContents(true);

    lClient->setPixmap(QPixmap(":/treatment/fieldBackground"));
    lClient->setScaledContents(true);

    lProduct->setPixmap(QPixmap(":/treatment/fieldBackground"));
    lProduct->setScaledContents(true);

    lNoteProduct->setToolTip(tr("Anotações"));
    lNoteClient->setToolTip(tr("Anotações"));

    lNoteProduct->setImage(":/treatment/icoNote");
    lNoteProduct->setHover(":/treatment/icoNote-hover");
    lNoteProduct->setAlternateImage(":/treatment/icoNote-alternate");

    lNoteClient->setImage(":/treatment/icoNote");
    lNoteClient->setHover(":/treatment/icoNote-hover");
    lNoteClient->setAlternateImage(":/treatment/icoNote-alternate");

    swSelectClient->addWidget(wSelectPeople);
    swSelectClient->addWidget(wSelectBusiness);

    pbLeaveClient->setObjectName("pbLeaveClient");
    pbLeaveClient->setStyleSheet(this->styleSheet());
    pbLeaveClient->setFont(this->font());
    pbLeaveClient->setText(tr("Limpar"));
    pbLeaveClient->setCursor(Qt::PointingHandCursor);

    pbConfirmProduct->setObjectName("pbConfirmProduct");
    pbConfirmProduct->setStyleSheet(this->styleSheet());
    pbConfirmProduct->setFont(this->font());
    pbConfirmProduct->setText(tr("Confirmar"));
    pbConfirmProduct->setCursor(Qt::PointingHandCursor);

    pbLeaveProduct->setObjectName("pbLeaveProduct");
    pbLeaveProduct->setStyleSheet(this->styleSheet());
    pbLeaveProduct->setFont(this->font());
    pbLeaveProduct->setText(tr("Limpar"));
    pbLeaveProduct->setCursor(Qt::PointingHandCursor);

    pbSelectPeople->setToolTip(tr("Cliente Pessoa Física"));
    pbSelectPeople->setText(tr("Pessoa"));
    pbSelectPeople->setObjectName("pbSPeopleDelivery");
    pbSelectPeople->setCursor(Qt::PointingHandCursor);
    pbSelectPeople->setStyleSheet(this->styleSheet());

    pbSelectBusiness->setToolTip(tr("Cliente Corporativo"));
    pbSelectBusiness->setText(tr("Empresa"));
    pbSelectBusiness->setObjectName("pbSBusinessDelivery");
    pbSelectBusiness->setCursor(Qt::PointingHandCursor);
    pbSelectBusiness->setStyleSheet(this->styleSheet());

    productPreview->setStyleSheet("background-color: rgba(0,0,0,0);"
                                  "color: rgba(255,255,255,240);");

    DSBValueProduct->setPrefix(tr("R$ "));

    lPizza->setImage(":/treatment/icopizza");
    lPizza->setHover(":/treatment/icopizza-hover");

    sbAmount->setPrefix(tr("Quant "));
    sbAmount->setFont(this->font());
    sbAmount->setMinimum(1);
    sbAmount->setValue(1);
    sbAmount->setMaximum(99999);

    wClientNotes->hide();

    wProductNotes->hide();

    lProductNotes->setPixmap(QPixmap(":/treatment/field-notes-Background"));
    lTextNotesProduct->setText(tr("Anotações"));
    lTextNotesProduct->setFont(this->font());
    lTextNotesProduct->setAlignment(Qt::AlignHCenter);
    lTextNotesProduct->setStyleSheet("color: rgba(254, 255, 180, 255);");

    teProductNotes->setStyleSheet("background-color: qlineargradient"
                                  "(spread:pad, x1:0, y1:1, x2:0.165045,"
                                  "y2:0, stop:0 rgba(254, 255, 180, 255),"
                                  "stop:0.721591 rgba(255, 250, 205, 255));"
                                  "border-radius: 5px;border: 1px solid #C0C0C0;");

    teProductNotes->setFont(this->font());

    lClientNotes->setPixmap(QPixmap(":/treatment/field-notes-Background"));
    lTextNotesClient->setText(tr("Anotações"));
    lTextNotesClient->setAlignment(Qt::AlignHCenter);
    lTextNotesClient->setFont(this->font());
    lTextNotesClient->setStyleSheet("color: rgba(254, 255, 180, 255);");

    teClientNotes->setStyleSheet(teProductNotes->styleSheet());
    teClientNotes->setFont(this->font());

    QFont f;

#if defined(Q_WS_X11)
    f.setPointSize(9);
#endif

#if defined(Q_WS_WIN)
    f.setPointSize(12);
#endif

    productPreview->setFont(f);
    setPreview();

#if defined(Q_WS_X11)
    f.setPointSize(8);
#endif

#if defined(Q_WS_WIN)
    f.setPointSize(10);
#endif

    lItens->setStyleSheet("background-color: rgb(245,245,245);");

    pbSaveOrdered->setObjectName("pbSaveOrdered");
    pbSaveOrdered->setStyleSheet(this->styleSheet());
    pbSaveOrdered->setText(tr("Salvar"));
    pbSaveOrdered->setFont(this->font());
    pbSaveOrdered->setCursor(Qt::PointingHandCursor);

    pbRemoveItem->setObjectName("pbRemoveItem");
    pbRemoveItem->setStyleSheet(this->styleSheet());
    pbRemoveItem->setText(tr("Remover"));
    pbRemoveItem->setFont(this->font());
    pbRemoveItem->setCursor(Qt::PointingHandCursor);

    pbClearOrdered->setObjectName("pbClearOrdered");
    pbClearOrdered->setStyleSheet(this->styleSheet());
    pbClearOrdered->setText(tr("Limpar"));
    pbClearOrdered->setFont(this->font());
    pbClearOrdered->setCursor(Qt::PointingHandCursor);

    pbCancelOrdered->setObjectName("pbCancelOrdered");
    pbCancelOrdered->setStyleSheet(this->styleSheet());
    pbCancelOrdered->setText(tr("Cancelar"));
    pbCancelOrdered->setFont(this->font());
    pbCancelOrdered->setCursor(Qt::PointingHandCursor);

    lValues->setStyleSheet("background-color: rgb(245,245,245);");

    lValuesField->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.498,"
                                "y1:0, x2:0.494636, y2:1, stop:0 rgba(0, 0, 0, 210),"
                                "stop:0.982955 rgba(0, 0, 0, 170));"
                                "border-radius: 5px;border: 1px solid rgba(0,0,0,0);");

    pbNumberPayment->setText(tr("Forma de pagamento"));
    pbNumberPayment->setToolTip(tr("Escolha a forma de pagamento"));
    pbNumberPayment->setObjectName("pbNumberPayment");
    pbNumberPayment->setStyleSheet(this->styleSheet());
    pbNumberPayment->setFont(this->font());
    pbNumberPayment->setCursor(Qt::PointingHandCursor);

    QString styleText = "color: rgba(240,240,240,255);";

    lSubtotalOrdered->setText(tr("SubTotal R$: 0"));
    lSubtotalOrdered->setFont(f);
    lSubtotalOrdered->setStyleSheet(styleText);

    DSBShipping->setPrefix(tr("Frete R$ "));
    DSBShipping->setMaximum(999999999.000000);
    DSBShipping->setObjectName("DSBDeliveryRate");
    DSBShipping->setFont(this->font());

    lTotalOrdered->setText(tr("Total R$: 0"));
    lTotalOrdered->setFont(f);
    lTotalOrdered->setStyleSheet(styleText);

    isPaid->setText(tr("Este pedido já foi pago"));
    isPaid->setStyleSheet(styleText);
    isPaid->setFont(f);

    DSBPaid->setPrefix(tr("Pago R$ "));
    DSBPaid->setMaximum(999999999.000000);
    DSBPaid->setFont(this->font());

    lTransshipment->setText(tr("Troco R$: 0"));
    lTransshipment->setFont(f);
    lTransshipment->setStyleSheet(styleText);

    cbPaymentForm->addItems(order->paymentTypes());
    cbPaymentForm->setToolTip(tr("Forma de pagamento"));
    cbPaymentForm->setObjectName("cbPaymentForm");
    cbPaymentForm->setStyleSheet(this->styleSheet());
    cbPaymentForm->setFont(this->font());

    lPaymentFormTotal->setText(tr("Total restante R$: "));
    lPaymentFormTotal->setFont(f);
    lPaymentFormTotal->setStyleSheet(styleText);

    DSBValuePaymentForm->setPrefix(tr("Valor R$: "));
    DSBValuePaymentForm->setFont(this->font());
    DSBValuePaymentForm->setMaximum(999999999.000000);

    pbAddPaymentForm->setText(tr("Adicionar a lista"));
    pbAddPaymentForm->setFont(this->font());

    pbRemovePaymentForm->setText(tr("Remover da lista"));
    pbRemovePaymentForm->setFont(this->font());

    swValuesFields->addWidget(wValuesFields);
    swValuesFields->addWidget(wPaymentForm);

    wPizzaMixed->setFont(this->font());
    wPizzaMixed->setStyleSheet(this->styleSheet());

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

    DSBShipping->setNextComponent(isPaid);

    cbSize->addItem("Tamanhos");

    // setando os nextComponents
    ePhone->setNextComponent(eCep);
    eCep->setNextComponent(eName);
    eName->setNextComponent(eStreet);
    eStreet->setNextComponent(eNumber);
    eNumber->setNextComponent(eComplement);
    eComplement->setNextComponent(eDistrict);
    eDistrict->setNextComponent(searchProduct->eSearch);

    ePhoneBusiness->setNextComponent(eCepBusiness);
    eCepBusiness->setNextComponent(eNameContactBusiness);
    eNameContactBusiness->setNextComponent(eNameBusiness);
    eNameBusiness->setNextComponent(eFantasyNameBusiness);
    eFantasyNameBusiness->setNextComponent(eCNPJBusiness);
    eCNPJBusiness->setNextComponent(eStreetBusiness);
    eStreetBusiness->setNextComponent(eNumberBusiness);
    eNumberBusiness->setNextComponent(eComplementBusiness);
    eComplementBusiness->setNextComponent(eDistrictBusiness);
    eDistrictBusiness->setNextComponent(searchProduct->eSearch);

    searchProduct->eSearch->setNextComponent(cbSize);
    cbSize->setNextComponent(DSBValueProduct);
    DSBValueProduct->setNextComponent(sbAmount);

    searchProduct->setCompleter(order->bd.getCompleter("product"));

    ePhone->setCompleter(order->bd.getCompleter("phone"));

    ePhoneBusiness->setCompleter(order->bd.getCompleter("phoneBusiness"));

    eDateDelivery->setNextComponent(eHourDelivery);

    typeClient = NORMAL;

    tableItem->setModel(dataModelItems);

    tablePaymentForm->setModel(dataModelPayment);

    totalValueOrder = 0;
    totalValueOrderProducts = 0;

    actionName->setCheckable(true);
    actionName->setChecked(true);
    actionName->setEnabled(false);

    actionNickName->setCheckable(true);
    actionNickName->setChecked(false);
    actionNickName->setEnabled(true);

    searchProduct->setAction(actionName);
    searchProduct->setAction(actionNickName);

    eDateDelivery->setInputMask("  /  /    ");
    eHourDelivery->setInputMask("  :  ");

    DSBValueProduct->setMaximum(99999999);

    emit setInput();
}

void DeliveryInterface::setPreview()
{
    QString html;

    html = "<html>"
            "<p align=\"center\"> Nome do produto</p>"
            "<p align=\"center\"> Valor em reais</p>"
            "<p align=\"center\"> Unidade(s)</p>"
            "<p align=\"center\"> Valor total </p>"
            "</html>";

    productPreview->setText(html);
}

void DeliveryInterface::setPreview(QString nome, float value, int quant)
{
    QString html;
    QString quantText, valueText, totalText;

    valueText.setNum(value);
    quantText.setNum(quant);
    totalText.setNum(value * quant);

    html = "<html>"
            "<p align=\"center\">" + nome + "</p>"
            "<p align=\"center\">" + valueText + tr(" Reais") + "</p>"
            "<p align=\"center\">" + quantText + tr(" Unidade(s)") + "</p>"
            "<p align=\"center\">" + totalText +  tr(" Reais") + "</p>"
            "</html>";

    productPreview->setText(html);
}

void DeliveryInterface::onClick_pbSelectPeople()
{
    ePhone->clear();

    QString black = "border-radius: 2px;"
            "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            "stop: 0 rgb(80,80,80), stop: 1 rgb(60,60,60));"
            "color: rgb(220,220,220);"
            "border: 1px solid rgb(220,220,220,50);";

    QString white = "border-radius: 2px;"
            "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            "stop: 0 rgb(240,240,240), stop: 1 rgb(220,220,220));"
            "color: rgb(60,60,60);"
            "border: 1px solid rgb(60,60,60);";

    pbSelectPeople->setStyleSheet(white);

    pbSelectBusiness->setStyleSheet(black);

    swSelectClient->setCurrentIndex(0);

    typeClient = NORMAL;

    order->setTypeClient(NORMAL);

    ePhone->setFocus();
}

void DeliveryInterface::onClick_pbSelectBusiness()
{
    ePhoneBusiness->Clear();

    QString black = "border-radius: 2px;"
            "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            "stop: 0 rgb(80,80,80), stop: 1 rgb(60,60,60));"
            "color: rgb(220,220,220);"
            "border: 1px solid rgb(220,220,220,50);";

    QString white = "border-radius: 2px;"
            "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            "stop: 0 rgb(240,240,240), stop: 1 rgb(220,220,220));"
            "color: rgb(60,60,60);"
            "border: 1px solid rgb(60,60,60);";

    pbSelectPeople->setStyleSheet(black);

    pbSelectBusiness->setStyleSheet(white);

    swSelectClient->setCurrentIndex(1);

    typeClient = BUSINESS;

    order->setTypeClient(BUSINESS);

    ePhoneBusiness->setFocus();
}

void DeliveryInterface:: onClick_pbLeaveClient()
{
    ePhone->Clear();
    eCep->Clear();
    eName->Clear();
    eStreet->Clear();
    eNumber->Clear();
    eComplement->Clear();
    eDistrict->Clear();

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

    if (typeClient == NORMAL)
        ePhone->setFocus();
    else
        ePhoneBusiness->setFocus();
}

void DeliveryInterface::onClick_lPizza()
{
    wPizzaMixed->show();
}

void DeliveryInterface::onClick_pbLeaveProduct()
{
    setPreview();
    listSizes.clear();
    listValues.clear();
    sbAmount->setValue(1);
    cbSize->clear();
    cbSize->addItem("Tamanho");
    teProductNotes->setText("");
    searchProduct->eSearch->Clear();
    DSBValueProduct->setValue(0);

    productName.clear();

    dataOrderIdProduct.clear();

    pbLeaveProduct->setText("Limpar");
    pbConfirmProduct->setText("Confirmar");
}



bool DeliveryInterface::isAvailable()
{
    if (ePhone->isEmpty()){
        QMessageBox::warning(this, tr("Telefone inválido"), tr("Por favor, digite um número de telefone."), tr("Ok"));

        ePhone->setFocus();

        return false;
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

bool DeliveryInterface::isAvailableBusiness(){
    if (ePhoneBusiness->isEmpty()){
        QMessageBox::warning(this, tr("Telefone inválido"), tr("Por favor, digite um número de telefone."), tr("Ok"));

        ePhoneBusiness->setFocus();

        return false;
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

void DeliveryInterface::onClick_pbSaveOrdered()
{    
    if (! eDateDelivery->isEmpty())
        if (! tools.isValidDate(eDateDelivery->text())){
            QMessageBox::warning(this, "Data inválida", "Data para agendamento inválida.", QMessageBox::Ok);

            return;
        }

    if (! eHourDelivery->isEmpty())
        if (! tools.isValidHour(eHourDelivery->text())){
            QMessageBox::warning(this, "Hora inválida", "Hora para agendamento inválida.", QMessageBox::Ok);

            return;
        }

    if (orderIdProduct.length() == 0){

        QMessageBox::warning(this, tr("Selecione um produto"), tr("O pedido deve ter pelo menos um produto"), tr("Voltar"));

        searchProduct->eSearch->Clear();

        searchProduct->eSearch->setFocus();

        return;
    }

    if (totalValueOrder > DSBPaid->value()){

        int op = QMessageBox::question(this, tr("Preço pago insuficiente"), tr("O valor pago é insuficiente para pagar o pedido.\nDeseja salvar o pedido dessa forma?"), tr("Sim"), tr("Não"));

        if (op == 1)
            return;

        DSBPaid->setFocus();
    }

    bool client = true;

    if (idClient == 0 && ePhone->isEmpty() && ePhoneBusiness->isEmpty()){
        int op = QMessageBox::question(this, tr("Selecionar um cliente"), tr("Você não selecionou um cliente.\nDeseja salvar o pedido sem cliente? Ele será salvo como pedido no balcão."), "Salvar", "Cancelar");

        if (op == 1){

            if (typeClient == NORMAL)
                ePhone->setFocus();
            else
                ePhoneBusiness->setFocus();

            return;
        }else{
            order->typeOrder = Order::COUNTER;
            client = false;
        }

    }else{
        order->typeOrder = Order::DELYVERY;
    }

    // se o cliente tiver sido selecionado, ele vai verificar se o cliente se foi alterado
    if (! idClient == 0){
        if (typeClient == NORMAL){

            dataClientF << QString::number(idClient) << eName->text() << ePhone->text() << eStreet->text() << eNumber->text() << eComplement->text() << eDistrict->text() << eCep->text() << QString::number(DSBShipping->value()) << teClientNotes->toPlainText();

        }else{

            dataClientF << QString::number(idClient) << eNameBusiness->text() << eNameBusiness->text() << eFantasyNameBusiness->text() << eCNPJBusiness->text() << ePhoneBusiness->text() << eStreetBusiness->text() << eNumberBusiness->text() << eComplementBusiness->text() << eDistrictBusiness->text() << eCepBusiness->text() << QString::number(DSBShipping->value()) << teClientNotes->toPlainText();

        }

        bool equal = true;

        for (int i = 0; i < dataClientI.length(); i++){
            if (dataClientI[i] != dataClientF[i]){
                equal = false;
                break;
            }
        }

        if (! equal){
            int op = QMessageBox::question(this, tr("Alteração do cliente."), tr("Tiveram alterações no cliente.\nDeseja salvá-las?"), "Sim", "Não");

            if (op == 0)
                if (! order->updateClient(dataClientF))
                    qDebug() << "[DeliveryInterface] [Salvar Pedido]" << "cliente não alterado";
        }
    }

    if (client){
        if (idClient == 0){
            if (typeClient == NORMAL){
                if (isAvailable()){
                    //seta os valores

                    idClient = order->nextIdClient();

                    order->phone = ePhone->text();
                    order->name = eName->text();
                    order->cep = eCep->text();
                    order->street = eStreet->text();
                    order->number = eNumber->text();
                    order->complement = eComplement->text();
                    order->district = eDistrict->text();
                    order->shipping = DSBShipping->value();
                    order->note = teClientNotes->toPlainText();

                    order->insertClient();
                }
            }else{
                if (isAvailableBusiness()){
                    //seta os valores

                    idClient = order->nextIdClient();

                    order->phone = ePhoneBusiness->text();
                    order->nameRespondible = eNameContactBusiness->text();
                    order->cep = eCepBusiness->text();
                    order->street = eStreetBusiness->text();
                    order->number = eNumberBusiness->text();
                    order->complement = eComplementBusiness->text();
                    order->district = eDistrictBusiness->text();
                    order->shipping = DSBShipping->value();
                    order->note = teClientNotes->toPlainText();
                    order->corporateName = eNameBusiness->text();
                    order->CNPJ = eCNPJBusiness->text();
                    order->fantasyName = eFantasyNameBusiness->text();

                    order->insertClient();
                }
            }
        }
    }

    order->typeClient = typeClient;
    order->idClient = idClient;
    order->totalValueOrderProducts = totalValueOrderProducts;
    order->valuePaid = DSBPaid->value();
    order->valueShipping = DSBShipping->value();
    order->orderQuantity = orderQuantity;
    order->orderValues = orderValues;
    order->orderSizes = orderSizes;
    order->orderPartProduct = orderPartProduct;
    order->orderIdProduct = orderIdProduct;
    order->orderNameProduct = orderNameProduct;
    order->orderNoteProduct = orderNoteProduct;
    order->isPaid = isPaid->isChecked();

    QList<QString> orderPaymentForm;
    QList<QString> orderPaymentFormValue;

    if (tablePaymentForm->model()->rowCount() > 0){
        for (int i = 0; i < tablePaymentForm->model()->rowCount(); i++){
            orderPaymentForm << tablePaymentForm->model()->index(i, 0).data().toString();
            orderPaymentFormValue << tablePaymentForm->model()->index(i, 1).data().toString();
        }
    }

    order->orderPaymentForm = orderPaymentForm;
    order->orderPaymentFormValue = orderPaymentFormValue;

    order->isAdd = isAdd;

    if (order->insertOrder()){

        int op = QMessageBox::question(this, tr("Salvo com sucesso"), tr("Pedido salvo com sucesso.\nDeseja imprimir o pedido agora?"), tr("Sim"), tr("Não"));

        if (op == 0)
            html.print(html.createHTMLOrder(order->idOrder.toInt()));
    }

    if ((! eDateDelivery->isEmpty()) && (! eHourDelivery->isEmpty())){
        int op = QMessageBox::question(this, tr("Agendamento"), tr("O pedido foi agendado para ") + eDateDelivery->text() + " " + eHourDelivery->text() + tr(".\nConfirmar agendamento?"), tr("Sim"), tr("Não"));

        if (op == 0){
            order->dateSchedule = eDateDelivery->text();
            order->hourSchedule = eHourDelivery->text();

            order->insertShedule();
        }

    }

    emit newOrder();

    clearOrder();
}


void DeliveryInterface::onClick_pbRemoveItem()
{
    int row = tableItem->currentIndex().row();

    if (row >= 0){

        int op = QMessageBox::question(this, tr("Remover item"),tr("Deseja remover o produto do pedido?"), tr("Sim"), tr("Não"));

        if (op == 0){
            dataModelItems->removeRow(row);

            totalValueOrderProducts -= orderValues[row] * orderQuantity[row];

            orderQuantity.removeAt(row);
            orderValues.removeAt(row);
            orderSizes.removeAt(row);
            orderPartProduct.removeAt(row);
            orderIdProduct.removeAt(row);
            orderNameProduct.removeAt(row);
            orderNoteProduct.removeAt(row);

            updateValues();
        }
    }
}

void DeliveryInterface::onClick_pbClearOrdered()
{
    if (tableItem->model()->rowCount() == 0)
        return;

    int op = QMessageBox::question(this, tr("Deleter todos os itens."),tr("Tem certeza que deseja apagar todos os itens deste pedido?"), tr("Sim"), tr("Não"));

    if (op == 0){
        dataModelItems->clear();

        orderQuantity.clear();
        orderValues.clear();
        orderSizes.clear();
        orderPartProduct.clear();
        orderIdProduct.clear();
        orderNameProduct.clear();
        orderNoteProduct.clear();

        totalValueOrderProducts = 0;

        eDateDelivery->Clear();

        eHourDelivery->Clear();

        updateValues();
    }
}

void DeliveryInterface::clearOrder(){
    dataModelItems->clear();

    orderQuantity.clear();
    orderValues.clear();
    orderSizes.clear();
    orderPartProduct.clear();
    orderIdProduct.clear();
    orderNameProduct.clear();
    orderNoteProduct.clear();

    totalValueOrderProducts = 0;

    ePhone->Clear();

    ePhoneBusiness->Clear();

    searchProduct->eSearch->Clear();

    DSBShipping->setValue(0);

    DSBPaid->setValue(0);

    dataModelPayment->clear();

    DSBValuePaymentForm->setValue(0);

    cbPaymentForm->setCurrentIndex(0);

    eDateDelivery->Clear();

    eHourDelivery->Clear();

    onClick_pbLeaveProduct();
    onClick_pbLeaveClient();

    if (typeClient == NORMAL)
        ePhone->setFocus();
    else
        ePhoneBusiness->setFocus();

    updateValues();

    isAdd = true;

    DSBPaid->setEnabled(true);

    pbSaveOrdered->setText(tr("Salvar"));
}

void DeliveryInterface::onClick_pbCancelOrdered()
{
    int op;

    if (isAdd)
        op = QMessageBox::question(this, tr("Deleter todos os itens."),tr("Tem certeza que deseja cancelar este pedido?"), tr("Sim"), tr("Não"));
    else
        op = QMessageBox::question(this, tr("Deleter todos os itens."),tr("Tem certeza que deseja cancelar a edição deste pedido?"), tr("Sim"), tr("Não"));

    if (op == 0){
        clearOrder();
    }
}

void DeliveryInterface::onClick_pbNumberPayment()
{
    if(swValuesFields->currentIndex() == 0)
    {
        swValuesFields->setCurrentIndex(1);
        pbNumberPayment->setText(tr("Voltar"));

        double totalPaid = 0;

        for(int i = 0; i < tablePaymentForm->model()->rowCount(); i++){
            totalPaid += tablePaymentForm->model()->index(i, 1).data().toDouble();
        }

        if (totalValueOrder - totalPaid >= 0)
            lPaymentFormTotal->setText("Total restante R$: " + QString::number(totalValueOrder - totalPaid));
        else
            lPaymentFormTotal->setText("Total restante R$: 0");
    }
    else
    {
        swValuesFields->setCurrentIndex(0);
        pbNumberPayment->setText(tr("Forma de pagamento"));
    }
}

void DeliveryInterface::hideFinishedClient()
{
    disconnect(hide,SIGNAL(finished()),this,SLOT(hideFinishedClient()));

    wProduct->setVisible(isNoteClient);

    show->start();
}

void DeliveryInterface::hideFinishedProduct()
{
    disconnect(hide,SIGNAL(finished()),this,SLOT(hideFinishedProduct()));

    wClient->setVisible(isNoteProduct);

    show->start();
}

void DeliveryInterface::onClick_lSchedule()
{
    if(isSchedule)
        wScheduleX = this->width() - 330;
    else
        wScheduleX = this->width() - 50;

    isSchedule = !isSchedule;

    wSchedule->setGeometry(wScheduleX,130,330,50);

    eDateDelivery->setFocus();
}

void DeliveryInterface::showNotesClient(bool b)
{
    if(b)
    {
        hide = transition->createAnimation(wProduct,wClient->geometry(),300,QEasingCurve::Linear);

        show = transition->createAnimation(wClientNotes,QRect(wClient->x() + wClient->width() + 10,wClient->y(),
                                                              wClient->width(),wClient->height()),300,QEasingCurve::Linear);
        wClientNotes->show();

        wProductNotes->hide();

        hide->start();
    }
    else
    {
        hide = transition->createAnimation(wClientNotes,wClient->geometry(),300,QEasingCurve::Linear);

        show = transition->createAnimation(wProduct,QRect(((this->width() - 700) / 3) * 2 + wClient->width(),
                                                          15,350,310),300,QEasingCurve::Linear);
        hide->start();

    }

    connect(hide,SIGNAL(finished()),this,SLOT(hideFinishedClient()));
}

void DeliveryInterface::showNotesProduct(bool b)
{
    if(b)
    {
        hide = transition->createAnimation(wClient,wProduct->geometry(),300,QEasingCurve::Linear);

        show = transition->createAnimation(wProductNotes,QRect(wProduct->x() - 360,wProduct->y(),
                                                               wProduct->width(),wProduct->height()),300,QEasingCurve::Linear);
        wClientNotes->hide();

        wProductNotes->show();

        hide->start();
    }
    else
    {
        hide = transition->createAnimation(wProductNotes,wProduct->geometry(),300,QEasingCurve::Linear);

        show = transition->createAnimation(wClient,QRect((this->width() - 700) / 3,15,350,310),300,QEasingCurve::Linear);

        hide->start();
    }

    connect(hide,SIGNAL(finished()),this,SLOT(hideFinishedProduct()));
}

void DeliveryInterface::onClick_lNoteProduct()
{
    lNoteProduct->setAlternate(isNoteProduct);

    if(isNoteProduct)
    {
        showNotesProduct(true);
        isNoteProduct = false;
    }
    else
    {
        showNotesProduct(false);
        isNoteProduct = true;
    }
}


void DeliveryInterface::onClick_lNoteClient()
{
    lNoteClient->setAlternate(isNoteClient);

    if(isNoteClient)
    {
        showNotesClient(true);
        isNoteClient = false;
    }
    else
    {
        showNotesClient(false);
        isNoteClient = true;
    }
}

void DeliveryInterface::onTextChanged_ePhone(QString text){
    if (text.length() == ePhone->maxLength())
        eCep->setFocus();

    if (text.length() >= 9){


        QList<QString> data;

        data = order->getDataClient(text);

        if (data.length() > 0){
            dataClientI = data;

            idClient = data[0].toInt();
            eName->setText(data[1]);
            eCep->setText(data[7]);
            eStreet->setText(data[3]);
            eNumber->setText(data[4]);
            eComplement->setText(data[5]);
            eDistrict->setText(data[6]);
            teClientNotes->setText(data[9]);

            DSBShipping->setValue(data[8].toDouble());

            searchProduct->eSearch->Clear();

            searchProduct->eSearch->setFocus();
        }

        return;
    }

    idClient = 0;

    eCep->setText(eCep->valueStandart);
    eName->setText(eName->valueStandart);
    eStreet->setText(eStreet->valueStandart);
    eNumber->setText(eNumber->valueStandart);
    eComplement->setText(eComplement->valueStandart);
    eDistrict->setText(eDistrict->valueStandart);
    teClientNotes->setText("");
}

void DeliveryInterface::onTextChanged_ePhoneBusiness(QString text){
    if (text.length() == ePhoneBusiness->maxLength())
        eCepBusiness->setFocus();

    if (text.length() >= 9){
        QList<QString> data;

        data = order->getDataClient(text);

        if (data.length() > 0){

            dataClientF = data;

            idClient = data[0].toInt();
            eNameContactBusiness->setText(data[1]);
            eNameBusiness->setText(data[2]);
            eFantasyNameBusiness->setText(data[3]);
            eCNPJBusiness->setText(data[4]);
            eStreetBusiness->setText(data[6]);
            eNumberBusiness->setText(data[7]);
            eComplementBusiness->setText(data[8]);
            eDistrictBusiness->setText(data[9]);
            eCepBusiness->setText(data[10]);
            teClientNotes->setText(data[12]);

            DSBShipping->setValue(data[11].toDouble());

            searchProduct->eSearch->Clear();

            searchProduct->eSearch->setFocus();
        }

        return;
    }

    idClient = 0;

    eCepBusiness->setText(eCepBusiness->valueStandart);
    eNameContactBusiness->setText(eNameContactBusiness->valueStandart);
    eNameBusiness->setText(eNameBusiness->valueStandart);
    eFantasyNameBusiness->setText(eFantasyNameBusiness->valueStandart);
    eCNPJBusiness->setText(eCNPJBusiness->valueStandart);
    eStreetBusiness->setText(eStreetBusiness->valueStandart);
    eNumberBusiness->setText(eNumberBusiness->valueStandart);
    eComplementBusiness->setText(eComplementBusiness->valueStandart);
    eDistrictBusiness->setText(eDistrictBusiness->valueStandart);
    teClientNotes->setText("");
}

void DeliveryInterface::onClick_pbConfirmProduct()
{
    if (pbConfirmProduct->text() == "Editar item"){
        dataModelItems->removeRow(indexSelectedItem);

        totalValueOrderProducts -= orderValues[indexSelectedItem] * orderQuantity[indexSelectedItem];

        orderQuantity.removeAt(indexSelectedItem);
        orderValues.removeAt(indexSelectedItem);
        orderSizes.removeAt(indexSelectedItem);
        orderPartProduct.removeAt(indexSelectedItem);
        orderIdProduct.removeAt(indexSelectedItem);
        orderNameProduct.removeAt(indexSelectedItem);
        orderNoteProduct.removeAt(indexSelectedItem);

        updateValues();
    }

    if (dataOrderIdProduct.isEmpty()){
        QMessageBox::warning(this, tr("Selecione um sabor"), tr("Por favor, selecione um produto válido."), QMessageBox::Ok);

        searchProduct->eSearch->Clear();

        searchProduct->eSearch->setFocus();

        return;
    }

    dataOrderQntProduct = sbAmount->value();
    dataOrderSizeProduct = cbSize->currentText();

    orderQuantity << dataOrderQntProduct;
    //    orderValues << dataOrderValueProduct;

    orderValues << DSBValueProduct->value();
    orderSizes << dataOrderSizeProduct;
    orderNoteProduct << teProductNotes->toPlainText();

    QList<QString> id, name;

    id << dataOrderIdProduct;
    name << dataOrderNameProduct;

    orderIdProduct << id;
    orderNameProduct << name;

    QList<int> part;

    part << 1;

    orderPartProduct << part;

    //    totalValueOrderProducts += dataOrderValueProduct * dataOrderQntProduct;
    totalValueOrderProducts += DSBValueProduct->value() * dataOrderQntProduct;

    QList<QString> dataForModel;

    //    dataForModel << dataOrderNameProduct << cbSize->currentText() << QString::number(dataOrderQntProduct) << QString::number(dataOrderValueProduct) << teProductNotes->toPlainText();
    dataForModel << dataOrderNameProduct << cbSize->currentText() << QString::number(dataOrderQntProduct) << QString::number(DSBValueProduct->value()) << teProductNotes->toPlainText();

    dataModelItems->add(dataForModel);

    updateValues();

    double totalPaid = 0;

    for(int i = 0; i < tablePaymentForm->model()->rowCount(); i++){
        totalPaid += tablePaymentForm->model()->index(i, 1).data().toDouble();
    }

    if (totalValueOrder - totalPaid >= 0)
        lPaymentFormTotal->setText("Total restante R$: " + QString::number(totalValueOrder - totalPaid));
    else
        lPaymentFormTotal->setText("Total restante R$: 0");

    onClick_pbLeaveProduct();
}

void DeliveryInterface::onTextChanged_eCep(QString text){
    if (text.length() == eCep->maxLength())
        eName->setFocus();
}

void DeliveryInterface::onTextChanged_eCepBusiness(QString text){
    if (text.length() == eCepBusiness->maxLength())
        eNameContactBusiness->setFocus();
}

void DeliveryInterface::getSearchProduct(QString text){
    if (text.length() > 0){
        QString sql = "select * from product where lower(name) like lower('" + text + "%') order by idproduct";

        order->bd.updateModelQuery("product", sql);

        sql = "select idproduct, note from product where lower(name) = lower('" + text + "')";

        if (order->bd.search(sql)){

            dataOrderIdProduct = order->bd.loadSimple(0);
            dataOrderNoteProduct = order->bd.loadSimple(1);
            dataOrderNameProduct = text;

            teProductNotes->setText(order->bd.loadSimple(1));

            sql = "select s.name, pv.valueproduct from size s, product p, productvalue pv where p.idproduct = pv.idproduct and pv.idsize = s.idsize and p.idproduct = " + order->bd.loadSimple(0);

            if (order->bd.search(sql, 1)){
                cbSize->clear();
                listSizes = order->bd.loadColumn(0, 1);
                listValues = order->bd.loadColumn(1, 1);
                cbSize->addItems(listSizes);

                float value = listValues[0].toDouble();

                dataOrderValueProduct = listValues[0].toDouble();

                productName = text;

                DSBValueProduct->setValue(value);

                setPreview(productName, value, sbAmount->value());
            }

        }else{
            listSizes.clear();
            listValues.clear();
            cbSize->clear();
            cbSize->addItem("Tamanho");
            DSBValueProduct->setValue(0.00);
            sbAmount->setValue(1);
            productName.clear();
            setPreview();
        }

    }else{
        QString sql = "select * from product order by idproduct";

        order->bd.updateModelQuery("product", sql);

        setPreview();
    }
}

void DeliveryInterface::valueChanged_sbAmount(int value){
    Q_UNUSED(value);

    if (listSizes.length() > 0){
        //        float value = listValues[cbSize->currentIndex()].toDouble();
        float value = DSBValueProduct->value();

        dataOrderValueProduct = listValues[cbSize->currentIndex()].toDouble();

        dataOrderQntProduct = sbAmount->value();

        setPreview(productName, value, sbAmount->value());
    }
}

void DeliveryInterface::currentIndexChanged_cbSize(int index){
    Q_UNUSED(index);

    if (listSizes.length() > 0 && cbSize->count() > 0){
        float value = listValues[cbSize->currentIndex()].toDouble();

        DSBValueProduct->setValue(value);

        dataOrderValueProduct = value;

        setPreview(productName, value, sbAmount->value());
    }
}

void DeliveryInterface::doubleClicked_tableItem(QModelIndex index){
    int row = index.row();
    if (orderIdProduct[row].length() == 1){
        // produto normal
        searchProduct->eSearch->setText(orderNameProduct[row][0]);

        for (int j = 0; j < cbSize->count(); j++){
            cbSize->setCurrentIndex(j);
            if (orderSizes[row] == cbSize->currentText())
                break;
        }

        sbAmount->setValue(orderQuantity[row]);

        indexSelectedItem = row;

        teProductNotes->setText(orderNoteProduct[row]);

        pbConfirmProduct->setText("Editar item");
        pbLeaveProduct->setText("Cancelar");
    }else{
        emit editPizzaMixed(orderPartProduct[row], orderIdProduct[row], orderNameProduct[row], orderNoteProduct[row], orderQuantity[row], orderValues[row], orderSizes[row], row);

        wPizzaMixed->show();
    }
}

void DeliveryInterface::valueChanged_dsbDeliveryRate(double value){
    totalValueOrder = totalValueOrderProducts + value;

    updateValues();
}

void DeliveryInterface::valueChanged_dsbPaid(double value){
    if (value > totalValueOrder)
        lTransshipment->setText(tr("Troco R$: ") + QString::number(value - totalValueOrder));
    else
        lTransshipment->setText(tr("Troco R$: 0"));

    if (DSBPaid->hasFocus()){
        dataModelPayment->clear();

        QList<QString> values;

        values << "Dinheiro" << QString::number(DSBPaid->value());

        dataModelPayment->add(values);
    }
}

void DeliveryInterface::updateValues()
{
    lSubtotalOrdered->setText(tr("SubTotal R$: ") + QString::number(totalValueOrderProducts));

    totalValueOrder = totalValueOrderProducts + DSBShipping->value();

    lTotalOrdered->setText(tr("Total R$: ") + QString::number(totalValueOrder));

    if (DSBPaid->value() > totalValueOrder)
        lTransshipment->setText(tr("Troco R$: ") + QString::number(DSBPaid->value() - totalValueOrder));
    else
        lTransshipment->setText(tr("Troco R$: 0"));
}

void DeliveryInterface::slotPizzaMista(QList<int> listOrderPartProduct, QList<QString> listOrderIdProduct, QList<QString> listOrderNameProduct, QString note, int quantity, double value, QString size, bool insert, int row){

    if (! insert){

        dataModelItems->removeRow(row);

        totalValueOrderProducts -= orderValues[row] * orderQuantity[row];

        orderQuantity.removeAt(row);
        orderValues.removeAt(row);
        orderSizes.removeAt(row);
        orderPartProduct.removeAt(row);
        orderIdProduct.removeAt(row);
        orderNameProduct.removeAt(row);
        orderNoteProduct.removeAt(row);

        updateValues();
    }

    orderQuantity << quantity;
    orderValues << value;
    orderPartProduct << listOrderPartProduct;
    orderIdProduct << listOrderIdProduct;
    orderNameProduct << listOrderNameProduct;
    orderSizes << size;

    QString noteN = "Sabores: ";

    int totalParts = 0;

    foreach(int part, listOrderPartProduct)
        totalParts += part;

    for (int i = 0; i < listOrderNameProduct.length(); i++){
        noteN += QString::number(listOrderPartProduct[i]) + "/" + QString::number(totalParts) + " de " + listOrderNameProduct[i];
        if (i == listOrderNameProduct.length() - 2)
            noteN += " e ";
        else
            if (i < listOrderNameProduct.length() - 1)
                noteN += ", ";
    }

    if (! note.isEmpty())
        noteN += "\n" + note;

    orderNoteProduct << noteN;

    QList<QString> dataForModel;

    dataForModel << "Pizza mista" << size << QString::number(quantity) << QString::number(value) << noteN;

    dataModelItems->add(dataForModel);

    totalValueOrderProducts += value * quantity;

    updateValues();
}

void DeliveryInterface::onClick_pbAddPaymentForm(){
    if (DSBValuePaymentForm->value() == 0){
        QMessageBox::warning(this, "Selecione um valor", "Por favor, selecione um valor", QMessageBox::Ok);

        DSBValuePaymentForm->setFocus();

        return;
    }

    QList<QString> dataForModel;

    dataForModel << cbPaymentForm->currentText() << QString::number(DSBValuePaymentForm->value());

    dataModelPayment->add(dataForModel);

    if (tablePaymentForm->model()->rowCount() > 0)
        DSBPaid->setEnabled(false);
    else{
        DSBPaid->setEnabled(true);

        DSBPaid->setValue(0);
    }

    double totalPaid = 0;


    for(int i = 0; i < tablePaymentForm->model()->rowCount(); i++){
        totalPaid += tablePaymentForm->model()->index(i, 1).data().toDouble();
    }

    if (totalValueOrder - totalPaid >= 0)
        lPaymentFormTotal->setText("Total restante R$: " + QString::number(totalValueOrder - totalPaid));
    else
        lPaymentFormTotal->setText("Total restante R$: 0");

    DSBValuePaymentForm->setValue(0);

    cbPaymentForm->setFocus();

    DSBPaid->setValue(totalPaid);

    updateValues();
}

void DeliveryInterface::onClick_pbRemovePaimentForm(){
    if (dataModelPayment->rowCount(QModelIndex()) == 0)
        return;

    if (!tablePaymentForm->currentIndex().isValid())
        return;

    int row = tablePaymentForm->currentIndex().row();

    if(dataModelPayment->removeRow(row))
        return;

    if (tablePaymentForm->model()->rowCount() > 0)
        DSBPaid->setEnabled(false);
    else{
        DSBPaid->setEnabled(true);

        DSBPaid->setValue(0);
    }

    double totalPaid = 0;

    for(int i = 0; i < tablePaymentForm->model()->rowCount(); i++){
        totalPaid += tablePaymentForm->model()->index(i, 1).data().toDouble();
    }

    if (totalValueOrder - totalPaid >= 0)
        lPaymentFormTotal->setText("Total restante R$: " + QString::number(totalValueOrder - totalPaid));
    else
        lPaymentFormTotal->setText("Total restante R$: 0");

    DSBValuePaymentForm->setValue(0);

    cbPaymentForm->setFocus();

    if (totalPaid >= totalValueOrder){
        QMessageBox::information(this, "Valor total atingido", "O valor total da compra já foi atingido", QMessageBox::Ok);

        DSBPaid->setValue(totalPaid);

        onClick_pbNumberPayment();

        return;
    }
}

void DeliveryInterface::onTriggered_actionName()
{
    actionName->setChecked(true);
    actionName->setEnabled(false);

    actionNickName->setChecked(false);
    actionNickName->setEnabled(true);
}

void DeliveryInterface::onTriggered_actionNickName()
{
    actionName->setChecked(false);
    actionName->setEnabled(true);

    actionNickName->setChecked(true);
    actionNickName->setEnabled(false);
}

void DeliveryInterface::editOrder(int idOrder)
{
    order->loadDataOrder(idOrder);
}

void DeliveryInterface::loadDataOrder()
{
    qDebug() << Q_FUNC_INFO << "loadDataOrder";

    pbSaveOrdered->setText(tr("Editar")); // muda o texto

    isAdd = false;

    if (order->typeClient == 1){
        typeClient = NORMAL;

        onClick_pbSelectPeople();

        ePhone->setText(order->phoneClient);
    }else if (typeClient == 2){

        typeClient = BUSINESS;

        onClick_pbSelectBusiness();

        ePhoneBusiness->setText(order->phoneClient);
    }

    idClient = order->idClient;

    totalValueOrderProducts = order->totalValueOrderProducts;

    DSBPaid->setValue(order->valuePaid);

    DSBShipping->setValue(order->valueShipping);

    isPaid->setChecked(order->isPaid);

    orderQuantity = order->orderQuantity;
    orderValues = order->orderValues;
    orderSizes = order->orderSizes;
    orderPartProduct = order->orderPartProduct;
    orderIdProduct = order->orderIdProduct;
    orderNameProduct = order->orderNameProduct;
    orderNoteProduct = order->orderNoteProduct;

    if (! order->dateSchedule.isEmpty()){
        eDateDelivery->setText(order->dateSchedule);
        eHourDelivery->setText(order->hourSchedule);
    }

    dataModelPayment->clear();

    dataModelItems->clear();

    QList<QString> dataForModel;
    for (int i = 0; i < order->orderPaymentForm.length(); i++){

        dataForModel.clear();

        dataForModel << order->orderPaymentForm[i] << order->orderPaymentFormValue[i];

        dataModelPayment->add(dataForModel);
    }


    for (int i = 0; i < order->orderIdProduct.length(); i++){
        dataForModel.clear();

        if (orderIdProduct[i].length() == 1)
        {

            dataForModel << order->orderNameProduct[i] << order->orderSizes[i] << QString::number(order->orderQuantity[i]) << QString::number(order->orderValues[i]) << order->orderNoteProduct[i];

        }else{

            dataForModel << "Pizza mista" << order->orderSizes[i] << QString::number(order->orderQuantity[i]) << QString::number(order->orderValues[i]) << order->orderNoteProduct[i];

        }

        dataModelItems->add(dataForModel);
    }

    // da update nos valores

    updateValues();
}

void DeliveryInterface::valueChanged_DSBValueProduct(double value)
{
    if (! productName.isEmpty())
        setPreview(productName, value, sbAmount->value());
    else
        DSBValueProduct->setValue(0);
}
