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


 #ifndef JAGUATIINTERFACE_H
#define JAGUATIINTERFACE_H

#include <QWidget>
#include <QLabel>
#include <Component/label.h>
#include <QKeyEvent>
#include <QMessageBox>

class JaguatiInterface : public QWidget
{
    Q_OBJECT
public:
    explicit JaguatiInterface(QWidget *parent = 0);

    void createComponent();

    void connections();

    void resizeEvent(QResizeEvent *);

signals:
    void goToPanel(int);

    void fullScreen();

public slots:
    void keyPressEvent(QKeyEvent *event);

    void input();

    void adjustLayout();

    void onClick_lTwitterHeron();

    void onClick_lTwitterVegan();

    void onClick_lLinkLicense();

    void onClick_lLinkJaguati();

    void onClick_lLinkSource();

    void onClick_lLinkQt();

public:

    QWidget *wCenter;
    QLabel *lCenter;
    QLabel *lFooter;

    QLabel *lThank;

    QLabel *lNokia;
    QLabel *lContactNokia;
    QLabel *lContributionNokia;

    QLabel *lDavid;
    QLabel *lContactlDavid;
    QLabel *lContributionlDavid;

    QLabel *lCreators;

    QLabel *lHeron;
    QLabel *lEmailHeron;
    Label *lTwitterHeron;

    QLabel *lVegan;
    QLabel *lEmailVegan;
    Label *lTwitterVegan;

    QLabel *lImage;

    Label *lLinkLicense;
    Label *lLinkJaguati;
    Label *lLinkSource;
    Label *lLinkQt;

private:
    QString _linkHeron;
    QString _linkVegan;

    QString _linkLicense;
    QString _linkJaguati;
    QString _linkSource;
};

#endif // JAGUATIINTERFACE_H

