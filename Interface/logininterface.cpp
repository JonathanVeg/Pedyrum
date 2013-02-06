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


 #include "logininterface.h"
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

LoginInterface::LoginInterface(QWidget *parent) :
    QWidget(parent)
{
    createComponent();    
    connections();        

}

void LoginInterface::createComponent()
{
    login = new Login();

    wLogin = new QWidget(this);
    {
        lLogin = new QLabel(wLogin);
        lLoginText = new QLabel(wLogin);
        eUsername = new Edit(wLogin,true,tr("Nome"));
        ePassword = new Edit(wLogin,false,tr("Senha"));
        tbViewPassword = new QToolButton(wLogin);
        aShowPassword = new QAction(tr("Exibir senha"),this);
        cbShowAskSecret = new QCheckBox(wLogin);
        lTextTitle = new QLabel(wLogin);
        lTextAsk = new QLabel(wLogin);
        eAnswer = new Edit(wLogin,true,tr("Resposta"));
        pbLogin = new QPushButton(wLogin);
    }
}

void LoginInterface::input()
{
    lLogin->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.498,"
                          "y1:0, x2:0.494636, y2:1, stop:0 rgba(255, 255, 255, 210),"
                          "stop:0.982955 rgba(255, 255, 255, 170));"
                          "border-radius: 15px;border: 1px solid rgba(0,0,0,25);");

    tbViewPassword->setAutoRaise(true);
    tbViewPassword->setPopupMode(QToolButton::MenuButtonPopup);
    tbViewPassword->setIconSize(QSize(20,20));
    tbViewPassword->setIcon(QIcon(":/Extras/question"));
    tbViewPassword->addAction(aShowPassword);
    tbViewPassword->setStyleSheet("");

    aShowPassword->setChecked(false);
    aShowPassword->setCheckable(true);

    QString cssBlack = "color: rgba(0,0,0,150);";

    QFont f;

#if defined(Q_WS_X11)
    {
        f.setPointSize(8);
        f.setFamily("Sans Serif");
    }
#endif

#if defined(Q_WS_WIN)
    {
        f.setPointSize(10);
        f.setFamily("MS Shell Dlq 2");
    }
#endif

    cbShowAskSecret->setText(tr("Esqueci minha senha."));
    cbShowAskSecret->setFont(f);
    cbShowAskSecret->setStyleSheet(cssBlack);

    int size = 0;

    size =  (f.pointSize() + 9);

    f.setPointSize(size);

    lLoginText->setFont(f);
    lLoginText->setText(tr("Login"));
    lLoginText->setStyleSheet(cssBlack);
    lLoginText->setAlignment(Qt::AlignCenter);

    size = size - 10;

    f.setPointSize(size);

    lTextTitle->setFont(f);
    lTextTitle->setText(tr("Pergunta secreta"));
    lTextTitle->setAlignment(Qt::AlignHCenter);
    lTextTitle->setStyleSheet(cssBlack);

    lTextAsk->setFont(f);
    lTextAsk->setAlignment(Qt::AlignHCenter);
    lTextAsk->setStyleSheet(cssBlack);

    pbLogin->setText(tr("Login"));
    pbLogin->setCursor(Qt::PointingHandCursor);

    eUsername->setNextComponent(ePassword);
}

void LoginInterface::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void LoginInterface::adjustLayout()
{
    short int y_ = this->height()/2 - 145;

    wLogin->setGeometry(this->width()/2 - 150,y_,300,290);

    lLogin->setGeometry(0,0,wLogin->width(), wLogin->height());

    lLoginText->setGeometry(100,30,100,40);

    eUsername->setGeometry(25,105,250,28);

    ePassword->setGeometry(25,eUsername->y() + eUsername->height() + 20,250,28);

    tbViewPassword->setGeometry(ePassword->x() + ePassword->width() - 30,ePassword->y(),30,28);

    cbShowAskSecret->setGeometry(25,ePassword->y() + ePassword->height() + 15,250,20);

    if(!cbShowAskSecret->isChecked())
    {
        lTextTitle->setGeometry(0,0,0,0);

        lTextAsk->setGeometry(0,0,0,0);

        eAnswer->setGeometry(0,cbShowAskSecret->y() + cbShowAskSecret->height(),0,0);

        wLogin->setGeometry(this->width()/2 - 150,y_,300,290);
    }
    else
    {
        y_ -= 45;

        lTextAsk->setText(login->secretQuestion(eUsername->text()));

        if (lTextAsk->text() != ""){

            lTextTitle->setGeometry(0,cbShowAskSecret->y() + cbShowAskSecret->height() + 10,300,20);

            lTextAsk->setGeometry(0,lTextTitle->y() + lTextTitle->height() + 10,300,20);

            eAnswer->setGeometry(25,lTextAsk->y() + lTextAsk->height() + 10,250,28);

            wLogin->setGeometry(this->width()/2 - 150,y_,300,380);

            eAnswer->setFocus();
        }
        else
        {
            cbShowAskSecret->setChecked(false);

            eUsername->setFocus();
        }
    }

    lLogin->setGeometry(0,0,wLogin->width(), wLogin->height());

    pbLogin->setGeometry(100,eAnswer->y() + eAnswer->height() + 20,100,33);    
}

void LoginInterface::onClick_pbLogin()
{
    if (cbShowAskSecret->isChecked())
        login->loginWithSecretQuestion(eUsername->value(), eAnswer->value());
    else{
        if (!login->login(eUsername->value(), ePassword->value())){
            eUsername->Clear();
            ePassword->Clear();

            eUsername->setFocus();
        }
    }
}

void LoginInterface::connections()
{
    connect(pbLogin,SIGNAL(clicked()),SLOT(onClick_pbLogin()));

    connect(cbShowAskSecret,SIGNAL(clicked()),SLOT(adjustLayout()));

    connect(ePassword, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged_ePassword()));

    connect(aShowPassword,SIGNAL(triggered()),this,SLOT(onTriggered_aShowPassword()));

    connect(login, SIGNAL(logado(QString,QString,QList<bool>)), this, SLOT(logado(QString,QString,QList<bool>)));

    connect(ePassword, SIGNAL(returnPressed()), SLOT(onClick_pbLogin()));

    connect(eAnswer, SIGNAL(returnPressed()), SLOT(onClick_pbLogin()));
}

void LoginInterface::logado(QString username, QString id, QList<bool> permissions){

    eUsername->Clear();

    ePassword->Clear();

    eAnswer->Clear();

    cbShowAskSecret->setChecked(false);

    emit openSystem(username, id, permissions);
}

void LoginInterface::onTextChanged_ePassword()
{
    if(aShowPassword->isChecked() == false && ePassword->echoMode() == Edit::Normal)
        ePassword->setEchoMode(Edit::Password);

    if(ePassword->text() == ePassword->valueStandart)
    {
        if(ePassword->echoMode() == Edit::Password)
            ePassword->setEchoMode(Edit::Normal);
    }
}

void LoginInterface::onTriggered_aShowPassword()
{
    if(!aShowPassword->isChecked())
    {
        ePassword->setEchoMode(Edit::Password);

        aShowPassword->setChecked(false);
    }
    else
    {
        ePassword->setEchoMode(Edit::Normal);

        aShowPassword->setChecked(true);
    }
}

void LoginInterface::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F11)
        emit fullScreen();
}
