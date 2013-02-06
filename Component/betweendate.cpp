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


 #include "betweendate.h"

BetweenDate::BetweenDate(QWidget *parent) :
    QWidget(parent)
{
    createComponent();
    connections();
    input();
}

void BetweenDate::createComponent()
{
    lBackground = new QLabel(this);
    eBeforeDate = new Edit(this, true, tr("Data inicial"));
    eAfterDate = new Edit(this, true, tr("Data final"));
    pbConfirm = new QPushButton(tr("Confirmar"),this);
}

void BetweenDate::connections()
{
    connect(pbConfirm,SIGNAL(clicked()),this,SLOT(onClick_pbConfirm()));

    connect(eBeforeDate,SIGNAL(textChanged(QString)),this,SLOT(onTextChangedBefore(QString)));

    connect(eAfterDate,SIGNAL(textChanged(QString)),this,SLOT(onTextChangedLater(QString)));

    connect(eAfterDate, SIGNAL(returnPressed()), SLOT(onClick_pbConfirm()));
}

void BetweenDate::onClick_pbConfirm()
{
    // aqui terá uma verificação de validade das datas antes do emit

    emit selectedDates(eBeforeDate->text(), eAfterDate->text());
}

void BetweenDate::input()
{
    lBackground->setStyleSheet("background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                               "stop: 1 rgba(170,50,50,230), stop: 0 rgba(125,30,30,190));"
                               "border-radius: 10px;border: 1px solid rgba(255,255,255,255);");

    eBeforeDate->setInputMask("  /  /    ");

    eAfterDate->setInputMask("  /  /    ");

    H_ = 130;

    W_ = 160;

    eBeforeDate->setNextComponent(eAfterDate);
}

void BetweenDate::resizeEvent(QResizeEvent *)
{
    lBackground->setGeometry(0,0,this->width(),this->height());

    eBeforeDate->setGeometry(this->width()/ 2 - 60, 12, 120, 27);

    eAfterDate->setGeometry(eBeforeDate->x(), eBeforeDate->y() + eBeforeDate->height() + 10, 120, 27);

    pbConfirm->setGeometry(this->width()/2 - 45,eAfterDate->y() + eAfterDate->height() + 10,90,30);
}

QString BetweenDate::dateBefore()
{
    return before;
}

QString BetweenDate::laterDate()
{
    return later;
}

void BetweenDate::onTextChangedLater(QString text)
{
    if(eAfterDate->text() != eAfterDate->valueStandart)
        later = text;
}

void BetweenDate::onTextChangedBefore(QString text)
{
    if(eBeforeDate->text() != eBeforeDate->valueStandart)
        before = text;
}
