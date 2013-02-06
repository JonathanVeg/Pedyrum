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


 #include "item.h"
#include <QDebug>

Item::Item(QWidget *parent, QString urlImage, QString text):
    QWidget(parent)
{
    createComponent();

    setImage(urlImage);

    setText(text);

    setStyle("background-color: rgba(255,255,255,20);");

    lBody->setAlignment(Qt::AlignVCenter);

    setStandard("background-color: rgba(255,255,255,10);");

    connect(space,SIGNAL(onClick()),SLOT(onClick_space()));

    connect(space,SIGNAL(mouseOnImage(bool)),SLOT(showHover(bool)));
}

void Item::createComponent()
{
    image = new QLabel(this);

    lBody = new QLabel(this);

    space = new Label(this,true,100);
}

void Item::setImage(QString urlImage)
{
    image->setPixmap(QPixmap(urlImage));
    image->setScaledContents(true);
}

void Item::setText(QString text)
{
    lBody->setText(text);
}

void Item::setFont(QFont f)
{
    lBody->setFont(f);
}

void Item::adjustLayout()
{
    image->setGeometry(this->height() * 0.2,this->height() * 0.1,
                       this->height() * 0.8, this->height() * 0.8);

    lBody->setGeometry(this->height() * 0.4 + image->width(), this->height() * 0.1,
                       this->width() - (this->width() * 0.2 + image->width()), this->height() * 0.8);

    space->setGeometry(0,0,this->width(),this->height());
}

void Item::setStyle(QString style)
{
    lBody->setStyleSheet(style);
}

void Item::onClick_space()
{
    emit onClick();
}

void Item::setHover(QString style)
{
    Style = style;
}

void Item::showHover(bool b)
{
    if(b)
       space->setStyleSheet(Style);
    else
       space->setStyleSheet(StyleStandard);
}

void Item::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void Item::setFontColor(QString styleCor)
{
    lBody->setStyleSheet(styleCor);
}

void Item::setStandard(QString style)
{
    StyleStandard = style;
    space->setStyleSheet(StyleStandard);
}
