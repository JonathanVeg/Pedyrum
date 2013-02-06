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


 #ifndef ORDEREDINTERFACE_H
#define ORDEREDINTERFACE_H

#include <QWidget>
#include <QLabel>
#include <Component/label.h>
#include <QTextBrowser>
#include <Component/search.h>
#include <Component/tableview.h>
#include <Component/combobox.h>
#include <QPushButton>
#include <Object/ordered.h>
#include <Abstract/tools.h>
#include <Interface/selectdeliverer.h>
#include <QAction>
#include <Object/makehtml.h>
#include <QKeyEvent>

class OrderedInterface : public QWidget
{
    Q_OBJECT
public:
    explicit OrderedInterface(QWidget *parent = 0);

    void createComponent();

    void statics();

    void connections();

    void resizeEvent(QResizeEvent *);

    void setProfileVisible(bool b);

    void insertItemsInTable();

    int textCountOrderedSize();

    void updateCountOrdered(QString qnt);

    void showSelectDeliveres();

signals:    
    void gotoPanel(int index, int index2, int idOrder = 0); // 0 é no caso do novo pedido, que não tem edição

    void goToPanel(int);

private slots:
    void keyPressEvent(QKeyEvent *event);

    void onClick_lAdd();
    void onClick_lRemove();
    void onClick_lProfile();

signals:
    void fullScreen();

public slots:

    void input();

    void adjustLayout();

    void onClick_pbAll();

    void onClick_pbToday();

    void onClick_pbFree();

    void onClick_pbOccupied();

    void clicked_tableOrdered(QModelIndex index);

    void clicked_tableFunctionary(QModelIndex index);

    void currentIndexChanged_cbChangeStatusDelegate(QString text);

    void currentIndexChanged_cbStatus(QString text);

    void doubleClicked_tableOrdered(QModelIndex index);

    void setFilterModel();

    void updateDataFuncionary(QString id);

    void getSearchEmployee(QString text);

    void onTriggered_actionPhone();

    void onTriggered_actionDate();

    void onTriggered_actionHour();

    void onClick_pbPrint();

    void onClick_pbPDF();

    void rowChanged_tableOrdered(int row);

public:
    MakeHTML *makeHTML;

    Ordered *ordered;

    ComboBox *cbChangeStatusDelegate;

    bool isProfileShow;

    bool isShowDeliverer;

    QLabel *lTop;

    Label *lAdd;
    Label *lRemove;
    Label *lProfile;
    QTextBrowser *textCountOrdered;

    ComboBox *cbShowType;
    ComboBox *cbShowStatus;

    QPushButton *pbToday;
    QPushButton *pbAll;

    Search *searchOrdered;
    QAction *actionPhone;
    QAction *actionDate;
    QAction *actionHour;

    TableView *tableOrdered;

    QWidget *wViewOrdered;
    QLabel *lViewOrdered;
    QPushButton *pbPDF;
    QPushButton *pbPrint;
    QLabel *lStatusStatic;
    QLabel *lStatus;
    QTextBrowser *teView;

    QWidget *wFunctionary;
    QLabel *lFunctionary;
    Search *searchFunctionary;
    ComboBox *cbStatus;
    QPushButton *pbFree;
    QPushButton *pbOccupied;
    TableView *tableFunctionary;

    Tools tools;

    bool isToday;

    SelectDeliverer *selectDeliverer;

    QString currentIdOrder;
};

#endif // ORDEREDINTERFACE_H

