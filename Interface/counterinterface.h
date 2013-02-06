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


 #ifndef COUNTERINTERFACE_H
#define COUNTERINTERFACE_H

#include <QWidget>
#include <QLabel>
#include <Interface/paymentview.h>
#include <Component/tableview.h>
#include <QPushButton>
#include <Component/label.h>
#include <Component/search.h>
#include <QTextBrowser>
#include <Object/order.h>
#include <Abstract/datamodel.h>
#include <Interface/pizzamixed.h>
#include <QKeyEvent>
#include <QAction>
#include <Object/makehtml.h>

class CounterInterface : public QWidget
{
    Q_OBJECT
public:
    explicit CounterInterface(QWidget *parent = 0);
    void createComponent();
    void resizeEvent(QResizeEvent *);
    void statics();
    void connections();

    void setPreview();
    void setPreview(QString nome, float value, int quant);

    void updateValues();
    
    void clearOrder();

    Order *order;

    PizzaMixed *wPizzaMixed;

    PaymentView *wPaymentView;

    bool isAvailable();

    bool isAvailableBusiness();

signals:
    void newOrder();

    void setInput();

    void editPizzaMixed(QList<int> listOrderPartProduct, QList<QString> listOrderIdProduct, QList<QString> listOrderNameProduct, QString note, int quantity, double value, QString size, int row);
    
    void fullScreen();

public slots:
    void input();
    void adjustLayout();

    void onClick_lPizza();

    void onClick_pbConfirmProduct();

    void onClick_pbLeaveProduct();

    void slotPizzaMista(QList<int> listOrderPartProduct, QList<QString> listOrderIdProduct, QList<QString> listOrderNameProduct, QString note, int quantity, double value, QString size, bool insert, int row);

    void getSearchProduct(QString text);

    void doubleClicked_tableItem(QModelIndex index);

    void valueChanged_sbAmount(int value);

    void currentIndexChanged_cbSize(int index);

    void onClick_pbRemoveItem();

    void onClick_pbClearOrdered();

    void onClick_pbCancelOrdered();

    void onClick_pbSaveOrdered();

    void keyPressEvent(QKeyEvent *event);

    void onTriggered_actionName();

    void onTriggered_actionNickName();

    void editOrder(int idOrder);

    void loadDataOrder();

    void valueChanged_DSBValueProduct(double value);

private:

    bool bogth;

   QWidget *wProduct;
   QLabel * lProduct;

   Search *searchProduct;   
   QTextBrowser *productPreview;
   ComboBox *cbSize;
   DoubleSpinBox *DSBValueProduct;
   SpinBox *sbAmount;
   Label *lPizza;

   QPushButton *pbConfirmProduct;  //add & save
   QPushButton *pbLeaveProduct;    //clear & cancel,

   QWidget *wProductNotes;
   QLabel *lProductNotes;
   QLabel *lTextNotesProduct;
   QTextEdit *teProductNotes;

   QWidget *wItens;
   QLabel *lItens;

   QPushButton *pbSaveOrdered;
   QPushButton *pbRemoveItem;
   QPushButton *pbClearOrdered;
   QPushButton *pbCancelOrdered;
   TableView *tableItem;

   DataModel *dataModelItems;

   /// dados do pedido

   QList<QString> listSizes, listValues;
   QString productName;

   // dados do produto atualmente selecionado
   QString dataOrderIdProduct;
   QString dataOrderNameProduct;
   QString dataOrderSizeProduct;
   double dataOrderValueProduct;
   int dataOrderQntProduct;
   QString dataOrderNoteProduct;

   double totalValueOrder;
   double totalValueOrderProducts;

   QList<QString> listOrderIdProduct;
   QList<QString> listOrderNameProduct;
   QList<QString> listOrderSizeProduct;
   QList<QString> listOrderValueProduct;
   QList<QString> listOrderQntProduct;
   QList<QString> listOrderNoteProduct;

   int indexSelectedItem;

   // tratamento dos itens
   QList<int> orderQuantity;
   QList<double> orderValues;
   QList<QString> orderSizes;
   QList<QList<int> > orderPartProduct;
   QList<QList<QString> > orderIdProduct;
   QList<QList<QString> > orderNameProduct;
   QList<QString> orderNoteProduct;

   QAction *actionName;
   QAction *actionNickName;

public:

   MakeHTML html;

   bool isAdd;
};

#endif // COUNTERINTERFACE_H
