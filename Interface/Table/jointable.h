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


 #ifndef JOINTABLE_H
#define JOINTABLE_H

#include <QWidget>
#include <Component/tableview.h>
#include <Component/spinbox.h>
#include <QSpinBox>
#include <Abstract/datamodel.h>
#include <QPushButton>
#include <QList>
#include <QKeyEvent>
#include <QComboBox>
#include <QMap>
#include <Interface/Table/dataitem.h>
#include <QLabel>

typedef unsigned short int usint;

class JoinTable : public QWidget
{
    Q_OBJECT
public:
    explicit JoinTable(QWidget *parent = 0);

    void createComponent();

    void input();

    void connections();

    void adjustLayout();

    void resizeEvent(QResizeEvent *);

signals:
    void back();    
    void updateData(int index, bool expor);

public slots:    

    void keyPressEvent(QKeyEvent *event);

    void onClick_pbAdd();

    void onClick_pbRemove();

    void onClick_pbBack();

    void onClick_pbSave();

    void load(int index);

    bool verification(int newtable);

    bool addingDouble(int newtable);

private:
    usint selected() const {return _selected;}

public:

    DataItem *_data[100];
    usint _selected;

    SpinBox *SBTable;
    QPushButton *pbAdd;

    TableView *tables;
    QPushButton *pbRemove;

    DataModel *model;
    QList<int> tempList;

    QPushButton *pbBack;
    QPushButton *pbSave;

    QLabel *lTextMesa;
};

#endif // JOINTABLE_H
