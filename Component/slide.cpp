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


 #include "slide.h"
#include <QDebug>

Slide::Slide(QWidget *parent) :
    QWidget(parent)
{
    createComponent();
    connections();
    input();
}

void Slide::setDelay(short segs)
{
    interval_ = segs;
}

void Slide::createComponent()
{
    Delay = new QTimer(this);
    timerPoint = new QTimer(this);

    Slide0 = new Label(this,true,300);
    Slide1 = new Label(this,true,300);
    Slide2 = new Label(this,true,300);
    Slide3 = new Label(this,true,300);
    Slide4 = new Label(this,true,300);

    lRightArrow = new Label(this);
    lLefttArrow = new Label(this);

    lB0 = new Label(this,true,100);
    lB1 = new Label(this,true,100);
    lB2 = new Label(this,true,100);
    lB3 = new Label(this,true,100);
    lB4 = new Label(this,true,100);
}

void Slide::input()
{
    currentIndex_ = 0;    
    countDelay_ = 0;
    countPage_ = 0;
    interval_ = 1;
    isStarted = false;

    Delay->setInterval(1000);

    timerPoint->setInterval(1000);
    timerPoint->start();

    lRightArrow->setToolTip(tr("Voltar"));    
    lRightArrow->setImage(RIGHT_ARROW);
    lRightArrow->hide();

    lLefttArrow->setToolTip(tr("Avançar"));
    lLefttArrow->setImage(LEFT_ARROW);
    lLefttArrow->hide();

    lB0->setImage(ITEM);
    lB0->setHover(ITEM_HOVER);
    lB0->setAlternateImage(CURRENT_ITEM);
    lB0->setAlternate(true);
    lB0->hide();
    lB0->setToolTip(tr("Slide 0"));

    lB1->setImage(ITEM);
    lB1->setHover(ITEM_HOVER);
    lB1->setAlternateImage(CURRENT_ITEM);
    lB1->hide();
    lB1->setToolTip(tr("Slide 1"));

    lB2->setImage(ITEM);
    lB2->setHover(ITEM_HOVER);
    lB2->setAlternateImage(CURRENT_ITEM);
    lB2->hide();
    lB2->setToolTip(tr("Slide 2"));

    lB3->setImage(ITEM);
    lB3->setHover(ITEM_HOVER);
    lB3->setAlternateImage(CURRENT_ITEM);
    lB3->hide();
    lB3->setToolTip(tr("Slide 3"));

    lB4->setImage(ITEM);
    lB4->setHover(ITEM_HOVER);
    lB4->setAlternateImage(CURRENT_ITEM);
    lB4->hide();
    lB4->setToolTip(tr("Slide 4"));
}

void Slide::setSlide(short index, QString url)
{
    if(countPage_ == 5)
    {
        qDebug() << "[SLIDE][setSlide] O número máximo de slide já foi atingindo";
        return;
    }
    else
        countPage_ += 1;

    switch(index)
    {
    case 0:
    {
        Slide0->setImage(url);
        lB0->show(); break;
    }
    case 1:
    {
        Slide1->setImage(url);
        lB1->show(); break;
    }
    case 2:
    {
        Slide2->setImage(url);
        lB2->show(); break;
    }
    case 3:
    {
        Slide3->setImage(url);
        lB3->show(); break;
    }
    case 4:
    {
        Slide4->setImage(url);
        lB4->show(); break;
    }
    }    
}

void Slide::adjustLayout()
{
    lLefttArrow->setGeometry(10,this->height()/2 - 30,30,60);
    lRightArrow->setGeometry(this->width() - 40,this->height()/2 - 30,30,60);

    lB0->setGeometry(this->width()/2 - ((countPage_ * 13) + ((countPage_ -1) * 10))/2,
                     this->height() - 25,13,13);

    lB1->setGeometry(lB0->x() + lB0->width() + 10,lB0->y(),13,13);
    lB2->setGeometry(lB1->x() + lB1->width() + 10,lB1->y(),13,13);
    lB3->setGeometry(lB2->x() + lB2->width() + 10,lB2->y(),13,13);
    lB4->setGeometry(lB3->x() + lB3->width() + 10,lB3->y(),13,13);

    Slide0->setGeometry(0,0,this->width(), this->height());
    Slide1->setGeometry(this->width() * 2,0,this->width(), this->height());
    Slide2->setGeometry(this->width() * 3,0,this->width(), this->height());
    Slide3->setGeometry(this->width() * 4,0,this->width(), this->height());
    Slide4->setGeometry(this->width() * 5,0,this->width(), this->height());
}

void Slide::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void Slide::setCurrentIndex(short index)
{
    currentIndex_ = index;
}

int Slide::currentIndex()
{
    return currentIndex_;
}

void Slide::nextSlide()
{
    countDelay_ += 1;

    if(countDelay_ == interval_)
    {
        if(currentIndex_ == countPage_ -1)
            NextSlide(0);
        else
            NextSlide(currentIndex_ + 1);

        countDelay_ = 1;
    }
}

void Slide::NextSlide(short index)
{
    switch(currentIndex_)
    {
    case 0:back = new QPropertyAnimation(Slide0, "geometry"); break;

    case 1:back = new QPropertyAnimation(Slide1, "geometry"); break;

    case 2:back = new QPropertyAnimation(Slide2, "geometry"); break;

    case 3:back = new QPropertyAnimation(Slide3, "geometry"); break;

    case 4:back = new QPropertyAnimation(Slide4, "geometry"); break;
    }

    switch(index)
    {
    case 0:next = new QPropertyAnimation(Slide0, "geometry"); break;

    case 1:next = new QPropertyAnimation(Slide1, "geometry"); break;

    case 2:next = new QPropertyAnimation(Slide2, "geometry"); break;

    case 3:next = new QPropertyAnimation(Slide3, "geometry"); break;

    case 4:next = new QPropertyAnimation(Slide4, "geometry"); break;
    }

    back->setStartValue(QRect(0,0,this->width(),this->height()));

    back->setEndValue(QRect(this->width() * -1,0,this->width(),this->height()));

    back->setEasingCurve(QEasingCurve::InSine);

    back->setDuration(500);

    next->setStartValue(QRect(this->width() * 2,0,this->width(),this->height()));

    next->setEndValue(QRect(0,0,this->width(),this->height()));

    next->setEasingCurve(QEasingCurve::OutSine);

    next->setDuration(500);

    currentIndex_ = index;
    selectButton(currentIndex_);

    back->start();

    next->start();

}

void Slide::connections()
{
    connect(Delay,SIGNAL(timeout()),SLOT(nextSlide()));

    connect(timerPoint,SIGNAL(timeout()),SLOT(mouseOn()));

    connect(lRightArrow,SIGNAL(onClick()),SLOT(onClick_lRightArrow()));

    connect(lLefttArrow,SIGNAL(onClick()),SLOT(onClick_lLefttArrow()));

    connect(lB0,SIGNAL(onClick()),SLOT(onClick_lB0()));
    connect(lB1,SIGNAL(onClick()),SLOT(onClick_lB1()));
    connect(lB2,SIGNAL(onClick()),SLOT(onClick_lB2()));
    connect(lB3,SIGNAL(onClick()),SLOT(onClick_lB3()));
    connect(lB4,SIGNAL(onClick()),SLOT(onClick_lB4()));

    connect(Slide0,SIGNAL(onClick()),SLOT(onClick_slide0()));
    connect(Slide1,SIGNAL(onClick()),SLOT(onClick_slide1()));
    connect(Slide2,SIGNAL(onClick()),SLOT(onClick_slide2()));
    connect(Slide3,SIGNAL(onClick()),SLOT(onClick_slide3()));
    connect(Slide4,SIGNAL(onClick()),SLOT(onClick_slide4()));

    connect(this,SIGNAL(onNow(bool)),SLOT(mouseOn_slide(bool)));
}

void Slide::onClick_lRightArrow()
{
    if(currentIndex_ == 4)
        NextSlide(0);
    else
        NextSlide(currentIndex_ + 1);

    lRightArrow->show();
}

void Slide::onClick_lLefttArrow()
{
    if(currentIndex_ == 0)
        NextSlide(4);
    else
        NextSlide(currentIndex_ - 1);

    lLefttArrow->show();
}

void Slide::startTransicao()
{
    Delay->start();

    isStarted = true;

}

void Slide::onClick_lB0()
{
    NextSlide(0);
}

void Slide::onClick_lB1()
{
    NextSlide(1);
}

void Slide::onClick_lB2()
{
    NextSlide(2);
}

void Slide::onClick_lB3()
{
    NextSlide(3);
}

void Slide::onClick_lB4()
{
    NextSlide(4);
}

void Slide::selectButton(short index)
{
    lB0->setAlternate(false);
    lB1->setAlternate(false);
    lB2->setAlternate(false);
    lB3->setAlternate(false);
    lB4->setAlternate(false);

    switch(index)
    {
    case 0:lB0->setAlternate(true); break;

    case 1:lB1->setAlternate(true); break;

    case 2:lB2->setAlternate(true); break;

    case 3:lB3->setAlternate(true); break;

    case 4:lB4->setAlternate(true); break;
    }
}

void Slide::setCurrentPage(short current)
{
    currentIndex_ = current;
}

int Slide::countPage()
{
    return countPage_;
}


void Slide::onClick_slide0()
{
    QDesktopServices::openUrl(QUrl(URL_SLIDE0));
}

void Slide::onClick_slide1()
{
    QDesktopServices::openUrl(QUrl(URL_SLIDE1));
}

void Slide::onClick_slide2()
{
    QDesktopServices::openUrl(QUrl(URL_SLIDE2));
}

void Slide::onClick_slide3()
{
    QDesktopServices::openUrl(QUrl(URL_SLIDE3));
}

void Slide::onClick_slide4()
{
    QDesktopServices::openUrl(QUrl(URL_SLIDE4));
}

void Slide::mouseOn_slide(bool b)
{
    if(b)
    {
        lRightArrow->show();
        lLefttArrow->show();
        countDelay_ = 0;

        if(isStarted)
            Delay->stop();

    }
    else
    {
        lRightArrow->hide();
        lLefttArrow->hide();

        if(isStarted)
            Delay->start();
    }

}

void Slide::mouseOn()
{
    point = this->mapFromGlobal(QCursor::pos());

    if(point.x() >= 0 && point.x() <= this->width()
            && point.y() >= 0 && point.y() <= this->height())
    {
        if(!isOn)
        {
            isOn = true;
            emit onNow(isOn);
        }
    }
    else
    {
        if(isOn)
        {
            isOn = false;
            emit onNow(isOn);
        }
    }
}
