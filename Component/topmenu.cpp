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


 #include "topmenu.h"
#include <QDebug>
#include <QFile>

const char URL_FULLSCREEN_OFF[50] = ":/topMenu/fullscreen-off";

const char URL_FULLSCREEN_ON[50] = ":/topMenu/fullscreen-on";

topMenu::topMenu(QWidget *parent) :
    QWidget(parent)
{

    createComponent();
    setComponent();

    connect(menuButton,SIGNAL(mouseOnImage(bool)),this,SLOT(onFocus_menuButton(bool)));
    connect(fullScreen,SIGNAL(onClick()),this,SLOT(onClick_fullScreen()));
    connect(nickName,SIGNAL(onClick()),SLOT(onClick_nickName()));
}

void topMenu::goResize(int width_, int height_)
{
    Q_UNUSED(height_);    
    this->resize(width_,const_Height);
}

void topMenu::createComponent()
{
    background = new QLabel(this);
    menuButton = new Label(this,true,250);
    fullScreen = new Label(this);
    nickName = new Label(this,true,100);
    trayIco = new Label(this);
}

void topMenu::setComponent()
{
    visible = true;
    activateFocus = true;
    isOnMenu = true;

    nickNameText = "";

    background->setPixmap(QPixmap(url_MenuSuperior));
    background->setScaledContents(true);   

    fullScreen->setImage(URL_FULLSCREEN_OFF);
    fullScreen->setAlternateImage(URL_FULLSCREEN_ON);
    fullScreen->setAlternate(false);

    fullScreen->setToolTip(tr("Tela Cheia"));

    menuButton->setImage(":/topMenu/menuButton");
    menuButton->setHover(":/topMenu/menuButton-hover");
    menuButton->setToolTip(tr("Menu"));

    trayIco->setToolTip(tr("Minimizar para bandeja."));
    trayIco->setImage(":/topMenu/trayIco");

    nickName->setAlignment(Qt::AlignHCenter);

    QFont f;
    f.setPointSize(8);
    f.setBold(true);
    nickName->setFont(f);

    nickName->setStyleSheet("color: rgba(255,255,255,230);");
}

void topMenu::resizeEvent(QResizeEvent *)
{
    ajustLayout();
}

int topMenu::sizeNickName(QString name)
{
    QChar lyrics;
    int ASCII = 0;
    int space = 0;

    space = 20;

    for (int var = 0; var < name.length(); ++var)
    {
        lyrics = name.at(var).toAscii();
        ASCII = lyrics.toAscii();

        if(ASCII >= 65 && ASCII <= 90)
            space += 9;
        else
            space += 7;

        space--;
    }
    return  space;
}

QString topMenu::firstName(QString name)
{
    for (int num = 0; num < name.length(); ++num) {
        if(name.mid(num,1)  == " ")
        {
            return name.mid(0,num - 1);
        }
    }

    return name;
}

void topMenu::ajustLayout()
{
    fullScreen->setGeometry(this->width() - 24,this->height()/2  - 8,16,16);

    int sizeName = 0;

    if(nickNameText != NULL)
    {
        sizeName = sizeNickName(nickName->text());

        nickName->setGeometry(this->width() - (sizeName + 20),8,sizeName - 16 ,16);
    }

    this->setGeometry(0,0,this->width(),const_Height);

    background->setGeometry(0,0,this->width(),this->height());

    trayIco->setGeometry(this->width() - (sizeName + 60),6,14,14);

    menuButton->setGeometry(0,0,60,25);    
}

void topMenu::onFocus_menuButton(bool b)
{
    if(b)
    {
        if(activateFocus)
        {
            activateFocus = false;

            if(visible)
            {
                visible = false;

                emit sideVisible(visible);                
            }
            else
            {
                visible = true;

                emit sideVisible(visible);                
            }
        }
    }
    else
    {
        activateFocus = true;
    }
}

void topMenu::setNickName(QString text)
{
    nickNameText = text;

    nickName->setText(nickNameText);

    ajustLayout();
}


void topMenu::onClick_fullScreen()
{
    bool b = fullScreen->isAlternate;

    if(b)
    {
        fullScreen->setAlternate(false);
        emit FullScreen(false);
    }
    else
    {
        fullScreen->setAlternate(true);
        emit FullScreen(true);
    }
}

void topMenu::onClick_nickName()
{
    emit userClick();
}


