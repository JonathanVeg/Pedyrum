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


 #include "Component/search.h"
#include <QFont>
#include <QDebug>

Search::Search(QWidget *parent, QString text, bool single):
    QWidget(parent)
{
    textStandard = text;

    isSingle = single;

    createCompleter();
    input();
    connections();

    eSearch->setToolTip("");
}

void Search::createCompleter()
{
    completer = new QCompleter();
    eSearch = new Edit(this,true,textStandard);
    tbMenu = new QToolButton(this);
}

void Search::input()
{
    eSearch->setStyleSheet("border-radius: 8px;"
                           "border: 1px solid rgba(0,0,0,50);");

    eSearch->setStyleSheet("padding: 1px 12px 1px 30px;"
                           "padding-left: 30px;"
                           "color: rgba(0,0,0,150);");

    QFont f;

#if defined(Q_WS_X11)
    f.setPointSize(7);
#endif

#if defined(Q_WS_WIN)
    f.setPointSize(10);
#endif

    f.setItalic(true);

    eSearch->setFont(f);

    if(!isSingle)
    {
        tbMenu->setPopupMode(QToolButton::MenuButtonPopup);
        tbMenu->setIconSize(QSize(22,22));
    }
    else
        tbMenu->setIconSize(QSize(14,14));


    tbMenu->setAutoRaise(true);
    tbMenu->setIcon(QIcon(URL_MENU));

}

void Search::connections()
{
    connect(eSearch,SIGNAL(textChanged(QString)),SLOT(textChanged(QString)));
}

void Search::layout()
{
    tbMenu->setGeometry(0,0,this->height() + (this->height() / 6),this->height());
    eSearch->setGeometry(0,0,this->width(),this->height());
}

int Search::column()
{
    return completer->completionColumn();
}

void Search::setColumn(int column)
{
    completer->setCompletionColumn(column);
}

void Search::setCompleter(QCompleter *autoCompleter)
{
    completer = autoCompleter;
    eSearch->setCompleter(completer);
}

void Search::setAction(QAction *action)
{
    tbMenu->addAction(action);
}

void Search::resizeEvent(QResizeEvent *)
{
    layout();
}

void Search::textChanged(QString text)
{
    if(text != textStandard)
    {
        eSearch->setStyleSheet("padding: 1px 12px 1px 30px;"
                               "padding-left: 30px;"
                               "color: rgba(0,0,0,255);");
        emit textSearch(text);
    }
    else
    {
        eSearch->setStyleSheet("padding: 1px 12px 1px 30px;"
                               "padding-left: 30px;"
                               "color: rgba(0,0,0,150);");
    }
}
