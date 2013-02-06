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


 #ifndef TREATMENTINTERFACE_H
#define TREATMENTINTERFACE_H

#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <Interface/deliveryinterface.h>
#include <Interface/counterinterface.h>
#include <Interface/tableinterface.h>
#include <QKeyEvent>

class TreatmentInterface : public QWidget
{
    Q_OBJECT
public:
    explicit TreatmentInterface(QWidget *parent = 0);
    void createComponent();    
    void resizeEvent(QResizeEvent *);
    void connections();

    void setIdOrderEdit(int type, int id);

public:
    int idOrderEdit;

signals:
    void setInput();
    void toPanel(int index);
    void editOrder(int type, int idOrder);

    void fullScreen();

    void goToPanel(int);
    
public slots:
    void adjustLayout();
    void input();

    void keyPressEvent(QKeyEvent *event);

    void onClick_pbDelivery();
    void onClick_pbTable();
    void onClick_pbCounter();
    void onClick_pbOrdered();

    void logout();

public:
    DeliveryInterface * wDelivery;
    CounterInterface *wCounter;
    TableInterface *wTable;


    QPushButton * pbDelivery;
    QPushButton * pbTable;
    QPushButton * pbCounter;
    QPushButton * pbOrdered;

    QStackedWidget *swPanel;

    QLabel *lBackground;
};

#endif // TREATMENTINTERFACE_H
