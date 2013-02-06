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


 #include "transfer.h"
#include <QMessageBox>

Transfer::Transfer(QWidget *parent) :
    QWidget(parent)
{
    createComponent();
    connections();
    input();
}

void Transfer::createComponent()
{
    for (int i = 0; i < 100; ++i) {
        _data[i] = new DataItem(this);
    }

    textSubsidized = new QLabel(tr("Subsidiado"),this);
    textBuyer = new QLabel(tr("Comprador"),this);

    cbSubsidized = new ComboBox(this);
    cbBuyer = new ComboBox(this);

    pbSave = new QPushButton(tr("Salvar"),this);
    pbBack = new QPushButton(tr("Voltar"),this);
    pbSeparate = new QPushButton(tr("Separar"),this);
}

void Transfer::input()
{
    cbSubsidized->addItem(tr("Mesa Subsidiada"));

    cbBuyer->addItem(tr("Mesa Compradora"));

    QFont f;

    f.setPointSize(12);

    textBuyer->setFont(f);
    textBuyer->setAlignment(Qt::AlignCenter);
    textBuyer->setStyleSheet("color: rgba(255,255,255,200);");

    textSubsidized->setFont(f);
    textSubsidized->setAlignment(Qt::AlignCenter);
    textSubsidized->setStyleSheet("color: rgba(255,255,255,200);");

    QString temp;

    for (int index = 0; index < 100; ++index)
    {
        temp.setNum(index);
        cbBuyer->addItem(tr("Mesa ").append(temp));
    }

    for (int index = 0; index < 100; ++index)
    {
        temp.setNum(index);
        cbSubsidized->addItem(tr("Mesa ").append(temp));
    }

    cbSubsidized->setNextComponent(cbBuyer);
}

void Transfer::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void Transfer::adjustLayout()
{
    textSubsidized->setGeometry(50,90,200,20);
    textBuyer->setGeometry(this->width() - 250,90,200,20);

    cbSubsidized->setGeometry(50,textSubsidized->y() + textSubsidized->height() + 10,200,28);
    cbBuyer->setGeometry(this->width() - 250,textSubsidized->y() + textSubsidized->height() + 10,200,28);

    pbSave->setGeometry(this->width()/2 - 170,this->height() - 50,100,30);
    pbBack->setGeometry(pbSave->x() + pbSave->width() + 20,this->height() - 50,100,30);
    pbSeparate->setGeometry(pbBack->x() + pbBack->width() + 20,this->height() - 50,100,30);
}

void Transfer::connections()
{
    connect(pbBack,SIGNAL(clicked()),SLOT(onClick_pbBack()));

    connect(pbSave,SIGNAL(clicked()),SLOT(onClick_pbSave()));

    connect(cbBuyer, SIGNAL(returnPressed()), SLOT(onClick_pbSave()));

    connect(pbSeparate,SIGNAL(clicked()),SLOT(onClick_pbSeparate()));
}

void Transfer::onClick_pbBack()
{
    cbSubsidized->setCurrentIndex(0);
    cbBuyer->setCurrentIndex(0);

    emit back();
}

void Transfer::onClick_pbSave()
{
    if (cbSubsidized->currentIndex() == 0 || cbBuyer->currentIndex() == 0){
        QMessageBox::warning(this, tr("Operação inválida"), tr("Por favor, selecione duas mesas válidas"), QMessageBox::Ok);
        return;
    }

    if (cbSubsidized->currentIndex() == cbBuyer->currentIndex()){
        QMessageBox::warning(this, tr("Operação inválida"), tr("Por favor, selecione duas mesas diferentes"), QMessageBox::Ok);
        return;
    }

    if(_data[cbSubsidized->currentIndex()-1]->isCompound() || _data[cbBuyer->currentIndex()-1]->isCompound()) {
        QMessageBox::warning(this, tr("Operação inválida"), tr("Já há uma operação de transferência nessa mesa"), QMessageBox::Ok);
        return;
    }

    if(_data[cbSubsidized->currentIndex()-1]->isFather() || _data[cbBuyer->currentIndex()-1]->isFather()) {
        QMessageBox::warning(this, tr("Operação inválida"), tr("Esta mesa já é compradora de outra"), QMessageBox::Ok);
        return;
    }

    if(_data[cbSubsidized->currentIndex()-1]->isChildren() || _data[cbBuyer->currentIndex()-1]->isChildren()) {
        QMessageBox::warning(this, tr("Operação inválida"), tr("Esta mesa já é subsídiada de outra"), QMessageBox::Ok);
        return;
    }

    _data[cbSubsidized->currentIndex()-1]->setFather(cbBuyer->currentIndex() - 1);
    _data[cbBuyer->currentIndex()-1]->setChildren(cbSubsidized->currentIndex() - 1);

    emit updateData(2,true);

    onClick_pbBack();
}

void Transfer::load(int index)
{
    if(!_data[index]->isFather() && !_data[index]->isChildren())
    {
        cbSubsidized->setCurrentIndex(_data[index]->id() + 1);        
    }
    else
    {
        if(_data[index]->isFather())
        {
            cbBuyer->setCurrentIndex(_data[index]->id() + 1);            
            cbSubsidized->setCurrentIndex(_data[index]->children() + 1);
        }
        else
        {
            cbSubsidized->setCurrentIndex(_data[index]->id() + 1);
            cbBuyer->setCurrentIndex(_data[index]->father() + 1);
        }
    }
}

void Transfer::onClick_pbSeparate()
{
    _data[cbSubsidized->currentIndex()-1]->clearTransfer();
    _data[cbBuyer->currentIndex()-1]->clearTransfer();

    emit updateData(2,true);

    onClick_pbBack();
}
