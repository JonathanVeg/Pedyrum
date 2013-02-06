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


 #ifndef FUNCTIONARYINTERFACE_H
#define FUNCTIONARYINTERFACE_H

#include <Abstract/graphiceffects.h>
#include <Abstract/tools.h>
#include <Component/combobox.h>
#include <Component/doublespinbox.h>
#include <Component/edit.h>
#include <Component/label.h>
#include <Component/phoneedit.h>
#include <Component/search.h>
#include <Component/spinbox.h>
#include <Component/tableview.h>
#include <Object/employee.h>
#include <QKeyEvent>
#include <QLabel>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QSplitter>
#include <QStackedWidget>
#include <QTabWidget>
#include <QTextBrowser>
#include <QWidget>

const QString CSS_INVALID_EDIT_EMPLOYEE =
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

const QString CSS_VALID_EDIT_EMPLOYEE =
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

class FunctionaryInterface : public QWidget
{
    Q_OBJECT

public:
    explicit FunctionaryInterface(QWidget *parent = 0);
    void createComponent(); // Cria os componentes na tela.    
    void connections();     // cria as coneções.
    void hidePanel(bool b);
    void statics();         // alinha o layout estático.
    bool isAvailable();
    void createProfile();
    void createProfileWithData();

private:
    void resizeEvent(QResizeEvent *);

    inline int row(); // retorna a linha corrente da tableEmployee

signals:
    void goToPanel(int);

    void fullScreen();
    
public slots:
    void adjustSplitter();
    void adjustLayout();
    void gotoPanel(int index);
    void nextPanel();
    void input();           // informa as configs dos componentes

    void keyPressEvent(QKeyEvent *event);      

    void logout();

private slots:
    void onClick_lAdd();
    void onClick_lRemove();
    void onClick_lProfile();
    void onClick_lCalc();
    void onClick_pbConfirm();
    void onClick_pbLeave();

    void textChanged_eCPF(QString text);
    void doubleClicked_tableEmployee(QModelIndex index);
    void currentIndexChanged_cbSelectJobRole(QString text);
    void onClick_pbCalculateCommission();

    void onClick_pbAvailable();
    void onClick_pbUnavailable();

    void getSearchFunctionary(QString text);


public:
    short int nextPanelIndex;
    short int sizePanel;

    QLabel *lTop;

    Label *lAdd;
    Label *lRemove;
    Label *lProfile;
    Label *lCalc;
    QPushButton *pbAvailable;
    QPushButton *pbUnavailable;
    ComboBox * cbSelectJobRole;

    Search *searchFunctionary;

    QWidget *wSpace;
    QLabel *lBackgroundSpace;

    QStackedWidget *swPanels;

    QWidget *wGroupPlus;
    QLabel *lBackgroundPlus;

    // Itens normais.

    Edit *eName;    
    Edit *eCPF;
    Edit *eRG;
    phoneEdit *ePhone;
    phoneEdit *eMobile;
    Edit *eRadio;
    Edit *eCep;
    Edit *eStreet;
    Edit *eNumber;
    Edit *eComplement;
    Edit *eDistrict;            

    ComboBox *cbJobRole;

    QLabel *textPercent;
    DoubleSpinBox *DSBComission;

    Edit *eBank;
    Edit *eCC;
    Edit *eAg;

    QTextEdit *teNoteFunctionary;

    QPushButton *pbConfirm;  //add & save
    QPushButton *pbLeave;    //clear & cancel

    QWidget *wGroupProfile;
    QTextBrowser *tbProfile;

    TableView *tableEmployee;
    QSplitter *splitter;

    GraphicEffects *panelEffects;

    QPropertyAnimation  *  animaUp;
    QPropertyAnimation  *  animaDown;

    Employee *employee;

    Tools tools;
    
    bool isAdd;
    int idEmployeeUpdate;

    QWidget *wCalc;
    QLabel *lCalcbg;

    QLabel *lValueCommission;
    Edit *eBeforeDateCommission;
    Edit *eAfterDateCommission;
    QPushButton *pbCalculateCommission;
};

#endif // FUNCTIONARYINTERFACE_H
