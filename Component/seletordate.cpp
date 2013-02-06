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


 #include "seletordate.h"
#include <QDebug>

SeletorDate::SeletorDate(QWidget *parent) :
    QWidget(parent)
{
    createComponent();
    input();
    connections();
}

void SeletorDate::adjustLayout()
{
    pbToday->setGeometry(0,0,this->width()/5,this->height());
    pbWeek->setGeometry(pbToday->x() + pbToday->width(),0,this->width()/5,this->height());
    pbMonth->setGeometry(pbWeek->x() + pbWeek->width(),0,this->width()/5,this->height());
    pbYear->setGeometry(pbMonth->x() + pbMonth->width(),0,this->width()/5,this->height());
    pbPeriod->setGeometry(pbYear->x() + pbYear->width(),0,this->width()/5,this->height());
}

void SeletorDate::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void SeletorDate::createComponent()
{
    pbToday = new QPushButton(this);
    pbWeek = new QPushButton(this);
    pbMonth = new QPushButton(this);
    pbYear = new QPushButton(this);
    pbPeriod = new QPushButton(this);
}

void SeletorDate::input()
{
#if defined(Q_WS_X11)
    {
        QFont f("Sans Serif", 6);
        this->setFont(f);
    }
#endif

#if defined(Q_WS_WIN)
    {
        QFont f("MS Shell Dlq 2", 9);
        this->setFont(f);
    }
#endif

    pbToday->setText(tr("Hoje"));
    pbToday->setFont(this->font());
    pbToday->setCursor(Qt::PointingHandCursor);

    pbWeek->setText(tr("Semana"));
    pbWeek->setFont(this->font());
    pbWeek->setCursor(Qt::PointingHandCursor);

    pbMonth->setText(tr("Mês"));
    pbMonth->setFont(this->font());
    pbMonth->setCursor(Qt::PointingHandCursor);

    pbYear->setText(tr("Ano"));
    pbYear->setFont(this->font());
    pbYear->setCursor(Qt::PointingHandCursor);

    pbPeriod->setText(tr("Periodo"));
    pbPeriod->setFont(this->font());
    pbPeriod->setCursor(Qt::PointingHandCursor);

    QString style;

    style = "QPushButton"
            "{"
            "background-color: qlineargradient(spread:pad, x1:0.500, y1:0.278136, x2:0.500, y2:1,"
            "stop:1 rgba(237, 240, 245, 255), stop:0 rgba(255, 255, 255, 255));"
            "color: rgba(0,0,0,240);"
            "border-top: 1px solid rgba(0, 0, 0, 30);"
            "border-bottom: 1px solid rgba(0, 0, 0, 50);"
            "border-radius: 0px;"
            "}"
            " "
            "QPushButton:hover, QPushButton:default:hover "
            "{ "
            "border-top: 2px solid rgba(0, 0, 0, 30); "
            "border-bottom: 2px solid rgba(0, 0, 0, 50); "
            "background-color: qlineargradient(spread:pad, x1:0.500, y1:0.278136, x2:0.500, y2:1,"
            "stop:1 rgba(255, 255, 255, 255), stop:0 rgba(237, 240, 245, 255)); "
            " }";



    this->setStyleSheet(style);

    pbToday->setStyleSheet("border-left: 1px solid rgba(0, 0, 0, 50);");

    pbWeek->setStyleSheet("border-left: 1px solid rgba(0, 0, 0, 30);");

    pbMonth->setStyleSheet("border-left: 1px solid rgba(0, 0, 0, 30);");

    pbYear->setStyleSheet("border-left: 1px solid rgba(0, 0, 0, 30);");

    pbPeriod->setStyleSheet("border-right: 1px solid rgba(0, 0, 0, 50);"
                            "border-left: 1px solid rgba(0, 0, 0, 30); ");

    onClick_pbToday();
}

void SeletorDate::connections()
{
    connect(pbToday,SIGNAL(clicked()),SLOT(onClick_pbToday()));

    connect(pbWeek,SIGNAL(clicked()),SLOT(onClick_pbWeek()));

    connect(pbMonth,SIGNAL(clicked()),SLOT(onClick_pbMonth()));

    connect(pbYear,SIGNAL(clicked()),SLOT(onClick_pbYear()));

    connect(pbPeriod,SIGNAL(clicked()),SLOT(onClick_pbPeriod()));
}

void SeletorDate::onClick_pbToday()
{

    QString temp = redStyle + "border-left 1px solid rgba(0, 0, 0, 50);";

    pbToday->setStyleSheet(temp);

    pbWeek->setStyleSheet(this->styleSheet());
    pbMonth->setStyleSheet(this->styleSheet());
    pbYear->setStyleSheet(this->styleSheet());
    pbPeriod->setStyleSheet(this->styleSheet());

    emit selected(0);
}

void SeletorDate::onClick_pbWeek()
{
    QString temp = redStyle + "border-left 1px solid rgba(0, 0, 0, 30);";

    pbWeek->setStyleSheet(temp);

    pbToday->setStyleSheet(this->styleSheet());
    pbMonth->setStyleSheet(this->styleSheet());
    pbYear->setStyleSheet(this->styleSheet());
    pbPeriod->setStyleSheet(this->styleSheet());
    emit selected(1);
}

void SeletorDate::onClick_pbMonth()
{
    QString temp = redStyle + "border-left 1px solid rgba(0, 0, 0, 30);";

    pbMonth->setStyleSheet(temp);

    pbToday->setStyleSheet(this->styleSheet());
    pbWeek->setStyleSheet(this->styleSheet());
    pbYear->setStyleSheet(this->styleSheet());
    pbPeriod->setStyleSheet(this->styleSheet());

    emit selected(2);
}

void SeletorDate::onClick_pbYear()
{
    QString temp = redStyle + "border-left 1px solid rgba(0, 0, 0, 30);";

    pbYear->setStyleSheet(temp);

    pbToday->setStyleSheet(this->styleSheet());
    pbWeek->setStyleSheet(this->styleSheet());
    pbMonth->setStyleSheet(this->styleSheet());
    pbPeriod->setStyleSheet(this->styleSheet());

    emit selected(3);
}

void SeletorDate::onClick_pbPeriod()
{
    QString temp = redStyle +"border-right 1px solid rgba(0, 0, 0, 50);"
            "border-right 1px solid rgba(0, 0, 0, 30);";

    pbPeriod->setStyleSheet(temp);

    pbToday->setStyleSheet(this->styleSheet());
    pbWeek->setStyleSheet(this->styleSheet());
    pbMonth->setStyleSheet(this->styleSheet());
    pbYear->setStyleSheet(this->styleSheet());

    emit selected(4);
}

void SeletorDate::setColor(QRgb color)
{

}

void SeletorDate::setColorAlternate(QRgb color)
{
}
