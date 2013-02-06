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


 #ifndef MEDIA_H
#define MEDIA_H

#include <QWidget>
#include <QTextBrowser>
#include <Component/yesno.h>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <Abstract/tools.h>
#include <Interface/Table/dataitem.h>

class Media : public QWidget
{
    Q_OBJECT

public:
    explicit Media(QWidget *parent = 0);

    void createComponent();

    void input();

    void resizeEvent(QResizeEvent *);

    void connections();

signals:
    void back();
    void updateData(int index, bool expor);

public slots:

    void setCountClient(int index);
    void load(int index);

private slots:
    void onClick_pbSave();
    void onClick_pbCancel();

private:
    short int _countClient;
    double _media;

    Tools t;

public:

    DataItem *_data[100];
    short int _selected;

    QTextBrowser *textBody;    
    QLabel *lTextSaveInOrdered;
    YesNo *YNSaveInOrdered;
    QSpinBox *DSBNumberPeople;
    QComboBox *cbTable;
    QPushButton *pbSave;
    QPushButton *pbCancel;

};

#endif // MEDIA_H
