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


 #ifndef PEDYRUM_H
#define PEDYRUM_H

#include <QMainWindow>

#include <Abstract/confnetwork.h>
#include <Abstract/encryption.h>

#include <Interface/logininterface.h>
#include <Interface/firstadmininterface.h>
#include <Interface/clientinterface.h>
#include <Interface/productinterface.h>
#include <Interface/functionaryinterface.h>
#include <Interface/treatmentinterface.h>
#include <Interface/orderedinterface.h>
#include <Interface/alertinterface.h>
#include <Interface/accountfinanceinterface.h>
#include <Interface/payboxinterface.h>
#include <Interface/configinterface.h>
#include <Interface/jaguatiinterface.h>
#include <Interface/usermenu.h>
#include <Interface/ads.h>

#include <Component/messagealert.h>
#include <Component/topmenu.h>
#include <Component/sidebar.h>

#include <Abstract/bdtools.h>
#include <Abstract/tools.h>

#include <Object/backup.h>

#include <QStackedWidget>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QPoint>



namespace Ui {
class Pedyrum;
}

class Pedyrum : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Pedyrum(QWidget *parent = 0);

    void createComponent(); // Cria os componentes na tela.

    void resizeEvent(QResizeEvent *);

    void connections();

    void closeEvent(QCloseEvent *e);

    void input();

    BDTools bd;
    Tools tools;

    ~Pedyrum();

public slots:

    void openLogin();

    void onFocus_Menu(bool b);
    void goToPanel(int currentIndex);    
    void setFullScreen(bool b);
    void adjustLayout();

    void keyPressEvent(QKeyEvent *event);

    void connected();

    void showNewAlert();

    void trayIcon_MessageCliked();

    void setMessageObject(MessageObject *message);

    void showAlert();

    void hideAlert();

    void openSystem(QString name, QString id, QList<bool> permissions);

    void logout();

    void configureUser();

    void timeLogoutUpdate();

    void reloadModels(int index);

private slots:
    void indexLoadOrdered(int index, int index2, int idOrder = 0);

signals:
    void finishedInput();
    void restoreLayoutScreen();

private:
    Ui::Pedyrum *ui;

    short int H_;
    short int W_;
    short int X_;
    short int Y_;

    QString styleSheet;

    QPropertyAnimation *next;

    topMenu *wTopMenu;    

    sideBar *wSideBar;

    ConfNetwork *wConfNetwork;

    FirstAdminInterface *wFirstAdminInterface;

    LoginInterface *wLoginInterface;

    QStackedWidget *swPanels;

    TreatmentInterface *wTreatmentInterface;

    ClientInterface *wClientInterface;

    ProductInterface *wProductInterface;

    OrderedInterface *wOrderedInterface;

    FunctionaryInterface *wFunctionaryInterface;    

    AlertInterface *wAlertInterface;

    QStackedWidget *swFinance;

    AccountFinanceInterface *wBillFinanceInterface;

    PayBoxInterface *wCahsInterface;

    ConfigInterface *wConfigInterface;

    JaguatiInterface *wJaguatiInterface;

    Ads *wAds;

    GraphicEffects * animationMenu;

    Backup backup;

    enum windowState{
        NORMAL,
        MAXIMIZED,
        FULL_SCREEN
    };

    windowState wState;

    QSystemTrayIcon *trayIcon;

    MessageAlert *wMessage;

    UserMenu *wUserMenu;

    QTimer *timeLogout;
    QTimer *timeServer;

    QPoint oldPoint;
    int delayLogout;

    QString idCurrentSystemUser;

    Encryption encryption;

};

#endif // PEDYRUM_H

