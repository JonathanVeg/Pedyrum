/*
Copyright (c) 2013, Luiz C�ndido da Silva J�nior, heron@jaguati.com.br

Copyright (c) 2013, Jonathan,  jonathan@jaguati.com.br
 All rights reserved. 

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 

 Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 

 Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. 

 Neither the name of Pedyrum may be used to endorse or promote products derived from this software without specific prior written permission. 

 THIS SOFTWARE IS PROVIDED BY {{THE COPYRIGHT HOLDERS AND CONTRIBUTORS}} "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL {{THE COPYRIGHT HOLDER OR CONTRIBUTORS}} BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


 #include "selectdeliverer.h"

SelectDeliverer::SelectDeliverer(QWidget *parent) :
    QWidget(parent)
{
    createComponent();

    statics();

    input();

    connections();
}

void SelectDeliverer::createComponent()
{
    lBackground = new QLabel(this);

    lTitle = new QLabel(this);

    cbWaiters = new ComboBox(this);

    textAlert = new QTextBrowser(this);

    pbConfirm = new QPushButton(this);
}

void SelectDeliverer::statics()
{
    lTitle->setGeometry(20, 10, 200, 20);

    cbWaiters->setGeometry(20, 40, 200, 28);

    textAlert->setGeometry(10, 70, 220, 60);

    pbConfirm->setGeometry(75, textAlert->y() + textAlert->height(), 90, 28);
}

void SelectDeliverer::connections()
{
    connect(pbConfirm, SIGNAL(clicked()), SLOT(onClick_pbConfirm()));
}

void SelectDeliverer::resizeEvent(QResizeEvent *)
{
    lBackground->setGeometry(0,0,this->width(),this->height());
}

void SelectDeliverer::input()
{
    lBackground->setStyleSheet("background-color: rgba(0,0,0,140);"
                               "border: 10px solid rgba(0,0,0,50);");
    QFont f;

    f.setPointSize(10);

    lTitle->setText(tr("Selecione o entregador:"));
    lTitle->setStyleSheet("color: rgba(255,255,255,240);");
    lTitle->setFont(f);

    f.setPointSize(9);

    textAlert->setText(tr("N�o se esque�a de marcar o entregador como"
                       " livre ap�s o retorno ao estabelecimento."));

    textAlert->setStyleSheet("background-color: rgba(0,0,0,0);"
                             "color: rgba(255,255,255,240);");

    textAlert->setAlignment(Qt::AlignCenter);

    textAlert->setFont(f);

    pbConfirm->setText(tr("Confirmar"));
}

void SelectDeliverer::setWaiters(QList<QString> namesWaiters, QList<QString> idsWaiters)
{
    cbWaiters->clear();

    cbWaiters->addItems(namesWaiters);

    this->idsWaiters = idsWaiters;
}

void SelectDeliverer::onClick_pbConfirm()
{
    emit sendDeliverer(idsWaiters[cbWaiters->currentIndex()]);

    this->close();
}
