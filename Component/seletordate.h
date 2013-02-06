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


 #ifndef SELETORDATE_H
#define SELETORDATE_H

#include <QWidget>
#include <QPushButton>
#include <QString>

const QString redStyle = "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                         "stop:1 rgba(165, 47, 31, 255), stop:0 rgba(180, 60, 40, 255));"
                         "color: rgb(230,230,230);"
                         "border-top: 1px solid rgba(0, 0, 0, 30);"
                         "border-bottom: 1px solid rgba(0, 0, 0, 50);"
                         "border-radius: 0px;";

class SeletorDate : public QWidget
{
    Q_OBJECT
public:
    explicit SeletorDate(QWidget *parent = 0);

    void createComponent();

    void resizeEvent(QResizeEvent *);

    void connections();

    void input();

    void setColor(QRgb color);

    void setColorAlternate(QRgb color);


signals:
    void selected(int index);

private slots:

    void onClick_pbToday();
    void onClick_pbWeek();
    void onClick_pbMonth();
    void onClick_pbYear();
    void onClick_pbPeriod();

public slots:

    void adjustLayout();

public:

    QPushButton *pbToday;
    QPushButton *pbWeek;
    QPushButton *pbMonth;
    QPushButton *pbYear;
    QPushButton *pbPeriod;

private:



};

#endif // SELETORDATE_H
