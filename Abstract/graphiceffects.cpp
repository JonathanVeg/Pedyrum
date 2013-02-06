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


 #include "graphiceffects.h"
#include <QRect>
#include <QDebug>

GraphicEffects::GraphicEffects(QObject *parent) :
    QObject(parent)
{

}

QPropertyAnimation * GraphicEffects::createAnimation(QWidget *widget, QRect startGeometry, QRect endGeometry, int time, QEasingCurve easingCurve)
{
    QPropertyAnimation *animation = new QPropertyAnimation(widget,"geometry");

    animation->setDuration(time);

    animation->setEasingCurve(easingCurve);

    animation->setStartValue(QRect(startGeometry));

    animation->setEndValue(QRect(endGeometry));

    return animation;
}

QPropertyAnimation *GraphicEffects::createAnimation(QWidget *widget, QRect endGeometry, int time, QEasingCurve easingCurve)
{
    QPropertyAnimation *animation = new QPropertyAnimation(widget,"geometry");

    animation->setDuration(time);

    animation->setEasingCurve(easingCurve);

    animation->setStartValue(QRect(widget->geometry()));

    animation->setEndValue(QRect(endGeometry));

    return animation;
}

void GraphicEffects::setGroupAnimation(QPropertyAnimation *animation1, QPropertyAnimation *animation2)
{
    groupAnimation = new QSequentialAnimationGroup();

    groupAnimation->addAnimation(animation1);

    groupAnimation->addAnimation(animation2);

    groupAnimation->start();
}

void GraphicEffects::setAnimation(QWidget *widget, QRect startGeometry, QRect endGeometry, int time,QEasingCurve easingCurve)
{
    simpleAnimation = new QPropertyAnimation(widget,"geometry");

    simpleAnimation->setDuration(time);

    simpleAnimation->setEasingCurve(easingCurve);

    simpleAnimation->setStartValue(QRect(startGeometry));

    simpleAnimation->setEndValue(QRect(endGeometry));

    simpleAnimation->start();

    emit startAnimation();
}
