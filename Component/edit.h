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


 #ifndef EDIT_H
#define EDIT_H

#include <QLineEdit>
#include <Component/label.h>
#include <QVariant>
const char url_clear[25] = ":/Extras/clear";

class Edit : public QLineEdit
{
    Q_OBJECT
public:

    explicit Edit(QWidget *parent = 0, bool usesClear = true,
                  QString standard = "", int maximumLetters = 255);

    void resizeEvent(QResizeEvent *);
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);    
    void setClear();    
    void checkClears();
    bool isEmpty();
    QString value();
    QString upper();
    void setInputMask(QString Mask);
    void clearMask();

    bool eventFilter(QObject *target, QEvent *event);

    void setNextComponent(QWidget *next);

    void connections();

    bool isUpperCase();

    void setUpperCase(bool upperCase);

    void setAutomaticallyNextComponent(bool set);

signals:
    void inFocus(bool);
    void positionChanged(int arg1, int arg2);

public slots:
    void cursorPositionChanged(int arg1, int arg2);
    void textChanged(const QString &);

    void checkEditText(QString text);
    void checkClear(bool check);
    void Clear();

    void toUpperCase(QString text);

public:
    bool useClear;    
    Label *button;
    QString valueStandart;
    bool isMask;

    bool upperCase;

    bool automaticallyNextComponent;

    QWidget *nextComponent;

    bool hasNext;

private:
    QString mask;

};

#endif // EDIT_H
