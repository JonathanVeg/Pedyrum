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


 #include "jaguatiinterface.h"
#include <QUrl>
#include <QDesktopServices>
#include <QDebug>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

JaguatiInterface::JaguatiInterface(QWidget *parent) :
    QWidget(parent)
{
    createComponent();
    connections();
}

void JaguatiInterface::createComponent()
{
    wCenter = new QWidget(this);
    lCenter = new QLabel(wCenter);
    lFooter = new QLabel(wCenter);

    lThank = new QLabel(tr("Agradecimentos"),wCenter);

    lNokia = new QLabel(tr("Nokia Corporation"),wCenter);
    lContactNokia = new QLabel(tr("Contact: < qt-info@nokia.com >"),wCenter);
    lContributionNokia = new QLabel(tr("contribution: QModelTable.h & QModelTable.cpp"),wCenter);

    lDavid = new QLabel(tr("David Johnson"),wCenter);
    lContactlDavid = new QLabel(tr("Contact: < david@usermode.org >"),wCenter);
    lContributionlDavid = new QLabel(tr("contribution: TextPrint.h & TextPrint.cpp"),wCenter);

    lCreators = new QLabel(tr("Criadores"),wCenter);

    lHeron = new QLabel(tr("Luiz Cândido da Silva Júnior"),wCenter);
    lEmailHeron = new QLabel(tr("Contact: < heron@jaguati.com.br >"),wCenter);
    lTwitterHeron = new Label(wCenter);

    lVegan = new QLabel(tr("Jonathan Gonçalves da Silva"),wCenter);
    lEmailVegan = new QLabel(tr("Conctact: < jonathan@jaguati.com.br >"),wCenter);
    lTwitterVegan = new Label(wCenter);

    lImage = new QLabel(wCenter);

    lLinkLicense = new Label(wCenter);
    lLinkJaguati = new Label(wCenter);
    lLinkSource = new Label(wCenter);
    lLinkQt = new Label(wCenter);
}


void JaguatiInterface::input()
{
    _linkHeron = "https://twitter.com/Junior_Heron";
    _linkVegan = "https://twitter.com/JonathanVeg2";

    _linkLicense = "http://pt.wikipedia.org/wiki/Licença_BSD";
    _linkJaguati = "www.jaguati.com.br";
    _linkSource = "https://github.com/JaguatiSource/Pedyrum4/";

    lCenter->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.498,"
                           "y1:0, x2:0.494636, y2:1, stop:0 rgba(255, 255, 255, 255),"
                           "stop:0.982955 rgba(248, 248, 248, 255));"
                           "border-radius: 5px; border: 1px solid rgba(0,0,0,30);");

    lFooter->setStyleSheet("background-color: rgba(0, 0, 0, 5);"
                           "border-radius: 5px; border: 1px solid rgba(0,0,0,15);");

    QFont f;
    f.setPointSize(13);

    lThank->setStyleSheet("color: rgba(0,0,0,180);");
    lThank->setFont(f);

    lCreators->setFont(f);
    lCreators->setStyleSheet("color: rgba(0,0,0,180);");

    f.setPixelSize(11);
    f.setBold(true);

    lNokia->setFont(f);
    lDavid->setFont(f);
    lHeron->setFont(f);
    lVegan->setFont(f);

    f.setBold(false);

    lContactNokia->setFont(f);
    lContactNokia->setStyleSheet("color: rgba(0,0,0,150);");

    lContributionNokia->setFont(f);
    lContributionNokia->setStyleSheet("color: rgba(0,0,0,150);");

    lContactlDavid->setFont(f);
    lContactlDavid->setStyleSheet("color: rgba(0,0,0,150);");

    lContributionlDavid->setFont(f);
    lContributionlDavid->setStyleSheet("color: rgba(0,0,0,150);");

    lContributionlDavid->setFont(f);
    lContributionlDavid->setStyleSheet("color: rgba(0,0,0,150);");

    lEmailHeron->setFont(f);
    lEmailHeron->setStyleSheet("color: rgba(0,0,0,150);");

    lTwitterHeron->setFont(f);
    lTwitterHeron->setStyleSheet("color: rgba(30,144,255,255);");
    lTwitterHeron->setText(tr("@Junior_Heron"));

    lEmailVegan->setFont(f);
    lEmailVegan->setStyleSheet("color: rgba(0,0,0,150);");

    lTwitterVegan->setFont(f);
    lTwitterVegan->setStyleSheet("color: rgba(30,144,255,255);");
    lTwitterVegan->setText(tr("@JonathanVeg2"));

    lImage->setScaledContents(true);
    lImage->setPixmap(QPixmap(":/Extras/credito-logo"));

    lLinkLicense->setFont(f);
    lLinkLicense->setStyleSheet("color: rgba(30,144,255,255);");
    lLinkLicense->setText(tr("Sobre a BSD"));
    lLinkLicense->setAlignment(Qt::AlignCenter);

    lLinkJaguati->setFont(f);
    lLinkJaguati->setStyleSheet("color: rgba(30,144,255,255);");

    lLinkJaguati->setText(tr("Conheça a Jaguati"));
    lLinkJaguati->setAlignment(Qt::AlignCenter);

    lLinkSource->setFont(f);
    lLinkSource->setStyleSheet("color: rgba(30,144,255,255);");
    lLinkSource->setText(tr("Código fonte"));
    lLinkSource->setAlignment(Qt::AlignCenter);

    lLinkQt->setFont(f);
    lLinkQt->setStyleSheet("color: rgba(30,144,255,255);");
    lLinkQt->setText(tr("Sobre o Qt"));
    lLinkQt->setAlignment(Qt::AlignCenter);
}

void JaguatiInterface::adjustLayout()
{    
    wCenter->setGeometry(this->width()/2 - 350,80,700,450);

    short int x = wCenter->width() - 280;

    lCenter->setGeometry(0,0,wCenter->width(),wCenter->height());

    lFooter->setGeometry(0,wCenter->height()-50,wCenter->width(),50);

    lThank->setGeometry(x,25,150,20);

    lNokia->setGeometry(x,lThank->y() + lThank->height() + 15,250,18);
    lContactNokia->setGeometry(x,lNokia->y() + lNokia->height() + 3,250,18);
    lContributionNokia->setGeometry(x,lContactNokia->y() + lContactNokia->height() + 3,250,18);

    lDavid->setGeometry(x,lNokia->y() + lNokia->height() + 50,250,18);
    lContactlDavid->setGeometry(x,lDavid->y() + lDavid->height() + 3,250,18);
    lContributionlDavid->setGeometry(x,lContactlDavid->y() + lContactlDavid->height() + 3,250,18);

    lCreators->setGeometry(x,lDavid->y() + lDavid->height() + 60,250,20);

    lHeron->setGeometry(x,lCreators->y() + lCreators->height() + 15,250,18);
    lEmailHeron->setGeometry(x,lHeron->y() + lHeron->height() + 3,250,18);
    lTwitterHeron->setGeometry(x,lEmailHeron->y() + lEmailHeron->height() + 3,250,18);

    lVegan->setGeometry(x,lHeron->y() + lHeron->height() + 50,250,18);
    lEmailVegan->setGeometry(x,lVegan->y() + lVegan->height() + 3,250,18);
    lTwitterVegan->setGeometry(x,lEmailVegan->y() + lEmailVegan->height() + 3,250,18);

    lImage->setGeometry(20,40,wCenter->width() - 330,wCenter->height() - 120);

    x = wCenter->width()/4;

    lLinkLicense->setGeometry(0,lFooter->y() + lFooter->height()/2 - 9,x,18);

    lLinkJaguati->setGeometry(x,lLinkLicense->y(),x,18);

    lLinkSource->setGeometry(x * 2,lLinkLicense->y(),x,18);

    lLinkQt->setGeometry(x * 3,lLinkLicense->y(),x,18);

}


void JaguatiInterface::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void JaguatiInterface::connections()
{
    connect(lTwitterHeron,SIGNAL(onClick()),SLOT(onClick_lTwitterHeron()));

    connect(lTwitterVegan,SIGNAL(onClick()),SLOT(onClick_lTwitterVegan()));

    connect(lLinkLicense,SIGNAL(onClick()),SLOT(onClick_lLinkLicense()));

    connect(lLinkJaguati,SIGNAL(onClick()),SLOT(onClick_lLinkJaguati()));

    connect(lLinkSource,SIGNAL(onClick()),SLOT(onClick_lLinkSource()));

    connect(lLinkQt,SIGNAL(onClick()),SLOT(onClick_lLinkQt()));
}

void JaguatiInterface::keyPressEvent(QKeyEvent *event)
{

    switch (event->modifiers()){

    case Qt::ControlModifier:{
        if (event->key() == Qt::Key_Tab)
            emit goToPanel(4);

        break;
    }
    }

    if (event->key() == Qt::Key_Backtab)
        emit goToPanel(11);

    if (event->key() == Qt::Key_F11)
        emit fullScreen();
}

void JaguatiInterface::onClick_lTwitterHeron()
{
    QDesktopServices::openUrl(QUrl(_linkHeron));
}

void JaguatiInterface::onClick_lTwitterVegan()
{
    QDesktopServices::openUrl(QUrl(_linkVegan));
}

void JaguatiInterface::onClick_lLinkLicense()
{
    QDesktopServices::openUrl(QUrl(_linkLicense));
}

void JaguatiInterface::onClick_lLinkJaguati()
{
    QDesktopServices::openUrl(QUrl(_linkJaguati));
}

void JaguatiInterface::onClick_lLinkSource()
{
    QDesktopServices::openUrl(QUrl(_linkSource));
}

void JaguatiInterface::onClick_lLinkQt()
{
    QMessageBox::aboutQt(this,tr("Sobre o Qt"));
}
