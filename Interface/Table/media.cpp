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


 #include "media.h"
#include <QDebug>
#include <QMessageBox>

Media::Media(QWidget *parent) :
    QWidget(parent)
{
    createComponent();
    input();
    connections();
}

void Media::createComponent()
{

    for (int i = 0; i < 100; ++i) {
        _data[i] = new DataItem();
    }

    textBody = new QTextBrowser(this);

    lTextSaveInOrdered = new QLabel(tr("Deseja exibir a média no Pedido?"),this);

    YNSaveInOrdered = new YesNo(this,tr("Sim"),tr("Não"));

    DSBNumberPeople = new QSpinBox(this);

    cbTable = new QComboBox(this);

    pbSave = new QPushButton(tr("Salvar"),this);

    pbCancel = new QPushButton(tr("Voltar"),this);

}

void Media::input()
{
    textBody->setStyleSheet("background-color: rgba(0,0,0,0);");

    QFont f;
    f.setPointSize(12);

    lTextSaveInOrdered->setStyleSheet("color: rgba(255,255,255,200);");
    lTextSaveInOrdered->setFont(f);

    DSBNumberPeople->setPrefix("Número de Pessoas ");
    DSBNumberPeople->setMinimum(1);
    DSBNumberPeople->setMaximum(999999);
    setCountClient(1);

    YNSaveInOrdered->setOption(false);

    QString temp;

    cbTable->addItem(tr("Selecione a Mesa"));

    for (int index = 0; index < 100; ++index)
    {
        temp.setNum(index);
        cbTable->addItem(tr("Mesa ").append(temp));
    }
}

void Media::resizeEvent(QResizeEvent *)
{
    lTextSaveInOrdered->setGeometry(50,25,250,20);

    YNSaveInOrdered->setGeometry(lTextSaveInOrdered->x() + lTextSaveInOrdered->width() + 10, lTextSaveInOrdered->y() - 4,100,28);

    DSBNumberPeople->setGeometry(lTextSaveInOrdered->x()-5,YNSaveInOrdered->y() + YNSaveInOrdered->height() + 20,220,28);

    cbTable->setGeometry(DSBNumberPeople->x() + DSBNumberPeople->width() + 20,DSBNumberPeople->y(),220,28);

    textBody->setGeometry(10,DSBNumberPeople->y() + DSBNumberPeople->height() + 10,this->width() - 20,this->height() - 157);

    pbSave->setGeometry(this->width()/2 - 100,this->height() - 40,90,30);

    pbCancel->setGeometry(pbSave->x() + pbSave->width() + 20,this->height() - 40,90,30);
}

void Media::onClick_pbCancel()
{
    cbTable->setEnabled(true);
    cbTable->setCurrentIndex(0);
    setCountClient(1);
    YNSaveInOrdered->setOption(false);
    DSBNumberPeople->setValue(1);

    emit back();
}

void Media::connections()
{
    connect(pbCancel,SIGNAL(clicked()),SLOT(onClick_pbCancel()));

    connect(pbSave,SIGNAL(clicked()),SLOT(onClick_pbSave()));

    connect(DSBNumberPeople,SIGNAL(valueChanged(int)),SLOT(setCountClient(int)));
}

void Media::setCountClient(int index)
{
    _countClient = index;

    float total = 0;

    _media = total / _countClient;

    QString temp;
    QString temp2;

    temp.setNum(index);

    temp2.setNum(_media);

    temp2 = t.convertMoney(temp2);

    QString html = "<html>"
            "<body style=\"font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">"
            "<br><br><br><br><br>"
            "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
            "-qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600;"
            "color:#707070;\">" + tr("Média para ") + "</span><span style=\" font-size:12pt; font-weight:600;"
            "color:#B5B5B5;\">" + temp + "</span><span style=\" font-size:12pt; font-weight:600; color:#707070;\">";

    if (_countClient > 1)
        html += tr(" clientes: ");
    else
        html += tr(" cliente: ");

    html += "</span>"
            "<span style=\" font-size:12pt; font-weight:600; color:#B5B5B5;\">" + temp2 + "</span><span style=\" font-size:12pt; font-weight:600;"
            "color:#707070;\"> </span></p></body></html>";

    textBody->setHtml(html);
}

void Media::load(int index)
{
    _selected = index;

    if(_data[index]->isMedia())
    {
        _countClient = _data[index]->countPeople();
        _media = _data[index]->media();
    }

    cbTable->setCurrentIndex(index+1);
    cbTable->setEnabled(false);
    DSBNumberPeople->setValue(_data[index]->countPeople());

    setCountClient(_data[index]->countPeople());
}

void Media::onClick_pbSave()
{
    if(YNSaveInOrdered->currentState())
    {
        _data[_selected]->setMedia(_countClient);
        emit updateData(3,true);
    }
    else
    {
        if (QMessageBox::question(this, tr("Visualizar média"),tr("Visualizar média no pedido?"), tr("Sim"), tr("Não")) == 0)
        {
            _data[_selected]->setMedia(_countClient);
            emit updateData(3,true);
        }
    }

    onClick_pbCancel();
}
