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


 #ifndef ITEMDESK_H
#define ITEMDESK_H

#include <QWidget>
#include <Component/label.h>
#include <Interface/Table/dataitem.h>
#include <QList>

typedef unsigned short int usint;

class ItemDesk : public QWidget
{
    Q_OBJECT
public:

    enum states{
        COMPOUND = 0,
        FATHER = 1,
        CHILDREM = 2,
        FREE = 3,
        MEDIA = 4
    };

    explicit ItemDesk(QWidget *parent = 0, QString name = tr("Mesa"), int number = 0);

    void createComponent();

    void connections();

    void adjustLayout();

    void resizeEvent(QResizeEvent *);

    void input();

    void setNumber(int number);

    void setFree(bool b);

    void setCompoundColor(bool b);

signals:        
    void onClick(int index);
    void onDoubleClicked(int index);
    void groupVisible(bool b,QList<int> list);

private slots:
    void onClickItem();
    void onDoubleClickItem();
    void onMouseFocus(bool b);

public slots:    
    void setFather(bool b);
    void setChildren(bool b);
    void setCompound(bool b);
    void setHover(bool b);

    states isType();

public:

    DataItem *data;

    short int _id;

    states getState();

public:
    Label *lBackground;
    Label *lFather;
    Label *lCompoud;
    Label *lChild;
    Label *lNumber;

};

#endif // ITEMDESK_H
