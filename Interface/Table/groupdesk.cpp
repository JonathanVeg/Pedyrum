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


 #include "groupdesk.h"
#include <QDebug>

const char RIGHT_ARROW[35] = ":/Extras/slide-right";
const char LEFT_ARROW[35] = ":/Extras/slide-left";

const char ITEM[35] = ":/Extras/point";
const char ITEM_HOVER[35] = ":/Extras/point-hover";
const char CURRENT_ITEM[35] = ":/Extras/point-alternate";

GroupDesk::GroupDesk(QWidget *parent, int maximum) :
    QWidget(parent)
{
    createComponent(maximum);
    connections();
    input();
}

void GroupDesk::createComponent(int maximum)
{
    lBackground = new QLabel(this);

    swPages = new QStackedWidget(this);

    _generalData = new GeneralData(this);

    wGroup = new QWidget(this);
    {
        wBackground = new QWidget(wGroup);
        {
            swPanelsDesk = new QStackedWidget(wBackground);

            _maximumItens = maximum;

            wPanel0 = new QWidget(wBackground);
            wPanel1 = new QWidget(wBackground);
            wPanel2 = new QWidget(wBackground);
            wPanel3 = new QWidget(wBackground);
            wPanel4 = new QWidget(wBackground);

            for (int index = 0; index < _maximumItens; ++index)
            {
                if(index >= 0 && index < 24)
                    itemDesk[index] = new ItemDesk(wPanel0,tr("Mesa"),index);

                if(index >= 24 && index < 48)
                    itemDesk[index] = new ItemDesk(wPanel1,tr("Mesa"),index);

                if(index >= 48 && index < 72)
                    itemDesk[index] = new ItemDesk(wPanel2,tr("Mesa"),index);

                if(index >= 72 && index < 96)
                    itemDesk[index] = new ItemDesk(wPanel3,tr("Mesa"),index);

                if(index >= 96 && index <= 99)
                    itemDesk[index] = new ItemDesk(wPanel4,tr("Mesa"),index);
            }
        }

        lSeletorLeft = new Label(wGroup);
        lSeletorRight = new Label(wGroup);

        lGroupVisible = new QLabel(wGroup);
    }

    wSelect = new QWidget(this);
    {
        lIndex = new QLabel(wSelect);

        wMenuOpen = new QWidget(wSelect);
        {
            lMenuOpen = new QLabel(wMenuOpen);

            lTreatment = new Label(wMenuOpen);
            lTreatmentStatics = new QLabel(wMenuOpen);

            lJoin = new Label(wMenuOpen);
            lJoinStatics = new QLabel(wMenuOpen);

            lTransfer = new Label(wMenuOpen);
            lTransferStatics = new QLabel(wMenuOpen);

            lMedia = new Label(wMenuOpen);
            lMediaStatics = new QLabel(wMenuOpen);
        }

        lBack = new Label(wSelect);
    }

    wTransfer = new Transfer(this);

    wTreatmentTable = new TreatmentTable(this);

    wTreatmentTable->setGeneralData(_generalData);

    wJoinTable = new JoinTable(this);

    wMedia = new Media(this);

    wGroupMedia = new QWidget(this);
    {
        lGroupMedia = new QLabel(wGroupMedia);
        lGroupMediaTitle = new QLabel(wGroupMedia);
        SBGroupMedia = new SpinBox(wGroupMedia);
        pbSaveGroupMedia = new QPushButton(tr("Salvar"),wGroupMedia);
        pbLeaveGroupMedia = new QPushButton(tr("Cancelar"),wGroupMedia);
    }
}


void GroupDesk::adjustLayout()
{
    swPages->setGeometry(0,0,this->width(),this->height());
    lBackground->setGeometry(0,0,this->width(),this->height());

    wGroup->setGeometry(0,0,this->width(),this->height());

    lSeletorLeft->setGeometry(10,this->height()/2 - 20,30,40);

    lSeletorRight->setGeometry(this->width() - 40,this->height()/2 - 20,30,40);

    wBackground->setGeometry(0,0,this->width(), this->height());

    swPanelsDesk->setGeometry(wBackground->width()/2 - 206,wBackground->height()/2 - 120,430,266);

    wPanel0->setGeometry(0,0,swPanelsDesk->width(),swPanelsDesk->height());
    wPanel1->setGeometry(0,0,swPanelsDesk->width(),swPanelsDesk->height());
    wPanel2->setGeometry(0,0,swPanelsDesk->width(),swPanelsDesk->height());
    wPanel3->setGeometry(0,0,swPanelsDesk->width(),swPanelsDesk->height());
    wPanel4->setGeometry(0,0,swPanelsDesk->width(),swPanelsDesk->height());

    adjustItens();

    wMenuOpen->setGeometry(this->width()/2 - 200,this->height()/2 - 60,400,120);    

    lTreatmentStatics->setGeometry(50,90,100,20);
    lTreatment->setGeometry(68,20,64,64);

    lJoinStatics->setGeometry(150,90,100,20);
    lJoin->setGeometry(168,20,64,64);

    lTransferStatics->setGeometry(250,90,100,20);
    lTransfer->setGeometry(268,20,64,64);

    lMediaStatics->setGeometry(350,90,100,20);
    lMedia->setGeometry(368,20,64,64);

    lIndex->setGeometry(this->width()/2 - 33,50,66,50);

    lBack->setGeometry(this->width()/2 - 50,260,100,30);

    lGroupVisible->setGeometry(0,this->height() - 30,this->width(),30);

    wGroupMedia->setGeometry(this->width()/2 - 130, this->height()/2 - 70,260,140);
    lGroupMedia->setGeometry(0,0,wGroupMedia->width(),wGroupMedia->height());
    lGroupMediaTitle->setGeometry(0,14,wGroupMedia->width(),20);
    SBGroupMedia->setGeometry(wGroupMedia->width()/2 - 110,wGroupMedia->height()/2 - 18,220,28);
    pbSaveGroupMedia->setGeometry(wGroupMedia->width()/2 - 110,SBGroupMedia->height()+ SBGroupMedia->y() + 20,100,30);
    pbLeaveGroupMedia->setGeometry(pbSaveGroupMedia->width() + pbSaveGroupMedia->x() + 20,pbSaveGroupMedia->y(),100,30);
}

void GroupDesk::input()
{
    SBGroupMedia->setMaximum(99);
    SBGroupMedia->setMinimum(2);

    swPages->addWidget(wGroup);
    swPages->addWidget(wSelect);
    swPages->addWidget(wTreatmentTable);
    swPages->addWidget(wJoinTable);
    swPages->addWidget(wTransfer);
    swPages->addWidget(wMedia);

    for (int i = 0; i < 100; ++i)
    {
        itemDesk[i]->data->setId(i);
        _generalData->_dataItem[i] = itemDesk[i]->data;
    }

    lBackground->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.498,"
                               "y1:0, x2:0.494636, y2:1, stop:0 rgba(0, 0, 0, 210),"
                               "stop:0.982955 rgba(0, 0, 0, 170));"
                               "border: 1px solid rgba(0,0,0,0);"
                               "border-radius: 20px;");

    lGroupMedia->setStyleSheet("background-color: rgba(255,255,255,200);"
                               "border: 2px solid rgba(255,255,255,220);"
                               "border-radius: 15px;");

    lSeletorRight->setImage(RIGHT_ARROW);

    lSeletorLeft->setImage(LEFT_ARROW);

    swPanelsDesk->addWidget(wPanel0);
    swPanelsDesk->addWidget(wPanel1);
    swPanelsDesk->addWidget(wPanel2);
    swPanelsDesk->addWidget(wPanel3);
    swPanelsDesk->addWidget(wPanel4);

    QFont f;

    f.setPointSize(9);

    lTreatmentStatics->setText(tr("Atendimento"));
    lTreatmentStatics->setFont(f);
    lTreatmentStatics->setAlignment(Qt::AlignCenter);
    lTreatmentStatics->setStyleSheet("color: white");

    lJoinStatics->setText(tr("Juntar Mesas"));
    lJoinStatics->setFont(f);
    lJoinStatics->setAlignment(Qt::AlignCenter);
    lJoinStatics->setStyleSheet("color: white");

    lTransferStatics->setText(tr("Transferir Conta"));
    lTransferStatics->setFont(f);
    lTransferStatics->setAlignment(Qt::AlignCenter);
    lTransferStatics->setStyleSheet("color: white");

    lMediaStatics->setText(tr("Média"));
    lMediaStatics->setFont(f);
    lMediaStatics->setAlignment(Qt::AlignCenter);
    lMediaStatics->setStyleSheet("color: white");

    lTreatment->setImage(":/treatment/treatment");
    lJoin->setImage(":/treatment/joinTable");
    lTransfer->setImage(":/treatment/transfer");
    lMedia->setImage(":/treatment/media");

    f.setPointSize(11);

    lBack->setAlignment(Qt::AlignCenter);
    lBack->setStyleSheet("color: rgba(255,255,255,220);");
    lBack->setText(tr("Voltar"));    
    lBack->setFont(f);
    lBack->setStyleSheet("background-color: green;"
                         "color: rgba(220,220,220,220);"
                         "border-radius: 5px; border: 1px solid rgba(255,255,255,10);");

    f.setPointSize(18);

    lIndex->setStyleSheet("background-color: green;"
                          "color: rgba(220,220,220,220);"
                          "border-radius: 5px; border: 1px solid rgba(255,255,255,10);");

    lIndex->setAlignment(Qt::AlignCenter);
    lIndex->setFont(f);

    f.setPixelSize(13);
    lGroupVisible->setAlignment(Qt::AlignCenter);
    lGroupVisible->setFont(f);
    lGroupVisible->setStyleSheet("color: rgb(255, 170, 0);");
    lGroupVisible->setVisible(false);

    f.setPointSize(12);
    lGroupMediaTitle->setText(tr("Média"));
    lGroupMediaTitle->setFont(f);
    lGroupMediaTitle->setAlignment(Qt::AlignCenter);
    lGroupMediaTitle->setStyleSheet("color: rgba(0,0,0,220);");

    SBGroupMedia->setPrefix(tr("Número de pessoas pagantes: "));

    wTreatmentTable->setWGroupMedia(wGroupMedia, SBGroupMedia);

    wGroupMedia->hide();

    lMedia->clear();
    lMediaStatics->clear();
}

void GroupDesk::onClick_lSeletorRight()
{
    if(swPanelsDesk->currentIndex() == 4)
        swPanelsDesk->setCurrentIndex(0);
    else    
        swPanelsDesk->setCurrentIndex(swPanelsDesk->currentIndex() + 1);

}

void GroupDesk::onClick_lSeletorLeft()
{
    if(swPanelsDesk->currentIndex() == 0)
        swPanelsDesk->setCurrentIndex(4);
    else    
        swPanelsDesk->setCurrentIndex(swPanelsDesk->currentIndex() - 1);    
}


void GroupDesk::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void GroupDesk::connections()
{

    // seletores das pÃ¡ginas de mesa

    connect(lSeletorLeft,SIGNAL(onClick()),SLOT(onClick_lSeletorLeft()));

    connect(lSeletorRight,SIGNAL(onClick()),SLOT(onClick_lSeletorRight()));


    // Menu de atividades

    connect(lTreatment,SIGNAL(onClick()),SLOT(onClick_lTreatment()));

    connect(lTransfer,SIGNAL(onClick()),SLOT(onClick_lTransfer()));

    connect(lJoin,SIGNAL(onClick()),SLOT(onClick_lJoin()));

    connect(lMedia,SIGNAL(onClick()),SLOT(onClick_lMedia()));


    // voltar a group desk

    connect(lBack,SIGNAL(onClick()),SLOT(back()));

    connect(wTreatmentTable,SIGNAL(back()),SLOT(back()));

    connect(wTransfer,SIGNAL(back()),SLOT(back()));

    connect(wJoinTable,SIGNAL(back()),SLOT(back()));

    connect(wMedia,SIGNAL(back()),SLOT(back()));


    // click na mesa

    for (int i = 0; i < 100; ++i)
    {
        connect(itemDesk[i],SIGNAL(onClick(int)),SLOT(onClick_Items(int)));
        connect(itemDesk[i],SIGNAL(onDoubleClicked(int)), SLOT(onDoubleClick_Items(int)));
    }

    for (int i = 0; i < 100; ++i)
    {
        connect(itemDesk[i],SIGNAL(groupVisible(bool,QList<int>)),SLOT(groupVisible(bool,QList<int>)));
    }

    connect(wMedia,SIGNAL(updateData(int,bool)),SLOT(updateData(int,bool)));

    connect(wTransfer,SIGNAL(updateData(int,bool)),SLOT(updateData(int,bool)));

    connect(wJoinTable,SIGNAL(updateData(int,bool)),SLOT(updateData(int,bool)));

    connect(wTreatmentTable,SIGNAL(updateData(int,bool)),SLOT(updateData(int,bool)));

    connect(_generalData,SIGNAL(updateData()),SLOT(convertData()));


    // recebimento do model
    connect(wTreatmentTable, SIGNAL(sendModelItems(DataModel*)), SLOT(receivedModelItems(DataModel*)));


    // Media group
    connect(pbSaveGroupMedia,SIGNAL(clicked()),SLOT(onClick_pbSaveGroupMedia()));
    connect(pbLeaveGroupMedia,SIGNAL(clicked()),SLOT(onClick_pbLeaveGroupMedia()));

    connect(wTreatmentTable, SIGNAL(closeWGroupMedia()), SLOT(onClick_pbLeaveGroupMedia()));

    connect(SBGroupMedia, SIGNAL(returnPressed()), SLOT(onClick_pbSaveGroupMedia()));

    // fechamento de mesa

    connect(wTreatmentTable,SIGNAL(closeTable()),SLOT(closeTable()));
}

void GroupDesk::adjustItens()
{
    short int x = 0;
    short int y = 0;
    short int w = 60;
    short int h = 50;
    short int space = 10;

    short int old = 0;
    short int countColumn = 0;
    short int countRow = 0;

    for (int i = 0; i < 100; ++i) {

        if(i/6 != old)
        {
            old = i/6;

            if(countRow >= 3)
                countRow = 0;
            else
                countRow++;

            countColumn = 0;
        }
        else
            countColumn++;

        if(i == 0)
            countColumn = 0;

        x = countColumn * (w + space);

        y = countRow * (h + space);

        itemDesk[i]->setGeometry(x,y,w,h);
    }
}

void GroupDesk::setPaymentView(PaymentView *paymentView){
    _paymentView = paymentView;
}

void GroupDesk::setTableItem(TableView *tableItem){
    _tableItem = tableItem;
}

void GroupDesk::onDoubleClick_Items(int index){
    onClick_lTreatment();

    QString temp;
    temp.setNum(index);
    _selected = index;

    currentIndexItem = index;

    lIndex->setText(temp);

    // seta o model da mesa corrente.

    setModels(_generalData->getModelItems(index), _generalData->getModelPayment(index));

    _paymentView->canAddPaymentForm = true;
}

void GroupDesk::onClick_Items(int index)
{
    QString temp;
    temp.setNum(index);
    _selected = index;

    currentIndexItem = index;

    lIndex->setText(temp);
    swPages->setCurrentIndex(1);

    // seta o model da mesa corrente.

    setModels(_generalData->getModelItems(index), _generalData->getModelPayment(index));

    if (itemDesk[selected()]->data->isChildren()){
        qDebug() << "Desativar PaymentView aqui, pq ele é filho.";

        _paymentView->canAddPaymentFormBecauseIsChidren = false;
        _paymentView->yourFather = itemDesk[selected()]->data->father();

    }else
        _paymentView->canAddPaymentFormBecauseIsChidren = true;

    _paymentView->canAddPaymentForm = true;

    DataOrder * dataOrder = _generalData->getDataOrder(index);

    dataOrder->print();

    try{
        wTreatmentTable->setSelectedTable(index);
        wTreatmentTable->orderQuantity = dataOrder->orderQuantity;
        wTreatmentTable->orderValues = dataOrder->orderValues;
        wTreatmentTable->orderSizes = dataOrder->orderSizes;
        wTreatmentTable->orderPartProduct = dataOrder->orderPartProduct;
        wTreatmentTable->orderIdProduct = dataOrder->orderIdProduct;
        wTreatmentTable->orderNameProduct = dataOrder->orderNameProduct;
        wTreatmentTable->orderNoteProduct = dataOrder->orderNoteProduct;
    }catch(...){
        qDebug() << "[GroupDesk] [onClick_Items] erro aqui. Index:" << index;
    }
}

void GroupDesk::onClick_lTreatment()
{
    updateData(0,false);
    wTreatmentTable->load(_selected);
    swPages->setCurrentIndex(2);
}

void GroupDesk::onClick_lJoin()
{
    updateData(1,false);
    wJoinTable->load(_selected);
    swPages->setCurrentIndex(3);
}

void GroupDesk::onClick_lTransfer()
{
    updateData(2,false);
    wTransfer->load(_selected);
    swPages->setCurrentIndex(4);
}

void GroupDesk::onClick_lMedia()
{
    updateData(3,false);
    wMedia->load(_selected);
    swPages->setCurrentIndex(5);
}

void GroupDesk::keyPressEvent(QKeyEvent *event)
{
    switch(event->modifiers()){

    case Qt::ControlModifier:{
        if (event->key() == Qt::Key_S)
            wTreatmentTable->saveOrder();
    }

    }

    if (event->key() == Qt::Key_Escape){
        if (! wTreatmentTable->gettingMedia)
            back();
        else
            onClick_pbLeaveGroupMedia();
    }
}

void GroupDesk::back()
{
    qDebug() << "[GroupDesk] [back] local de atualizaÃ§Ã£o do dataOrder";

    // atualiza a GeneralData

    DataOrder * dataOrder = new DataOrder();

    dataOrder->orderQuantity = wTreatmentTable->orderQuantity;
    dataOrder->orderValues = wTreatmentTable->orderValues;
    dataOrder->orderSizes = wTreatmentTable->orderSizes;
    dataOrder->orderPartProduct = wTreatmentTable->orderPartProduct;
    dataOrder->orderIdProduct = wTreatmentTable->orderIdProduct;
    dataOrder->orderNameProduct = wTreatmentTable->orderNameProduct;
    dataOrder->orderNoteProduct = wTreatmentTable->orderNoteProduct;

    _generalData->setDataOrder(_selected, dataOrder);

    // resto da back original

    swPages->setCurrentIndex(0);

    setModels(_generalData->getModelItems(_generalData->EMPTY_MODEL), NULL, true);

    _paymentView->canAddPaymentForm = false;
}

void GroupDesk::updateData(int index,bool import)
{
    if(import)
    {
        switch(index)
        {

        case 0: {
            for (int i = 0; i < 100; ++i) {
                itemDesk[i]->data = wTreatmentTable->_dataItem[i];
            }
            for (int i = 0; i < 100; i ++){
                treatment(i);
            }
            break;
        };

        case 1: {
            for (int i = 0; i < 100; ++i) {
                itemDesk[i]->data = wJoinTable->_data[i];
            }
            for (int i = 0; i < 100; ++i) {
                join(i);
            }
            break;
        };

        case 2: {
            for (int i = 0; i < 100; ++i) {
                itemDesk[i]->data = wTransfer->_data[i];
            }
            for (int i = 0; i < 100; ++i) {
                transfer(i);
            }
            break;
        };

        case 3: {
            for (int i = 0; i < 100; ++i) {
                itemDesk[i]->data = wMedia->_data[i];
            }break;
        };
        }

        for (int i = 0; i < 100; ++i){
            _generalData->_dataItem[i] = itemDesk[i]->data;
        }

    }
    else
    {
        switch(index)
        {

        case 0: {
            for (int i = 0; i < 100; ++i) {
                wTreatmentTable->_dataItem[i] = itemDesk[i]->data;
            }break;
        };

        case 1: {
            for (int i = 0; i < 100; ++i) {
                wJoinTable->_data[i] = itemDesk[i]->data;
            }break;
        };

        case 2: {
            for (int i = 0; i < 100; ++i) {
                wTransfer->_data[i] = itemDesk[i]->data;
            }break;
        };

        case 3: {
            for (int i = 0; i < 100; ++i) {
                wMedia->_data[i] = itemDesk[i]->data;
            }break;
        };        
        }
    }
}

void GroupDesk::transfer(int i)
{    
    if(itemDesk[i]->data->isFather() || itemDesk[i]->data->isChildren())
    {
        qDebug() << "\ti:" << i << "IsFather?" << itemDesk[i]->data->isFather() << "Father:" << itemDesk[i]->data->father();
        qDebug() << "\ti:" << i << "IsChildren?" << itemDesk[i]->data->isChildren() << "Children:" << itemDesk[i]->data->children();

        itemDesk[i]->data->setFree(false);
        itemDesk[i]->setFree(false);

        if(itemDesk[i]->data->isFather())
        {
            if(itemDesk[itemDesk[i]->data->children()]->data->isChildren())
                itemDesk[i]->setFather(true);
            else
                itemDesk[i]->setFather(false);
        }
        else
        {
            if(itemDesk[i]->data->isChildren())
                itemDesk[i]->setChildren(true);
        }
    }
    else
    {
        if(itemDesk[i]->data->isFree())
            itemDesk[i]->setFree(true);
        else
        {
            if(!itemDesk[i]->data->isCompound())
                itemDesk[i]->setFree(false);
        }

        itemDesk[i]->setFather(false);
        itemDesk[i]->setChildren(false);
    }
}

void GroupDesk::join(int i)
{
    if(itemDesk[i]->data->isCompound())
    {
        itemDesk[i]->data->setFree(false);

        itemDesk[i]->setCompound(true);
    }
    else
    {
        itemDesk[i]->setCompound(false);
    }
}

void GroupDesk::treatment(int i)
{
    if (!itemDesk[i]->data->isCompound()){
        itemDesk[i]->setFree(itemDesk[i]->data->isFree());
    }
}

void GroupDesk::groupVisible(bool b, QList<int> list)
{
    lGroupVisible->setVisible(b);
    QString temp;
    QString line;

    for (int i = 0; i < list.count(); ++i) {
        temp.setNum(list.value(i));
        if(list.value(i) != list.last())
            line+= temp + " | ";
        else
            line += temp;

        itemDesk[list.value(i)]->setHover(b);
    }

    lGroupVisible->setText(line);
}

void GroupDesk::convertData()
{
    for (int i = 0; i < 100; ++i) {
        itemDesk[i]->data = _generalData->_dataItem[i];
    }
}

void GroupDesk::receivedModelItems(DataModel * model)
{    
    if (model->rowCount(QModelIndex()) > 0)
        _generalData->setModelItems(currentIndexItem, model);
    else
        model = _generalData->getModelItems(currentIndexItem);

    setModels(model, NULL);
}

void GroupDesk::setModels(DataModel *modelItems, DataModel *modelPayments, bool clearModel){
    _tableItem->setModel(modelItems);

    wTreatmentTable->setDataModelItems(modelItems);

    if (modelPayments != NULL){
        _paymentView->dataModelPayment = modelPayments;

        _paymentView->tablePaymentForm->setModel(_paymentView->dataModelPayment);
    }

    _tableItem->setColumnWidth(0,_tableItem->width() * 0.18);
    _tableItem->setColumnWidth(1,_tableItem->width() * 0.14);
    _tableItem->setColumnWidth(2,_tableItem->width() * 0.14);
    _tableItem->setColumnWidth(3,_tableItem->width() * 0.1);
    _tableItem->setColumnWidth(4,_tableItem->width() * 0.44 - 1);

    double total = 0;
    for (int i = 0; i < modelItems->rowCount(QModelIndex()); i++){
        total += modelItems->index(i, 2).data().toDouble() * modelItems->index(i, 3).data().toDouble();
    }

    if (! clearModel){
        _paymentView->updateValuesTotal(total);
        _paymentView->updateTotalPaid();
    }else{
        _generalData->setModelPayment(_selected, _paymentView->dataModelPayment);

        _paymentView->dataModelPayment =_generalData->getModelPayment(_generalData->EMPTY_MODEL); // limpar

        _paymentView->updateTotalPaid();

        _paymentView->updateOthersValues();

        _paymentView->canAddPaymentForm = false;

        _paymentView->tablePaymentForm->setModel(_paymentView->dataModelPayment);

        _paymentView->clearValues();
    }
}

void GroupDesk::onClick_pbSaveGroupMedia()
{
    wTreatmentTable->gettingMedia = false;

    wGroupMedia->hide();

    int countPeople = SBGroupMedia->value();
    SBGroupMedia->setValue(2);

    if (SBGroupMedia->value() > 0)
        wTreatmentTable->saveOrderPt2(true, countPeople);
    else
        wTreatmentTable->saveOrderPt2();

}

void GroupDesk::onClick_pbLeaveGroupMedia()
{
    wTreatmentTable->gettingMedia = false;

    SBGroupMedia->setValue(2);
    wGroupMedia->hide();

    wTreatmentTable->saveOrderPt2();
}


void GroupDesk::closeTable()
{
    for (int i = 0; i < 100; ++i) {
        transfer(i);
    }
}
