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


 #ifndef CLIENTINTERFACE_H
#define CLIENTINTERFACE_H

#include <QWidget>

#include <Component/label.h>
#include <Component/edit.h>
#include <Component/tableview.h>
#include <Component/search.h>
#include <Abstract/graphiceffects.h>
#include <Object/client.h>
#include <QTextBrowser>
#include <QStackedWidget>
#include <Component/doublespinbox.h>
#include <Component/phoneedit.h>
#include <QTextEdit>
#include <QPushButton>
#include <QSplitter>
#include <Abstract/tools.h>
#include <QKeyEvent>

const QString CSS_INVALID_EDIT_CLIENT =
"QLineEdit:focus, QLineEdit:hover"
"{"
"color: rgb(0,0,0);"
"selection-color: GhostWhite;"
"border: 2px solid #009ACD;"
"}"

"QLineEdit "
"{"
"border: 1px solid #B0C4DE;"
"border-radius: 5px;"
"padding: 1px 14px 1px 5px;"
"color:#990000;"
"background-color: rgb(255,255,255);"

"}";

const QString CSS_VALID_EDIT_CLIENT =
"QLineEdit:focus, QLineEdit:hover"
"{"
"color: rgb(0,0,0);"
"selection-color: GhostWhite;"
"border: 2px solid #009ACD;"
"}"

"QLineEdit "
"{"
"border: 1px solid #B0C4DE;"
"border-radius: 5px;"
"padding: 1px 14px 1px 5px;"
"color:#333333;"
"background-color: rgb(255,255,255);"

"}";

class ClientInterface : public QWidget
{
    Q_OBJECT
public:
    explicit ClientInterface(QWidget *parent = 0);
    void createComponent(); // Cria os componentes na tela.

    void connections();     // cria as coneções.
    void hidePanel(bool b);
    void statics();         // alinha o layout estático.

    void createProfile(Client *c);
    void createProfile();

    int textCountClientSize();

private:
    void resizeEvent(QResizeEvent *);    
    bool isAvailable();
    bool isAvailableBusiness();

signals:
    void goToPanel(int);

    void fullScreen();

public slots:        

    void updateCountClients(QString qnt);
    void input();
    void adjustLayout();
    //void getDataAdress();
    void getDataClient(QString phone);
    void getDataClientBusiness(QString phoneBusiness);
    void gotoPanel(int index);
    void adjustSplitter();
    void nextPanel();

    void onDoubleClick_tbClient(QModelIndex index);
    void onClick_tbClient(QModelIndex index);
    void rowChanged_tbClient(int row);

    void onTextChanged_ePhone(QString text);
    void onTextChanged_ePhoneBusiness(QString text);
    void onTextChanged_eCNPJ(QString text);

    void inFocus_ePhoneBusiness(bool focus);

    void inFocus_ePhone(bool focus);

    void editingFinished_ePhone();

    void editingFinished_ePhoneBusiness();

    void logout();

private slots:
    void keyPressEvent(QKeyEvent *event);

    void onClick_lAdd();
    void onClick_lRemove();
    void onClick_lProfile();
    void onClick_pbConfirm();
    void onClick_pbLeave();
    void onClick_pbLeaveBusiness();
    void onClick_pbConfirmBusiness();
    void onClick_lRemoveBusiness();
    void getSearchClient(QString text);
    void onClick_pbSelectPeople();
    void onClick_pbSelectBusiness();

    void onTriggered_actionPhone();
    void onTriggered_actionName();

public:

    short int nextPanelIndex;
    short int sizePanel;

    int idClientUpdate;

    QLabel *lTop;

    Label *lAdd;
    Label *lRemove;
    Label *lProfile;
    QTextBrowser *textCountClient;

    Search *searchClient;

    QWidget *wSpace;
    QLabel *lBackgroundSpace;

    QStackedWidget *swPanels;
    QStackedWidget *swClientType;

    QPushButton *pbSelectPeople;
    QPushButton *pbSelectBusiness;

    TableView *tableClient;
    QSplitter *splitter;

    QWidget *wGroupClient;
    QLabel *lBackgroundPlus;
    phoneEdit *ePhone;
    Edit *eCep;
    Edit *eName;
    Edit *eStreet;
    Edit *eNumber;
    Edit *eComplement;
    Edit *eDistrict;
    DoubleSpinBox *DSBShipping;
    QTextEdit *teNotes;
    QPushButton *pbConfirm;  //add & save
    QPushButton *pbLeave;    //clear & cancel

    QWidget *wGroupBusiness;
    QLabel *lBgPlusBusiness;
    phoneEdit *ePhoneBusiness;
    Edit *eCepBusiness;
    Edit *eNameContactBusiness;
    Edit *eNameBusiness;
    Edit *eFantasyNameBusiness;
    Edit *eCNPJBusiness;
    Edit *eStreetBusiness;
    Edit *eNumberBusiness;
    Edit *eComplementBusiness;
    Edit *eDistrictBusiness;
    DoubleSpinBox *DSBShippingBusiness;
    QTextEdit* teNotesBusiness;
    QPushButton *pbConfirmBusiness;
    QPushButton *pbLeaveBusiness;

    QWidget *wGroupProfile;
    QTextBrowser *tbProfile;

    QWidget *wGroupAnalytics;

    Client *client;

    GraphicEffects *panelEffects;

    QPropertyAnimation  *  animaUp;
    QPropertyAnimation  *  animaDown;

    QAction *actionPhone;
    QAction *actionName;


private:
    bool isAdd;
    bool isAddBusiness;
    float percentSplitter;


    bool bogth;

    int typeSelected;

    enum{
        NORMAL = 1,
        BUSINESS = 2
    };

    Tools tools;
};

#endif // CLIENTINTERFACE_H
