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


 #ifndef FISTADMIN_H
#define FISTADMIN_H

#include <QWidget>
#include <QLabel>
#include <Component/edit.h>
#include <QPushButton>
#include <QToolButton>
#include <Abstract/bdtools.h>
#include <Abstract/encryption.h>

class FirstAdminInterface : public QWidget
{
    Q_OBJECT
public:
    explicit FirstAdminInterface(QWidget *parent = 0);

    void createComponent();

    void resizeEvent(QResizeEvent *);

    void connections();

signals:
     void gotoNext(int index);

public slots:

    void input();

    void adjustLayout();

    void textChanged_ePassword();

    void textChanged_ePasswordConfirm();

    void onClick_pbCreate();

    void onClick_pbLeave();

public:

    QLabel *lText;

    QWidget *wGroup;
    QLabel *lBackground;

    Edit *eUsername;
    Edit *ePassword;
    Edit *ePasswordConfirm;

    Edit *eSecretAsk;
    Edit *eAnswer;

    QPushButton *pbCreate; // testar a conexão
    QPushButton *pbLeave; // conectar

    BDTools bd;

    Encryption e;

};

#endif // FISTADMIN_H
