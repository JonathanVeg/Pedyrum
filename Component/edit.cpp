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


 #include "edit.h"
#include <QDebug>


Edit::Edit(QWidget *parent, bool usesClear, QString standard, int maximumLetters) : QLineEdit(parent)
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

    this->installEventFilter(this);

    hasNext = false;

    setAutomaticallyNextComponent(false);
}

void Edit::focusInEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);

    emit(inFocus(true));

    if(this->text() == valueStandart)
    {
        this->setText("");
        isMask = true;
    }

}

void Edit::focusOutEvent(QFocusEvent *e)
{
    QLineEdit::focusOutEvent(e);

    if(this->text() == NULL || this->text() == mask)
    {
        this->setText(valueStandart);
        isMask = false;
    }

    if (useClear)
        button->hide();
}

QString Edit::upper()
{
    return this->text().toUpper();
}

void Edit::setClear()
{
    button->setToolTip(tr("Limpar Campo"));
    button->setCursor(Qt::PointingHandCursor);
    button->setPixmap(QPixmap(url_clear));
    connect(button,SIGNAL(onClick()),this,SLOT(Clear()));
    useClear = true;
}

void Edit::checkClears()
{
    if(this->text() != "" && this->text() != valueStandart)
    {
        if(useClear)
            button->show();
    }
    else
    {
        if(useClear)
            button->hide();
    }
}

bool Edit::isEmpty()
{
    if(this->text() != "" && this->text() != valueStandart && this->text() != mask)
        return false;
    else
        return true;
}


void Edit::Clear()
{
    button->hide();
    this->setText(valueStandart);
    this->clearFocus();
    this->setFocus();
}

void Edit::checkClear(bool check)
{
    if(check)
        checkClears();
}

void Edit::checkEditText(QString text)
{
    if(text != "" && text != valueStandart)
    {
        if(useClear)
            button->show();
    }
    else
    {
        if(useClear)
            button->hide();
    }
}

void Edit::resizeEvent(QResizeEvent *)
{
    button->setGeometry((x() + width() + (height() * 0.4)) - height(),
                        y() + (height() * 0.3),height() * 0.4,height() * 0.4);
}

QString Edit::value()
{
    if(this->text() != valueStandart)
        return this->text();
    else
        return "";
}

void Edit::setInputMask(QString Mask)
{
    mask = Mask;
}

void Edit::clearMask()
{
    isMask = false;
    mask.clear();
}

void Edit::cursorPositionChanged(int arg1, int arg2)
{
    if(isMask)
    {
        QList<int> index;

        for (int n = 0; n < mask.count(); ++n)
            if(mask.mid(n,1) != " ")
                index << n;

        for (int i = 0; i < index.count(); ++i)
            if(this->text().count() == index.value(i) && arg1 < arg2)
                this->setText(this->text() + mask.mid(index.value(i),1));
    }

    emit positionChanged(arg1,arg2);
}

void Edit::textChanged(const QString &)
{
    if (! this->isEmpty()){
        QChar letter;
        int ascii;

        QString text = this->text();
        if (isMask){
            for (int i = 0; i < text.length(); i++){
                if (mask.mid(i,1) == " "){
                    letter = text.at(i).toAscii();
                    ascii = letter.toAscii();
                    if (ascii > 57 || ascii < 48)
                        this->setText(text.mid(0,text.length() - 1));
                }
            }

            if(this->text().length() == mask.length() && (! mask.isEmpty()) && automaticallyNextComponent)
                nextComponent->setFocus();

            if (hasNext && this->text().length() > mask.length() && (! mask.isEmpty())){
                this->setText(this->text().mid(0, mask.length()));
            }
        }

        if (automaticallyNextComponent && text.length() == this->maxLength())
           nextComponent->setFocus();
    }
}

void Edit::setNextComponent(QWidget *next){
    nextComponent = next;

    hasNext = true;

    connect(this, SIGNAL(returnPressed()), next, SLOT(setFocus()));
}

void Edit::connections()
{
    connect(this, SIGNAL(inFocus(bool)), this, SLOT(checkClear(bool)));

    connect(this, SIGNAL(textEdited(QString)), this, SLOT(checkEditText(QString)));

    connect(this, SIGNAL(cursorPositionChanged(int,int)), SLOT(cursorPositionChanged(int,int)));

    connect(this, SIGNAL(textChanged(QString)), SLOT(textChanged(QString)));

    connect(this, SIGNAL(textEdited(QString)), SLOT(toUpperCase(QString)));
}

void Edit::setUpperCase(bool upperCase)
{
    this->upperCase = upperCase;
}

bool Edit::isUpperCase()
{
    return this->upperCase;
}

void Edit::toUpperCase(QString text){
    Q_UNUSED(text);

    if (isUpperCase())
        this->setText(this->text().toUpper());
}

bool Edit::eventFilter(QObject *target, QEvent *event)
{
    if (event->type() == QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if (keyEvent->key() == Qt::Key_Escape)
            this->Clear();
    }

    return false;
}

void Edit::setAutomaticallyNextComponent(bool set)
{
    automaticallyNextComponent = set;
}
