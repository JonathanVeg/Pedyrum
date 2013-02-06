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


 #include "itemdesk.h"
#include <QDebug>

const QString STYLE_RED = "background-color: rgb(165, 47, 31,200);"
"color: rgba(220,220,220,220);"
"border-radius: 5px; border: 1px solid rgba(255,255,255,50);";

const QString STYLE_GREEN = "background-color: green;"
"color: rgba(220,220,220,220);"
"border-radius: 5px; border: 1px solid rgba(255,255,255,50);";

const QString STYLE_ORANGE = "background-color: rgb(255, 170, 0);"
"color: rgba(220,220,220,220);"
"border-radius: 5px; border: 1px solid rgba(255,255,255,50);";

const QString STYLE_HOVER = "background-color: silver;"
"border-radius: 5px;border: 1px solid rgba(255,255,255,70);";

ItemDesk::ItemDesk(QWidget *parent, QString name, int number) :
    QWidget(parent)
{
    createComponent();

    connections();

    input();

    setNumber(number);
}

void ItemDesk::createComponent()
{
    data = new DataItem(this);

    lBackground = new Label(this,true,100);
    lNumber = new Label(this);
    lFather = new Label(this);
    lCompoud = new Label(this);
    lChild = new Label(this);
}

void ItemDesk::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void ItemDesk::connections()
{
    connect(lBackground,SIGNAL(mouseOnImage(bool)),SLOT(onMouseFocus(bool)));

    connect(lBackground,SIGNAL(onClick()),SLOT(onClickItem()));

    connect(lNumber,SIGNAL(onClick()),SLOT(onClickItem()));

    connect(lChild,SIGNAL(onClick()),SLOT(onClickItem()));

    connect(lFather,SIGNAL(onClick()),SLOT(onClickItem()));

    // double clicked
    connect(lBackground,SIGNAL(onDoubleClick()),SLOT(onDoubleClickItem()));

    connect(lNumber,SIGNAL(onDoubleClick()),SLOT(onDoubleClickItem()));

    connect(lChild,SIGNAL(onDoubleClick()),SLOT(onDoubleClickItem()));

    connect(lFather,SIGNAL(onDoubleClick()),SLOT(onDoubleClickItem()));
}

void ItemDesk::adjustLayout()
{
    lBackground->setGeometry(0,0,this->width(), this->height());

    lNumber->setGeometry(0,0,this->width(),this->height());

    lFather->setGeometry(this->width()/2 - lBackground->width() * 0.25,0,
                         lBackground->width() * 0.5,lBackground->width() * 0.2);

    lCompoud->setGeometry(this->width()/2 - lBackground->width() * 0.2,0,
                          lBackground->width() * 0.4,lBackground->width() * 0.2);

    lChild->setGeometry(lNumber->x() + lNumber->width() - lBackground->width() * 0.15,lNumber->y() + lBackground->width() * 0.05,
                        lBackground->width() * 0.1,lBackground->width() * 0.1);
}

void ItemDesk::input()
{
    QFont f;

    f.setPointSize(9);

    lBackground->setStyleSheet(STYLE_GREEN);

    lFather->setAlignment(Qt::AlignCenter);
    lFather->setFont(f);
    lFather->setStyleSheet("background-color: green;"
                           "border-radius: 5px;border: 0.5px solid rgba(255,255,255,50);"
                           "color: rgba(220,220,220,220);");

    lCompoud->setAlignment(Qt::AlignCenter);
    lCompoud->setFont(f);
    lCompoud->setStyleSheet("background-color: green;"
                            "border-radius: 5px;border: 0.5px solid rgba(255,255,255,50);"
                            "color: rgba(220,220,220,220);");

    lChild->setStyleSheet(lFather->styleSheet());

    lFather->setVisible(false);
    lChild->setVisible(false);
    lCompoud->setVisible(false);

    f.setPointSize(18);

    lNumber->setAlignment(Qt::AlignCenter);
    lNumber->setFont(f);
}

void ItemDesk::setNumber(int number)
{
    _id = number;

    QString temp;

    temp.setNum(_id);

    lNumber->setText(temp);
}

void ItemDesk::onClickItem()
{
    emit onClick(_id);
}

void ItemDesk::onDoubleClickItem(){
    emit onDoubleClicked(_id);
}

void ItemDesk::setFree(bool b)
{
   data->setFree(b);

   if(b)
       lBackground->setStyleSheet(STYLE_GREEN);
    else
       lBackground->setStyleSheet(STYLE_RED);
}

void ItemDesk::onMouseFocus(bool b)
{
    if(data->isCompound())
       emit groupVisible(b,data->listCompound());
}

void ItemDesk::setFather(bool b)
{
    if(b)
    {
        QString temp;
        temp.setNum(data->children());
        lFather->setText(temp);
    }

    lFather->setVisible(b);
}

void ItemDesk::setChildren(bool b)
{
    if(b)
    {
        lChild->setVisible(true);
        lFather->setVisible(false);
    }
    else
    {
        lChild->setVisible(false);
        lFather->setVisible(false);
    }
}

void ItemDesk::setCompound(bool b)
{
    if(b)
        lBackground->setStyleSheet(STYLE_ORANGE);
    else
    {
        if(data->isFree())
            lBackground->setStyleSheet(STYLE_GREEN);
        else
            lBackground->setStyleSheet(STYLE_RED);
    }
}

void ItemDesk::setHover(bool b)
{
    if(b)
        lBackground->setStyleSheet(STYLE_HOVER);
    else
    {
        if(data->isFree())
            lBackground->setStyleSheet(STYLE_GREEN);
        else
        {
            if(data->isCompound())
                lBackground->setStyleSheet(STYLE_ORANGE);
            else
                lBackground->setStyleSheet(STYLE_RED);
        }
    }
}

ItemDesk::states ItemDesk::isType()
{
    if(data->isCompound())
        return COMPOUND;

    if(data->isChildren())
       return CHILDREM;

    if(data->isFather())
        return FATHER;

    if(data->isMedia())
        return MEDIA;

    if(data->isFree())
        return FREE;
}
