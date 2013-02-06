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


 #include "Interface/paymentview.h"
#include <QDebug>
#include <QMessageBox>

PaymentView::PaymentView(QWidget *parent) :
    QWidget(parent)
{
    createComponent();
    connections();
    statics();

    canAddPaymentForm = true;
    canAddPaymentFormBecauseIsChidren = true;
}

void PaymentView::createComponent()
{
    lViewBg = new QLabel(this);

    wUp = new QWidget(this);

    swPanels = new QStackedWidget(wUp);

    wViewValues = new QWidget(wUp);

    lTotalStatic = new QLabel(wViewValues);
    lTotal = new QLabel(wViewValues);

    lPaidStatic = new QLabel(wViewValues);
    lPaid = new QLabel(wViewValues);

    lTransshipmentStatic = new QLabel(wViewValues);
    lTransshipment = new QLabel(wViewValues);

    lClient = new Label(this,true,100);

    wViewClient = new QWidget(wUp);

    pbSelectPeople = new QPushButton(wViewClient);
    pbSelectBusiness = new QPushButton(wViewClient);

    swPanelsClient = new QStackedWidget(wViewClient);

    wGroupClient = new QWidget(wViewClient);

    ePhone = new phoneEdit(wGroupClient,true,tr("Telefone"), 14);
    eCep = new Edit(wGroupClient,true,tr("Cep"), 9);
    eName = new Edit(wGroupClient,true,tr("Nome"), 60);
    eStreet = new Edit(wGroupClient,true,tr("Rua"), 50);
    eNumber = new Edit(wGroupClient,true,tr("Nº"), 5);
    eComplement = new Edit(wGroupClient,true,tr("Complemento"), 50);
    eDistrict = new Edit(wGroupClient,true,tr("Bairro"), 30);

    DSBShipping = new DoubleSpinBox(wGroupClient);
    teNotes = new QTextEdit(wGroupClient);
    pbLeave = new QPushButton(wGroupClient);

    wGroupBusiness = new QWidget(wViewClient);

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
    pbLeaveBusiness = new QPushButton(wGroupBusiness);

    wPaymentForm = new QWidget(this);
    lPaymentForm = new QLabel(wPaymentForm);

    cbPaymentForm = new ComboBox(wPaymentForm);
    DSBValuePaymentForm = new DoubleSpinBox(wPaymentForm);
    pbAddPaymentForm = new QPushButton(wPaymentForm);
    pbRemovePaymentForm = new QPushButton(wPaymentForm);
    tablePaymentForm = new TableView(wPaymentForm);

    order = new Order("connectionPayment");

    QList<QString> headers;

    headers << tr("Forma de pagamento") << tr("Valor");

    dataModelPayment = new DataModel(this, headers);

}

void PaymentView::adjustLayout()
{    
    wUp->setGeometry(0,0,this->width(), this->height() * 0.55);

    swPanels->setGeometry(0,0,wUp->width(), wUp->height());

    lViewBg->setGeometry(0,5,swPanels->width() - 5, swPanels->height() - 10);

    wViewValues->setGeometry(0,0,swPanels->width(),swPanels->height());

    wViewClient->setGeometry(0,0,swPanels->width(),swPanels->height());

    swPanelsClient->setGeometry(0,50,wViewClient->width(), wViewClient->height() - 50);

    wGroupClient->setGeometry(0,0,swPanelsClient->width(), swPanelsClient->height());

    wGroupBusiness->setGeometry(0,0,swPanelsClient->width(), swPanelsClient->height());

    teNotes->setGeometry(20,DSBShipping->y() + DSBShipping->height() + 7,240,
                         wGroupClient->height() - (DSBShipping->y() + DSBShipping->height() + 57));

    pbLeave->setGeometry(95,teNotes->y() + teNotes->height() + 10,90,30);

    teNotesBusiness->setGeometry(20,DSBShippingBusiness->y() + DSBShippingBusiness->height() + 7,240,
                                 wGroupBusiness->height() - (DSBShippingBusiness->y() + DSBShippingBusiness->height() + 57));

    pbLeaveBusiness->setGeometry(95,teNotesBusiness->y() + teNotesBusiness->height() + 10,90,30);

    wPaymentForm->setGeometry(0,wViewValues->height(),this->width(), this->height() * 0.45);
    lPaymentForm->setGeometry(0,0,wPaymentForm->width() - 5, wPaymentForm->height() - 5);

    tablePaymentForm->setGeometry(7,pbAddPaymentForm->y() + 38,260,wPaymentForm->height() - (pbAddPaymentForm->y() + 50));
    tablePaymentForm->setColumnWidth(0,tablePaymentForm->width() * 0.65 -1);
    tablePaymentForm->setColumnWidth(1,tablePaymentForm->width() * 0.35 -1);

}


void PaymentView::input()
{
    lClient->setImage(":/treatment/icoClient");
    lClient->setHover(":/treatment/icoClient-hover");
    lClient->setAlternateImage(":/treatment/icoNote-alternate");

    swPanels->addWidget(wViewValues);
    swPanels->addWidget(wViewClient);

    swPanelsClient->addWidget(wGroupClient);
    swPanelsClient->addWidget(wGroupBusiness);

    lViewBg->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.498,"
                           "y1:0, x2:0.494636, y2:1, stop:0 rgba(255, 255, 255, 210),"
                           "stop:0.982955 rgba(255, 255, 255, 170));"
                           "border-radius: 5px;border: 1px solid rgba(255,255,255,255);");

    QFont f = this->font();


#if defined(Q_WS_X11)
    f.setPointSize(13);
#endif

#if defined(Q_WS_WIN)
    f.setPointSize(15);
#endif   

    lTotal->setAlignment(Qt::AlignCenter);
    lTotal->setFont(f);
    lTotal->setText(tr("R$ 0,00 "));
    lTotal->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.498,"
                          "y1:0, x2:0.494636, y2:1, stop:0 rgba(0, 0, 0, 210),"
                          "stop:0.982955 rgba(0, 0, 0, 170));"
                          "border-radius: 15px;border: 1px solid rgba(0,0,0,0);"
                          "color: white;");

    lPaid->setAlignment(Qt::AlignCenter);
    lPaid->setFont(f);
    lPaid->setText(tr("R$ 0,00"));
    lPaid->setStyleSheet(lTotal->styleSheet());

    lTransshipment->setAlignment(Qt::AlignCenter);
    lTransshipment->setFont(f);
    lTransshipment->setText(tr("R$ 0,00"));
    lTransshipment->setStyleSheet(lTotal->styleSheet());

#if defined(Q_WS_X11)
    f.setPointSize(11);
#endif

#if defined(Q_WS_WIN)
    f.setPointSize(13);
#endif

    f.setItalic(true);

    lTotalStatic->setText(tr("Total"));
    lTotalStatic->setFont(f);
    lTotalStatic->setStyleSheet("color: black");

    lPaidStatic->setText(tr("Pago"));
    lPaidStatic->setFont(f);
    lPaidStatic->setStyleSheet("color: black");

    lTransshipmentStatic->setText(tr("Troco"));
    lTransshipmentStatic->setFont(f);
    lTransshipmentStatic->setStyleSheet("color: black");

    lPaymentForm->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.498,"
                                "y1:0, x2:0.494636, y2:1, stop:0 rgba(255, 255, 255, 210),"
                                "stop:0.982955 rgba(255, 255, 255, 170));"
                                "border-radius: 5px;border: 1px solid rgba(255,255,255,255);");

    pbSelectPeople->setToolTip(tr("Cliente Pessoa Física"));
    pbSelectPeople->setText(tr("Pessoa"));
    pbSelectPeople->setCursor(Qt::PointingHandCursor);

    pbSelectBusiness->setToolTip(tr("Cliente Empresarial"));
    pbSelectBusiness->setText(tr("Empresa"));
    pbSelectBusiness->setCursor(Qt::PointingHandCursor);

    pbSelectBusiness->setStyleSheet("border-radius: 2px;"
                                    "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                    "stop: 0 rgb(80,80,80), stop: 1 rgb(60,60,60));"
                                    "color: rgb(220,220,220);"
                                    "border: 1px solid rgb(220,220,220,50);");

    pbSelectPeople->setStyleSheet("border-radius: 2px;"
                                  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                  "stop: 0 rgb(240,240,240), stop: 1 rgb(220,220,220));"
                                  "color: rgb(60,60,60);"
                                  "border: 1px solid rgba(60,60,60,25);");

    DSBShipping->setPrefix(tr("Frete R$ "));
    DSBShipping->setFont(this->font());

    DSBShippingBusiness->setPrefix(tr("Frete R$ "));
    DSBShippingBusiness->setFont(this->font());

    teNotes->setStyleSheet("background-color: qlineargradient"
                           "(spread:pad, x1:0, y1:1, x2:0.165045,"
                           "y2:0, stop:0 rgba(254, 255, 180, 255),"
                           "stop:0.721591 rgba(255, 250, 205, 255));"
                           "border-radius: 10px;border: 1px solid #C0C0C0;");

    teNotesBusiness->setStyleSheet(teNotes->styleSheet());
    teNotesBusiness->setToolTip(tr("Anotações"));

    pbLeave->setText(tr("Limpar"));
    pbLeave->setFont(this->font());

    pbLeaveBusiness->setText(tr("Limpar"));
    pbLeaveBusiness->setFont(this->font());

    ePhone->setInputMask("    -     ");
    eCep->setInputMask("     -   ");

    ePhoneBusiness->setInputMask("    -     ");
    eCepBusiness->setInputMask("     -   ");

    cbPaymentForm->addItems(order->paymentTypes());
    cbPaymentForm->setToolTip(tr("Forma de pagamento"));
    cbPaymentForm->setObjectName("cbPaymentForm");
    cbPaymentForm->setStyleSheet(this->styleSheet());
    cbPaymentForm->setFont(this->font());

    DSBValuePaymentForm->setPrefix(tr("Valor R$: "));
    DSBValuePaymentForm->setFont(this->font());
    DSBValuePaymentForm->setMaximum(999999999.000000);

    pbAddPaymentForm->setText(tr("Adicionar a lista"));
    pbAddPaymentForm->setFont(this->font());

    pbRemovePaymentForm->setText(tr("Remover da lista"));
    pbRemovePaymentForm->setFont(this->font());

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

    tablePaymentForm->setModel(dataModelPayment);

    ePhone->setCompleter(order->bd.getCompleter("phone"));

    ePhoneBusiness->setCompleter(order->bd.getCompleter("phoneBusiness"));

    typeClient = NORMAL;
}

void PaymentView::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void PaymentView::statics()
{
    lClient->setGeometry(235,10,35,35);

    lTotalStatic->setGeometry(10,45,100,20);
    lTotal->setGeometry(40,lTotalStatic->y() + lTotalStatic->height() + 10,200,50);

    lPaidStatic->setGeometry(10,lTotal->y() + lTotal->height() + 15,100,25);
    lPaid->setGeometry(40,lPaidStatic->y() + lPaidStatic->height() + 10,200,50);

    lTransshipmentStatic->setGeometry(10,lPaid->y() + lPaid->height() + 15,100,20);
    lTransshipment->setGeometry(40,lTransshipmentStatic->y() + lTransshipmentStatic->height() + 10,200,50);

    pbSelectPeople->setGeometry(80,12,60,26);
    pbSelectBusiness->setGeometry(pbSelectPeople->x() + pbSelectPeople->width(),12,60,26);

    ePhone->setGeometry(20,0,120,25);
    eCep->setGeometry(ePhone->x() + ePhone->width() + 5,ePhone->y(),115,25);
    eName->setGeometry(20,ePhone->y() + ePhone->height() + 7,240,25);
    eStreet->setGeometry(20,eName->y() + eName->height() + 7,240,25);
    eNumber->setGeometry(20,eStreet->y() + eStreet->height() + 7,100,25);
    eComplement->setGeometry(eNumber->x() + eNumber->width() + 5,eNumber->y(),135,25);
    eDistrict->setGeometry(20,eNumber->y() + eNumber->height() + 7,240,25);
    DSBShipping->setGeometry(20,eDistrict->y() + eDistrict->height() + 7,240,25);

    ePhoneBusiness->setGeometry(20,0,120,25);
    eCepBusiness->setGeometry(ePhoneBusiness->x() + ePhoneBusiness->width() + 5,ePhoneBusiness->y(),115,25);

    eNameContactBusiness->setGeometry(20,eCepBusiness->y() + eCepBusiness->height() + 7,120,25);
    eNameBusiness->setGeometry(eNameContactBusiness->x() + eNameContactBusiness->width() + 5,eNameContactBusiness->y(),115,25);

    eFantasyNameBusiness->setGeometry(20,eNameBusiness->y() + eNameBusiness->height() + 7,120,25);
    eCNPJBusiness->setGeometry(eFantasyNameBusiness->x() + eFantasyNameBusiness->width() + 5,eFantasyNameBusiness->y(),115,25);

    eStreetBusiness->setGeometry(20,eCNPJBusiness->y() + eCNPJBusiness->height() + 7,240,25);
    eNumberBusiness->setGeometry(20,eStreetBusiness->y() + eStreetBusiness->height() + 7,100,25);
    eComplementBusiness->setGeometry(eNumberBusiness->x() + eNumberBusiness->width() + 5,eNumberBusiness->y(),135,25);
    eDistrictBusiness->setGeometry(20,eNumberBusiness->y() + eNumberBusiness->height() + 7,240,25);
    DSBShippingBusiness->setGeometry(20,eDistrictBusiness->y() + eDistrictBusiness->height() + 7,240,25);

    cbPaymentForm->setGeometry(7,10,125,28);

    DSBValuePaymentForm->setGeometry(cbPaymentForm->x() + cbPaymentForm->width() + 8,cbPaymentForm->y(),125,28);

    pbAddPaymentForm->setGeometry(cbPaymentForm->x(), cbPaymentForm->y() + 38,125,28);

    pbRemovePaymentForm->setGeometry(pbAddPaymentForm->x() + pbAddPaymentForm->width() + 8,pbAddPaymentForm->y(),125,28);
}

void PaymentView::connections()
{
    connect(lClient,SIGNAL(onClick()),this,SLOT(onClick_lClient()));

    connect(pbSelectPeople,SIGNAL(clicked()),SLOT(onClick_pbSelectPeople()));

    connect(pbSelectBusiness,SIGNAL(clicked()),SLOT(onClick_pbSelectBusiness()));

    connect(pbLeave,SIGNAL(clicked()),SLOT(onClick_pbLeave()));

    connect(pbLeaveBusiness,SIGNAL(clicked()),SLOT(onClick_pbLeaveBusiness()));

    connect(ePhone, SIGNAL(textChanged(QString)), SLOT(onTextChanged_ePhone(QString)));

    connect(ePhoneBusiness, SIGNAL(textChanged(QString)), SLOT(onTextChanged_ePhoneBusiness(QString)));

    connect(eCep, SIGNAL(textChanged(QString)), SLOT(onTextChanged_eCep(QString)));

    connect(eCepBusiness, SIGNAL(textChanged(QString)), SLOT(onTextChanged_eCepBusiness(QString)));

    connect(pbAddPaymentForm, SIGNAL(clicked()), SLOT(onClick_pbAddPaymentForm()));

    connect(pbRemovePaymentForm, SIGNAL(clicked()), SLOT(onClick_pbRemovePaymentForm()));

    connect(DSBValuePaymentForm, SIGNAL(returnPressed()), SLOT(onClick_pbAddPaymentForm()));
}

void PaymentView::onClick_pbAddPaymentForm(){
    if (! canAddPaymentForm){
        QMessageBox::warning(this, tr("Operação inválida"), tr("Selecione uma mesa para poder adicionar valores."), QMessageBox::Ok);

        return;
    }

    if (! canAddPaymentFormBecauseIsChidren){
        QMessageBox::warning(this, tr("Operação inválida"), QString("Essa mesa foi marcada para ter outra mesa pagando sua conta, a mesa %1.\nSomente a mesa pagante pode adicionar pagamento.").arg(QString::number(yourFather)), QMessageBox::Ok);

        return;
    }

    if (DSBValuePaymentForm->value() == 0){
        QMessageBox::warning(this, "Selecione um valor", "Por favor, selecione um valor", QMessageBox::Ok);

        DSBValuePaymentForm->setFocus();

        return;
    }

    QList<QString> dataForModel;

    dataForModel << cbPaymentForm->currentText() << QString::number(DSBValuePaymentForm->value());

    dataModelPayment->add(dataForModel);

    updateTotalPaid();

    DSBValuePaymentForm->setValue(0);

    cbPaymentForm->setFocus();
}

void PaymentView::onClick_pbRemovePaymentForm()
{
    if (dataModelPayment->rowCount(QModelIndex()) == 0)
        return;

    if (!tablePaymentForm->currentIndex().isValid())
        return;

    int row = tablePaymentForm->currentIndex().row();

    if(dataModelPayment->removeRow(row))
        return;

    updateTotalPaid();

    DSBValuePaymentForm->setValue(0);

    cbPaymentForm->setFocus();
}

void PaymentView::onClick_lClient()
{
    if(lClient->isAlternate)
    {
        swPanels->setCurrentIndex(0);
        lClient->setAlternate(false);
    }
    else
    {
        swPanels->setCurrentIndex(1);
        lClient->setAlternate(true);
    }
}

void PaymentView::onClick_pbSelectPeople()
{

    QString black = "border-radius: 2px;"
            "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            "stop: 0 rgb(80,80,80), stop: 1 rgb(60,60,60));"
            "color: rgb(220,220,220);"
            "border: 1px solid rgb(220,220,220,50);";

    QString white = "border-radius: 2px;"
            "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            "stop: 0 rgb(240,240,240), stop: 1 rgb(220,220,220));"
            "color: rgb(60,60,60);"
            "border: 1px solid rgba(60,60,60,25);";

    pbSelectPeople->setStyleSheet(white);

    pbSelectBusiness->setStyleSheet(black);

    swPanelsClient->setCurrentIndex(0);

    typeClient = NORMAL;

    order->setTypeClient(NORMAL);

    ePhone->Clear();

    ePhone->setFocus();

    dataClientI.clear();
}

void PaymentView::onClick_pbSelectBusiness()
{
    QString black = "border-radius: 2px;"
            "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            "stop: 0 rgb(80,80,80), stop: 1 rgb(60,60,60));"
            "color: rgb(220,220,220);"
            "border: 1px solid rgb(220,220,220,50);";

    QString white = "border-radius: 2px;"
            "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            "stop: 0 rgb(240,240,240), stop: 1 rgb(220,220,220));"
            "color: rgb(60,60,60);"
            "border: 1px solid rgba(60,60,60,25);";

    pbSelectBusiness->setStyleSheet(white);

    pbSelectPeople->setStyleSheet(black);

    swPanelsClient->setCurrentIndex(1);

    typeClient = BUSINESS;

    order->setTypeClient(BUSINESS);

    ePhoneBusiness->Clear();

    ePhoneBusiness->setFocus();

    dataClientI.clear();
}

void PaymentView::onClick_pbLeave()
{
    ePhone->Clear();
    eCep->Clear();
    eName->Clear();
    eStreet->Clear();
    eNumber->Clear();
    eComplement->Clear();
    eDistrict->Clear();

    DSBShipping->setValue(0);

    ePhone->setFocus();
}

void PaymentView::onClick_pbLeaveBusiness()
{
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

    ePhoneBusiness->setFocus();
}

void PaymentView::onTextChanged_ePhone(QString text){
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

            DSBShipping->setValue(data[8].toDouble());

            teNotes->setText(data[9]);
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

    DSBShipping->setValue(0);

    teNotes->setText("");
}

void PaymentView::onTextChanged_ePhoneBusiness(QString text){
    if (text.length() == ePhoneBusiness->maxLength())
        eCepBusiness->setFocus();

    if (text.length() >= 9){
        QList<QString> data;

        data = order->getDataClient(text);

        if (data.length() > 0){
            dataClientI = data;

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

            DSBShippingBusiness->setValue(data[11].toDouble());

            teNotesBusiness->setText(data[12]);
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

    DSBShippingBusiness->setValue(0);

    teNotesBusiness->setText("");
}

void PaymentView::onTextChanged_eCep(QString text){
    if (text.length() == eCep->maxLength())
        eName->setFocus();
}

void PaymentView::onTextChanged_eCepBusiness(QString text){
    if (text.length() == eCepBusiness->maxLength())
        eNameContactBusiness->setFocus();
}

void PaymentView::updateValuesTotal(double total)
{
    totalValueOrder = total;

    lTotal->setText(tools.convertMoney(QString::number(total)));

    updateOthersValues();
}

void PaymentView::keyPressEvent(QKeyEvent *event)
{
    switch (event->modifiers()){

    case Qt::ControlModifier:{

        if (event->key() == Qt::Key_S)
            emit save();

        if (event->key() == Qt::Key_L)
            emit clear();

        if (event->key() == Qt::Key_C)
            emit cancel();  

        if (event->key() == Qt::Key_M)
            emit pizza();

        if (event->key() == Qt::Key_Tab)
            emit goToPanel(5);

        break;
    }
    }

    if (event->key() == Qt::Key_Backtab)
        emit goToPanel(12);

    if (event->key() == Qt::Key_F11)
        emit fullScreen();
}

void PaymentView::updateModelTable()
{
    tablePaymentForm->setModel(dataModelPayment);

    updateTotalPaid();
}

void PaymentView::updateTotalPaid()
{
    double totalPaid = 0;

    for(int i = 0; i < tablePaymentForm->model()->rowCount(); i++){
        totalPaid += tablePaymentForm->model()->index(i, 1).data().toDouble();
    }

    lPaid->setText(tools.convertMoney(QString::number(totalPaid)));

    this->totalPaid = totalPaid;

    updateValuesTotal(totalValueOrder);

    if (totalPaid - totalValueOrder >= 0)
        lTransshipment->setText(tools.convertMoney(QString::number(totalPaid - totalValueOrder)));
    else
        lTransshipment->setText("R$ 0,00");
}

void PaymentView::updateOthersValues(){
    double totalPaid = 0;

    for(int i = 0; i < tablePaymentForm->model()->rowCount(); i++){
        totalPaid += tablePaymentForm->model()->index(i, 1).data().toDouble();
    }

    if (totalPaid - totalValueOrder >= 0)
        lTransshipment->setText(tools.convertMoney(QString::number(totalPaid - totalValueOrder)));
    else
        lTransshipment->setText("R$ 0,00");
}

void PaymentView::cancelOrdered()
{
    totalValueOrder = 0;

    dataModelPayment->clear();
}

void PaymentView::clearValues()
{
    lTransshipment->setText("R$ 0,00");
    lPaid->setText("R$ 0,00");
    lTotal->setText("R$ 0,00");
}
