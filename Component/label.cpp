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


 #include "label.h"
#include <QDebug>

Label::Label(QWidget *parent, bool allFunction, short int time) :
    QLabel(parent)
{
    if(allFunction == true)
    {
        openFileTimer = new QTimer(this);
        openFileTimer->setInterval(time);
        openFileTimer->start();

        connect(openFileTimer,SIGNAL(timeout()),this,SLOT(mouseOn()));

        imageHover = "";
        imagePressed = "";
        imageStandart = "";
        styleHover = "";
        styleStandart = "";
        isOn = false;
        isAlternate = false;
    }

    this->setScaledContents(true);
    this->setCursor(Qt::PointingHandCursor);

}

void Label::mousePressEvent(QMouseEvent *e)
{
    if(imagePressed != "")
        this->setPixmap(QPixmap(imagePressed));

    emit onClick();
}

void Label::mouseDoubleClickEvent(QMouseEvent *e)
{
    emit onDoubleClick();
}

void Label::mouseOn()
{
    point = this->mapFromGlobal(QCursor::pos());

    if(point.x() >= 0 && point.x() <= this->width()
            && point.y() >= 0 && point.y() <= this->height())
    {
        if(this->text().isEmpty())
        {
            if((imageHover != "") && (!isAlternate))
                this->setPixmap(QPixmap(imageHover));
        }
        else
        {
            if(styleHover != "")
               this->setStyleSheet(styleHover);
        }

        if(!isOn)
        {
            isOn = true;
            emit(mouseOnImage(isOn));
        }
    }
    else
    {
        if(this->text().isEmpty())
        {
            if((imageStandart != "") && (!isAlternate))
                this->setPixmap(QPixmap(imageStandart));
            else
                this->setPixmap(QPixmap(imageAlternate));
        }
        else
        {
            if(styleStandart != "")
               this->setStyleSheet(styleStandart);
        }

        if(isOn)
        {
            isOn = false;
            emit(mouseOnImage(isOn));
        }
    }
}

void Label::setColor(short r, short g, short b, short a)
{
    QPalette p;

    QColor c;

    c.setRgb(r,g,b,a);

    p.setColor(QPalette::WindowText,c);

    this->setPalette(p);
}

void Label::setColor(QColor rgba)
{
    QPalette p;

    QColor c = rgba;

    p.setColor(QPalette::WindowText,c);

    this->setPalette(p);
}

void Label::setImage(QString url)
{
    imageStandart = url;
    this->setPixmap(QPixmap(url));
}

void Label::setHover(QString url)
{
    imageHover = url;
}

void Label::setAlternateImage(QString url)
{
    imageAlternate = url;
}

void Label::setAlternate(bool b)
{
    if(b)
        this->setPixmap(QPixmap(imageAlternate));
    else
        this->setPixmap(QPixmap(imageStandart));

    isAlternate = b;
}

QColor Label::Color()
{
    return color;
}

void Label::setHoverStyle(QString style)
{
    styleHover = style;
}

void Label::setStandartStyle(QString style)
{
    styleStandart = style;
}

