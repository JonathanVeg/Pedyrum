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


 #ifndef CONFIGINTERFACE_H
#define CONFIGINTERFACE_H

#include <QWidget>
#include <QLabel>
#include <Component/item.h>
#include <QComboBox>
#include <Component/edit.h>
#include <Component/phoneedit.h>
#include <Component/spinbox.h>
#include <Abstract/tools.h>
#include <QSpinBox>
#include <QStackedWidget>
#include <QTextBrowser>
#include <QPushButton>
#include <Object/config.h>
#include <QCheckBox>
#include <QProgressBar>
#include <QFileDialog>
#include <QTimer>
#include <Object/backup.h>
#include <Abstract/backupthread.h>
#include <Abstract/restorethread.h>
#include <QListView>
#include <Abstract/datamodel.h>
#include <QToolButton>
#include <QAction>
#include <QKeyEvent>
#include <Interface/Configure/config_manageuser.h>

class ConfigInterface : public QWidget
{
    Q_OBJECT
public:

    explicit ConfigInterface(QWidget *parent = 0);

    void createComponent();

    void resizeEvent(QResizeEvent *);

    void connections();

    bool isAvailableBusiness();

    bool isAvailableOrder();



signals:
    void finishedRestore();

    void reloadOrdered();

    void goToPanel(int);

    void fullScreen();

public slots:

    void keyPressEvent(QKeyEvent *event);

    void input();

    void adjustLayout();

    void onClick_itemFormPayment();
    void onClick_itemOrdered();
    void onClick_itemBusiness();
    void onClick_itemDB();
    void onClick_itemAccount();
    void onClick_itemAlerts();
    void onClick_itemNetwork();

    void onClick_pbSaveBusiness();
    void onClick_pbSaveOrdered();
    void onClick_pbSaveAlert();
    void onClick_pbSavePaymentForm();
    void onClick_pbSaveNetwork();

    void onClick_pbBackup();
    void onClick_pbRestore();

    void finished_Backup();
    void finished_Restore();

    void onClick_pbAddPaymentForm();
    void onClick_pbRemovePaymentForm();

    void deadLineProgress();

    void setPercentTextValue();

    void oncurrentIndexChanged_cbOptionNumber(int index);

    void onTriggered_aShowPasswordBusiness();

    void onTextChanged_ePasswordBusiness();

    void onCurrentIndexChanged_cbTypeConnection(int index);

    QString getIP();

public:

    Config *config;

    Tools *tools;

    QWidget *wBar;
    QLabel  *lBar;

    Item *itemFormPayment;
    Item *itemOrdered;
    Item *itemBusiness;
    Item *itemDB;
    Item *itemAccount;
    Item *itemAlerts;
    Item *itemNetwork;

    QStackedWidget *swPanels;

    QWidget *wFormPayment;
    QLabel *lFormPayment;
    QListView *listFormPayment;
    DataModel *modelPaymentForm;
    Edit *eNamePaymentForm;
    QPushButton *pbAddPaymentForm;
    QPushButton *pbRemovePaymentForm;
    QPushButton *pbSavePaymentForm;

    QWidget *wOrdered;    
    QLabel *lOrdered;
    SpinBox *sbNumberCopy;
    QComboBox *cbFont;
    QComboBox *cbOptionNumber;
    SpinBox *sbCurrentNumber;
    QTextBrowser *tbNumberDescription;
    QPushButton *pbSaveOrdered;

    QWidget *wBusiness;
    QLabel *lBusiness;
    phoneEdit *ePhoneBusiness;
    Edit *eNameBusinnes;
    Edit *eAddressBusiness;
    Edit *eDistrictBusiness;
    Edit *eCityBusiness;
    QComboBox *cbStateBusiness;
    QPushButton *pbSaveBusiness;

    QWidget *wAlert;
    QLabel *lAlert;
    QLabel *lTextAOrderedTime;
    SpinBox *sbDaysAlertOrder;
    QPushButton *pbSaveAlert;

    QWidget *wBackupRestore;
    QLabel *lBackupRestore;
    QLabel *lLastBackup;
    QPushButton *pbBackup;
    QPushButton *pbRestore;
    QLabel *lOperation;
    QLabel *lTextValueProgress;
    QProgressBar *progressOperation;
    QTimer *timerOperationDB;
    short int oldProgress;    
    QString fileNameDB;
    Backup *backup;
    bool isBackup;

    Config_ManageUser *wAccountUser;

    QWidget *wNetwork;
    QLabel *lNetwork;
    QLabel *lIP;
    Edit *eUsernameNetwork;
    Edit *ePasswordNetwork;
    QToolButton *tbViewPasswordNetwork;
    QAction *aShowPasswordNetwork;
    QComboBox *cbTypeConnection;
    Edit *eIPNetwork;
    Edit *ePortNetwork;
    Edit *eDir;
    QPushButton *pbSaveNetwork;

    BackupThread *bThread;
    RestoreThread *rThread;
};

#endif // CONFIGINTERFACE_H

