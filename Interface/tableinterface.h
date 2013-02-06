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


 #ifndef TABLEINTERFACE_H
#define TABLEINTERFACE_H

#include <QWidget>
#include <QLabel>
#include <Interface/paymentview.h>
#include <Component/tableview.h>
#include <QPushButton>
#include <Object/order.h>
#include <Abstract/datamodel.h>
#include <Interface/Table/groupdesk.h>

class TableInterface : public QWidget
{
    Q_OBJECT
public:
    explicit TableInterface(QWidget *parent = 0);
    void createComponent();
    void resizeEvent(QResizeEvent *);

signals:
    void setInput();

    void saveOrder();

    void removeProduct(usint index);

    void removeAllProducts();

    void clearOrder();

public slots:
    void input();
    void adjustLayout();
    void connections();

    void onClick_pbSaveOrdered();

    void onClick_pbRemoveItem();

    void onClick_pbClearOrdered();

    void onClick_pbCancelOrdered();

    void doubleClicked_tableItem(QModelIndex index);

public:

    PaymentView *wPaymentView;

    Order *order;

    GroupDesk *wGroupDesk;
private:

    QWidget *wItens;
    QLabel *lItens;

    QPushButton *pbSaveOrdered;
    QPushButton *pbRemoveItem;
    QPushButton *pbClearOrdered;
    QPushButton *pbCancelOrdered;


    TableView *tableItem;


};

#endif // TABLEINTERFACE_H
