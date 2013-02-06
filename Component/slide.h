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


 #ifndef SLIDE_H
#define SLIDE_H

#include <QWidget>
#include <Component/label.h>
#include <Abstract/graphiceffects.h>
#include <QTimer>
#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QDesktopServices>
#include <QUrl>
#include <QPoint>

const char RIGHT_ARROW[30] = ":/Extras/Slide-arrow-right";
const char LEFT_ARROW[30] = ":/Extras/Slide-arrow-left";

const char ITEM[30] = ":/Extras/point";
const char ITEM_HOVER[30] = ":/Extras/point-hover";
const char CURRENT_ITEM[30] = ":/Extras/point-alternate";

const char URL_SLIDE0[30] = "";
const char URL_SLIDE1[30] = "";
const char URL_SLIDE2[30] = "";
const char URL_SLIDE3[30] = "";
const char URL_SLIDE4[30] = "";

class Slide : public QWidget
{
    Q_OBJECT

public:

    explicit Slide(QWidget *parent = 0);

    void createComponent();

    void connections();

    void setDelay(short int time);

    void setSlide(short int index, QString url);

    void resizeEvent(QResizeEvent *);

    int currentIndex();

    void startTransicao();

    void selectButton(short int index);

    int countPage();

signals:

    void onNow(bool b);

public slots:

    void input();

    void adjustLayout();

    void mouseOn();

    void setCurrentIndex(short int index);

    void nextSlide();

    void NextSlide(short int index);

    void setCurrentPage(short int current);

    void onClick_lRightArrow();

    void onClick_lLefttArrow();

    void onClick_lB0();
    void onClick_lB1();
    void onClick_lB2();
    void onClick_lB3();
    void onClick_lB4();

    void onClick_slide0();
    void onClick_slide1();
    void onClick_slide2();
    void onClick_slide3();
    void onClick_slide4();

    void mouseOn_slide(bool b);

public:

    QTimer *Delay;
    QTimer *timerPoint;

    Label *lRightArrow;
    Label *lLefttArrow;

    Label *lB0;
    Label *lB1;
    Label *lB2;
    Label *lB3;
    Label *lB4;

    Label *Slide0;
    Label *Slide1;
    Label *Slide2;
    Label *Slide3;
    Label *Slide4;

    QPropertyAnimation *next;
    QPropertyAnimation *back;

    GraphicEffects *effetcs;

private:

    short int currentIndex_;
    short int countPage_;
    short int countDelay_;
    short int interval_;

    bool isStarted;

    QPoint point;
    bool isOn;




};

#endif // SLIDE_H

