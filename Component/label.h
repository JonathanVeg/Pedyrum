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


 #ifndef LABELBUTTON_H
#define LABELBUTTON_H

#include <QLabel>
#include <QMouseEvent>
#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QRgb>

class Label : public QLabel
{
    Q_OBJECT
public:    
    explicit Label(QWidget *parent = 0, bool allFunction = false, short int time = 250);
    void setColor(short int r = 0, short int g = 0, short int b = 0, short int a = 255);
    void setColor(QColor rgba);
    QColor Color();
    void setImage(QString url);
    void setHover(QString url);
    void setAlternateImage(QString url);
    void setAlternate(bool b);

    void setHoverStyle(QString style);
    void setStandartStyle(QString style);

signals:
    void onClick();
    void onDoubleClick();
    void mouseOnImage(bool);


protected:
    void mousePressEvent(QMouseEvent *e);

    void mouseDoubleClickEvent(QMouseEvent *e);

public slots:
    void mouseOn();

public:

    QTimer *openFileTimer;
    QPoint point;

    QString imageHover;
    QString imagePressed;
    QString imageStandart;
    QString imageAlternate;

    QString styleHover;
    QString styleStandart;

    bool isOn;
    bool isAlternate;

    QColor color;
    int alpha;

};

#endif // LABELBUTTON_H
