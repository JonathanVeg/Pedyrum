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


 #include "jointable.h"
#include <QList>
#include <QMessageBox>

JoinTable::JoinTable(QWidget *parent) :
    QWidget(parent)
{
    createComponent();
    connections();
    input();
}

void JoinTable::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_F2){
        onClick_pbSave();
    }
}

void JoinTable::createComponent()
{
    for (int i = 0; i < 100; ++i) {
        _data[i] = new DataItem(this);
    }

    SBTable = new SpinBox(this);

    pbAdd = new QPushButton(tr("Adicionar"),this);

    tables = new TableView(this);

    pbRemove = new QPushButton(tr("Remover"),this);

    pbBack = new QPushButton(tr("Voltar"),this);
    pbSave = new QPushButton(tr("Salvar"),this);

    QList<QString> list;

    list << "Mesas";

    model = new DataModel(this,list);

    lTextMesa = new QLabel(this);
}

void JoinTable::input()
{
    SBTable->setPrefix(tr("Mesa: "));
    SBTable->setMaximum(99);
    SBTable->setMinimum(0);

    tables->setModel(model);

    QFont f;
    f.setPointSize(14);
    f.setBold(true);
    lTextMesa->setStyleSheet("color: rgba(255,255,255,200);"
                             "background-color: green;"
                             "border-radius:5px;");
    lTextMesa->setAlignment(Qt::AlignCenter);
    lTextMesa->setFont(f);
}

void JoinTable::connections()
{    
    connect(SBTable, SIGNAL(returnPressed()), SLOT(onClick_pbAdd()));

    connect(pbAdd,SIGNAL(clicked()),SLOT(onClick_pbAdd()));

    connect(pbRemove,SIGNAL(clicked()),SLOT(onClick_pbRemove()));

    connect(pbBack,SIGNAL(clicked()),SLOT(onClick_pbBack()));

    connect(pbSave,SIGNAL(clicked()),SLOT(onClick_pbSave()));
}

void JoinTable::onClick_pbAdd()
{
//    if(SBTable->value() == _selected)
//    {
//        QMessageBox::warning(this, tr("Operação inválida"), tr("Por favor, selecione uma mesa diferente"), QMessageBox::Ok);
//        return;
//    }

    if(verification(SBTable->value()))
    {
        QMessageBox::warning(this, tr("Operação inválida"), tr("Essa mesa já foi unida, em outro grupo"), QMessageBox::Ok);
        return;
    }

    if(addingDouble(SBTable->value()))
    {
        QMessageBox::warning(this, tr("Operação inválida"), tr("Essa mesa já foi adicionada."), QMessageBox::Ok);
        return;
    }

    if(_data[SBTable->value()]->isFather() || _data[SBTable->value()]->isChildren())
    {
        QMessageBox::warning(this, tr("Operação inválida"), tr("Essa mesa fez uma transferência."), QMessageBox::Ok);
        return;
    }

    tempList << SBTable->value();
    model->clear();

    QString temp;

    for (int i = 0; i < tempList.count(); ++i) {
        temp.setNum(tempList.value(i));
        model->addText(temp);        
    }
}

void JoinTable::onClick_pbRemove()
{
    if(!tables->currentIndex().isValid())
        return;

    _data[tempList.value(tables->currentIndex().row())]->clearCompound();

    tempList.removeAt(tables->currentIndex().row());
    model->clear();

    QString temp;

    for (int i = 0; i < tempList.count(); ++i) {
        temp.setNum(tempList.value(i));
        model->addText(temp);
    }
}

void JoinTable::onClick_pbBack()
{
    model->clear();
    tempList.clear();
    SBTable->clear();

    emit back();
}

void JoinTable::onClick_pbSave()
{
    // verifica se o próprio item ta na lista
    for (usint i = 0; i < tempList.length(); i++){
        if (tempList[i] == selected()){
            tempList.removeAt(i);
            break;
        }
    }

    if(!tempList.isEmpty())
    {
        tempList << _selected;

        QList<int> temp;

        for (int i = 0; i < tempList.count(); ++i)
        {
            for (int n = 0; n < tempList.count(); ++n)
            {
                if(tempList.value(i)!= tempList.value(n))
                    temp << tempList.value(n);
            }

            _data[tempList.value(i)]->setCompound(temp);

            temp.clear();
        }
    }
    else
        _data[_selected]->clearCompound();

    emit updateData(1,true);

    onClick_pbBack();
}

void JoinTable::adjustLayout()
{
    SBTable->setGeometry(20,20,this->width()- 200,28);

    pbAdd->setGeometry(SBTable->x() + SBTable->width() + 30,19,100,30);

    tables->setGeometry(SBTable->y(),60,SBTable->width(),this->height()-80);
    tables->setColumnWidth(0,tables->width() -1);

    pbRemove->setGeometry(pbAdd->x(),120,100,30);
    pbSave->setGeometry(pbAdd->x(),pbRemove->y() + 60,100,30);
    pbBack->setGeometry(pbAdd->x(),pbSave->y() + 60,100,30);
    lTextMesa->setGeometry(this->width()-154,this->height()-50,120,24);
}

void JoinTable::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void JoinTable::load(int index)
{
    _selected = index;

    QString temp;
    temp.setNum(_selected);

    lTextMesa->setText(temp);

    if(!_data[_selected]->isCompound())
        return;
       else
    {
        for (int i = 0; i < _data[_selected]->listCompound().count(); ++i) {
            temp.setNum(_data[_selected]->listCompound().value(i));
            model->addText(temp);
            tempList << temp.toInt();
        }
    }
}

bool JoinTable::verification(int newtable)
{
    return _data[newtable]->isCompound();
}

bool JoinTable::addingDouble(int newtable)
{
    for (int i = 0; i < tempList.count(); ++i) {
        if(newtable == tempList.value(i))
            return true;
    }

    return false;
}
