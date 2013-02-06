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


 #ifndef LOGININTERFACE_H
#define LOGININTERFACE_H

#include <QWidget>
#include <QLabel>
#include <Component/edit.h>
#include <Abstract/tools.h>
#include <QToolButton>
#include <QCheckBox>
#include <QPushButton>
#include <QAction>
#include <Object/login.h>
#include <QKeyEvent>
#include <QMessageBox>
#include <Abstract/encryption.h>

class LoginInterface : public QWidget
{
    Q_OBJECT
public:
    explicit LoginInterface(QWidget *parent = 0);

    void createComponent();

    void resizeEvent(QResizeEvent *);

    void connections();

signals:    
    void openSystem(QString name, QString id, QList<bool> permissions);

    void fullScreen();

public slots:

    void keyPressEvent(QKeyEvent *event);

    void input();

    void adjustLayout();

    void onClick_pbLogin();

    void onTextChanged_ePassword();

    void onTriggered_aShowPassword();

    void logado(QString username, QString id, QList<bool> permissions);

public:

    QWidget *wLogin;
    QLabel *lLogin;

    QLabel *lLoginText;
    Edit *eUsername;
    Edit *ePassword;
    QToolButton *tbViewPassword;
    QAction *aShowPassword;
    QCheckBox *cbShowAskSecret;
    QLabel *lTextTitle;
    QLabel *lTextAsk;
    Edit *eAnswer;
    QPushButton *pbLogin;
    Login *login;  

    Tools tools;
};

#endif // LOGININTERFACE_H
