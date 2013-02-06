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


 #ifndef PAYMENTVIEW_H
#define PAYMENTVIEW_H

#include <QWidget>
#include <QLabel>
#include <Component/label.h>
#include <Component/combobox.h>
#include <QPushButton>
#include <Component/tableview.h>
#include <QStackedWidget>
#include <QResizeEvent>
#include <QTextEdit>
#include <Component/edit.h>
#include <Component/doublespinbox.h>
#include <Component/phoneedit.h>
#include <Object/order.h>
#include <Abstract/tools.h>
#include <Abstract/datamodel.h>
#include <QKeyEvent>

class PaymentView : public QWidget
{
    Q_OBJECT
public:
    explicit PaymentView(QWidget *parent = 0);
    void createComponent();   
    void resizeEvent(QResizeEvent *);
    void statics();
    void connections();

    void updateValuesTotal(double total);

    void cancelOrdered();

    void updateOthersValues();

    void clearValues();

signals:
    void goToPanel(int);
    void save();
    void clear();
    void cancel();
    void pizza();

    void fullScreen();

public slots:
    void keyPressEvent(QKeyEvent *event);

    void input();

    void adjustLayout();

    void onClick_lClient();

    void onClick_pbSelectPeople();

    void onClick_pbSelectBusiness();

    void onClick_pbLeave();

    void onClick_pbLeaveBusiness();

    void onTextChanged_ePhone(QString text);

    void onTextChanged_ePhoneBusiness(QString text);

    void onTextChanged_eCep(QString text);

    void onTextChanged_eCepBusiness(QString text);

    void onClick_pbAddPaymentForm();

    void onClick_pbRemovePaymentForm();

public:

    Tools tools;

    QWidget *wUp;

    QStackedWidget *swPanels;

    QWidget *wViewValues;
    QLabel *lViewBg;

    QLabel *lTotalStatic;
    QLabel *lTotal;

    QLabel *lPaidStatic;
    QLabel *lPaid;

    QLabel *lTransshipmentStatic;
    QLabel *lTransshipment;

    Label *lClient;

    QWidget *wViewClient;

    QStackedWidget *swPanelsClient;

    QPushButton *pbSelectPeople;
    QPushButton *pbSelectBusiness;

    QWidget *wGroupClient;
    phoneEdit *ePhone;
    Edit *eCep;
    Edit *eName;
    Edit *eStreet;
    Edit *eNumber;
    Edit *eComplement;
    Edit *eDistrict;
    DoubleSpinBox *DSBShipping;
    QTextEdit *teNotes;
    QPushButton *pbLeave;

    QWidget *wGroupBusiness;
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
    QPushButton *pbLeaveBusiness;

    QWidget *wPaymentForm;
    QLabel *lPaymentForm;

    ComboBox *cbPaymentForm;
    DoubleSpinBox *DSBValuePaymentForm;
    QPushButton *pbAddPaymentForm;
    QPushButton *pbRemovePaymentForm;
    TableView *tablePaymentForm;

    DataModel *dataModelPayment;

    Order *order;

    int idClient;

    int typeClient;

    QList<QString> dataClientI;
    QList<QString> dataClientF;

    double totalValueOrder;

    double totalPaid;

    enum{
        NORMAL = 1,
        BUSINESS = 2
    };

    void updateModelTable();

    void updateTotalPaid();

    bool canAddPaymentForm;

    bool canAddPaymentFormBecauseIsChidren;

    usint yourFather;
};

#endif // PAYMENTVIEW_H

