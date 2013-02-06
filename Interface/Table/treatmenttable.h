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


 #ifndef TREATMENTTABLE_H
#define TREATMENTTABLE_H

#include <QWidget>
#include <QLabel>
#include <Component/search.h>
#include <QTextBrowser>
#include <QDoubleSpinBox>
#include <Component/doublespinbox.h>
#include <Component/combobox.h>
#include <Component/spinbox.h>
#include <QPushButton>
#include <Object/order.h>
#include <Object/makehtml.h>
#include <Interface/pizzamixed.h>
#include <QKeyEvent>
#include <Interface/Table/dataitem.h>
#include <Interface/Table/generaldata.h>
#include <Component/tableview.h>
#include <Interface/paymentview.h>

class TreatmentTable : public QWidget
{
    Q_OBJECT

public:
    explicit TreatmentTable(QWidget *parent = 0);

    void createComponent();

    void statics();

    void adjustLayout();

    void resizeEvent(QResizeEvent *);

    void connections();

    void setCompleterProduct(QCompleter *completer);

    void updateValues();

    // na mudança de mesas, seta o model certo
    void setDataModelItems(DataModel *dataModelItems);

    void setGeneralData(GeneralData * generalData);

    // calcula os valores, simples e complexos das mesas.
    double calculateValueSimple(int index = -1); // se for -1 ele calcula para a própria mesa

    double calculateValueComplex();

    void setTableItem(TableView * tableItem);

    void clearAllOrders();

    void setWGroupMedia(QWidget *wGroupMedia, SpinBox *SBGroupMedia);

    void receiveTotalPeople(int people);

signals:    

    void closeTable();

    void closeWGroupMedia();

    void newOrder();

    void sendModelItems(DataModel *model);

    void back();

    void updateData(int index,bool expor);

    void editPizzaMixed(QList<int> listOrderPartProduct, QList<QString> listOrderIdProduct, QList<QString> listOrderNameProduct, QString note, int quantity, double value, QString size, int row);

public slots:

    void input();

    void saveOrder();

    void saveOrderPt2(bool isMedia = false, int countPeople = -1);

    void doubleClicked_tableItem(QModelIndex index);

    void slotPizzaMista(QList<int> listOrderPartProduct, QList<QString> listOrderIdProduct, QList<QString> listOrderNameProduct, QString note, int quantity, double value, QString size, bool insert, int row);

    void started();

    void keyPressEvent(QKeyEvent *event);

    void onClick_lPizza();

    void onClick_pbConfirmProduct();

    void onClick_pbLeaveProduct();

    void onClick_pbBack();

    void onClick_lNotes();

    void onClick_pbConfirmNotes();

    void load(int index);

    void setPreview(QString nome, float value, int quant);

    void setPreview();

    void valueChanged_DSBValueProduct(double value);

    void valueChanged_sbAmount(int value);

    void getSearchProduct(QString text);

    void removeProduct(usint index);

    void removeAllProducts();

    void clearOrder();

private:

    QString toStr(usint value);
    QString toStr(int value);
    QString toStr(double value);

    QString descriptionValueComplex;

    bool isAvailable();

    bool isAvailableBusiness();



    DataItem *dataItem(int index) const {return _dataItem[index];}

public:

    bool gettingMedia;

    bool isAdd;

    void setPaymentView(PaymentView * paymentView);

    usint selected() const {return _selected;}

    void setSelectedTable(int index);

    PaymentView *wPaymentView;

    Order *order;

    DataItem *_dataItem[100];
    usint _selected;

    GeneralData *_generalData;

    QWidget *wProduct;
    QLabel * lProduct;

    Search *searchProduct;
    QTextBrowser *productPreview;
    DoubleSpinBox *DSBValueProduct;
    ComboBox *cbSize;
    SpinBox *sbAmount;
    Label *lPizza;
    Label *lNotes;

    QPushButton *pbConfirmProduct;  //add & save
    QPushButton *pbLeaveProduct;    //clear & cancel,

    QWidget *wNotes;
    QLabel *lBgNotes;
    QTextEdit *teProductNotes;
    QPushButton *pbConfirmNotes;

    ComboBox *cbBartender;
    QLabel *lTable;
    QLabel *lTableText;
    QLabel *lTableTitle;
    QPushButton *pbBack;

    // dados do produto atualmente selecionado

    QAction *actionName;
    QAction *actionNickName;

    Tools tools;

    PizzaMixed *wPizzaMixed;

    MakeHTML html;


private:

    QString linkedTablesStr;
    QList<usint> linkedTables;

    QList<QList<QString> > bartenders;

    QWidget *wGroupMedia;
    SpinBox *SBGroupMedia;

    // copia do model para facilitar minha vida

    TableView * tableItem;

    // pedido

    QString productName;

    QList<QString> listSizes, listValues;

    DataModel* dataModelItems;
    DataModel* dataModelPayment;

    QList<QString> dataClienteI;
    QList<QString> dataClienteF;

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

public:

    bool individual;

    int indexSelectedItem;

    int idClient;

    // tratamento dos itens
    QList<int> orderQuantity;
    QList<double> orderValues;
    QList<QString> orderSizes;
    QList<QList<int> > orderPartProduct;
    QList<QList<QString> > orderIdProduct;
    QList<QList<QString> > orderNameProduct;
    QList<QString> orderNoteProduct;

};

#endif // TREATMENTTABLE_H
