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


 #include "phoneedit.h"
#include <QDebug>

phoneEdit::phoneEdit(QWidget *parent, bool usesClear, QString standard, int maximumLetters) :
    Edit(parent)
{
    button = new Label(parent);
    button->hide();

    valueStandart = standard;
    this->setText(valueStandart);
    this->setToolTip(valueStandart);

#if defined(Q_WS_X11)
    {
        QFont f("Sans Serif", 6);
        this->setFont(f);
    }
#endif

#if defined(Q_WS_WIN)
    {
        QFont f("MS Shell Dlq 2", 9);
        this->setFont(f);
    }
#endif

    if(usesClear)
    {
        button->setToolTip(tr("Limpar Campo"));
        button->setCursor(Qt::PointingHandCursor);
        connect(button,SIGNAL(onClick()),this,SLOT(Clear()));
        button->setPixmap(QPixmap(url_clear));
        useClear = true;
    }

    this->setMaxLength(maximumLetters);

    connections();

    setUpperCase(false);

    posN = this->cursorPosition();

    this->installEventFilter(this);

    this->setMaxLength(14);

    del = false;
}

bool phoneEdit::eventFilter(QObject *target, QEvent *event)
{
    if (event->type() == QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if (keyEvent->key() == Qt::Key_Delete || keyEvent->key() == Qt::Key_Backspace)
            del = true;
        else
            del = false;
    }

    // return false, deixa o evento passar
    return false;
}


void phoneEdit::textChanged(QString text){
    if (this->isEmpty())
        return;

    QString phone = text.replace("(", "").replace(")", "").replace("-", "");

    if (phone.length() == 4 && !del){
        phone += "-";
    }else if(phone.length() > 4 && phone.length() <= 9){
        phone = phone.mid(0, 4) + "-" + phone.mid(4);
    }else if(phone.length() > 9){
        phone = "(" + phone.mid(0, 2) + ")" + phone.mid(2, 4) + "-" + phone.mid(6);
    }

    setText(phone);
}

void phoneEdit::cursorPositionChanged(int posO, int posN)
{
    this->posO = posO;
    this->posN = posN;
}
