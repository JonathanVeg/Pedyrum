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


 #include "Interface/treatmentinterface.h"
#include <QDebug>
#include <QMessageBox>

TreatmentInterface::TreatmentInterface(QWidget *parent) :
    QWidget(parent)
{
    createComponent();
    connections();
}

void TreatmentInterface::createComponent()
{    
    lBackground = new QLabel(this);

    pbDelivery = new QPushButton(this);
    pbTable = new QPushButton(this);
    pbCounter = new QPushButton(this);

    pbOrdered = new QPushButton(this);

    swPanel = new QStackedWidget(this);

    wDelivery = new DeliveryInterface(this);
    wCounter = new CounterInterface(this);
    wTable = new TableInterface(this);
}

void TreatmentInterface::input()
{
    lBackground->setStyleSheet("background-color: rgba(123, 31, 31, 255);");

    pbDelivery->setText(tr("Entregas"));
    pbDelivery->setToolTip(tr("Atendimento para entrega"));
    pbDelivery->setObjectName("pbDelivery");
    pbDelivery->setCursor(Qt::PointingHandCursor);

    pbTable->setText(tr("Mesa"));
    pbTable->setToolTip(tr("Atendimento em mesa"));
    pbTable->setObjectName("pbTable");
    pbTable->setCursor(Qt::PointingHandCursor);

    pbCounter->setText(tr("Balcão"));
    pbCounter->setToolTip(tr("Atendimento no balcão"));
    pbCounter->setObjectName("pbCounter");
    pbCounter->setCursor(Qt::PointingHandCursor);

    pbOrdered->setText(tr("Pedidos"));
    pbOrdered->setToolTip(tr("Visualizar todos os pedidos"));
    pbOrdered->setObjectName("pbOrdered");
    pbOrdered->setCursor(Qt::PointingHandCursor);

    wDelivery->setFont(this->font());    
    wDelivery->setStyleSheet(this->styleSheet());

    wCounter->setFont(this->font());
    wCounter->setStyleSheet(this->styleSheet());

    wTable->setFont(this->font());
    wTable->setStyleSheet(this->styleSheet());

    swPanel->addWidget(wDelivery);
    swPanel->addWidget(wTable);
    swPanel->addWidget(wCounter);

    onClick_pbDelivery();

    setInput();

    wTable->wGroupDesk->wTreatmentTable->started();
}


void TreatmentInterface::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void TreatmentInterface::connections()
{
    connect(this,SIGNAL(setInput()),wDelivery,SLOT(input()));
    connect(this,SIGNAL(setInput()),wCounter,SLOT(input()));
    connect(this,SIGNAL(setInput()),wTable,SLOT(input()));

    connect(pbOrdered,SIGNAL(clicked()),SLOT(onClick_pbOrdered()));
    connect(pbCounter,SIGNAL(clicked()),SLOT(onClick_pbCounter()));

    connect(pbTable,SIGNAL(clicked()),SLOT(onClick_pbTable()));
    connect(pbDelivery,SIGNAL(clicked()),SLOT(onClick_pbDelivery()));

}

void TreatmentInterface::adjustLayout()
{
    lBackground->setGeometry(0,0,this->width(),this->height());

    pbDelivery->setGeometry(15,10,70,28);

    pbTable->setGeometry(pbDelivery->x() + pbDelivery->width(),10,70,28);

    pbCounter->setGeometry(pbTable->x() + pbTable->width(),10,70,28);

    pbOrdered->setGeometry(this->width() - 85,10,70,28);

    swPanel->setGeometry(0,pbDelivery->height() + pbDelivery->y() * 2, this->width(),
                         this->height() - (pbDelivery->height() + pbDelivery->y() * 2));

    wDelivery->setGeometry(0,0,swPanel->width(),swPanel->height());

    wTable->setGeometry(0,0,swPanel->width(),swPanel->height());

    wCounter->setGeometry(0,0,swPanel->width(),swPanel->height());
}

void TreatmentInterface::onClick_pbOrdered()
{
    emit toPanel(7);
}

void TreatmentInterface::onClick_pbCounter()
{
    swPanel->setCurrentIndex(2);

    pbCounter->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                             "stop: 0 rgb(80,80,80), stop: 1 rgb(40,40,40));"
                             "border: 1px solid rgba(245,245,245,180);"
                             "color: rgba(245,245,245,180);");

    pbDelivery->setStyleSheet(this->styleSheet());

    pbTable->setStyleSheet(this->styleSheet());
}

void TreatmentInterface::onClick_pbTable()
{
    swPanel->setCurrentIndex(1);

    pbTable->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                           "stop: 0 rgb(80,80,80), stop: 1 rgb(40,40,40));"
                           "border: 1px solid rgba(245,245,245,180);"
                           "color: rgba(245,245,245,180);");

    pbCounter->setStyleSheet(this->styleSheet());

    pbDelivery->setStyleSheet(this->styleSheet());
}

void TreatmentInterface::onClick_pbDelivery()
{
    swPanel->setCurrentIndex(0);

    pbDelivery->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                              "stop: 0 rgb(80,80,80), stop: 1 rgb(40,40,40));"
                              "border: 1px solid rgba(245,245,245,180);"
                              "color: rgba(245,245,245,180);");

    pbCounter->setStyleSheet(this->styleSheet());

    pbTable->setStyleSheet(this->styleSheet());
}

void TreatmentInterface::setIdOrderEdit(int type, int id)
{
    idOrderEdit = id;

    if (id > 0){
        if (type == 0)
            wDelivery->editOrder(idOrderEdit);
        else if (type == 1)
            return;
        else
            wCounter->editOrder(idOrderEdit);
    }
}

void TreatmentInterface::keyPressEvent(QKeyEvent *event)
{
    switch(event->modifiers())
    {

    case Qt::ControlModifier:{
        if (event->key() == Qt::Key_Tab)
            emit goToPanel(5);

        break;
    }

    }

    if (event->key() == Qt::Key_Backtab){
        goToPanel(11);
    }

    if (event->key() == Qt::Key_F11)
        emit fullScreen();
}

void TreatmentInterface::logout(){
    wDelivery->clearOrder();

    wCounter->clearOrder();
}
