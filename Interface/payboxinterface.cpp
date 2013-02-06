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


 #include "payboxinterface.h"
#include <QMessageBox>
#include <QDebug>

PayBoxInterface::PayBoxInterface(QWidget *parent) :
    QWidget(parent)
{
    createComponent();
    connections();
    statics();

    isAdd = true;

    typeS = DESCRIPTION;
}


void PayBoxInterface::createComponent()
{
    cash = new MovementCash();

    lTop = new QLabel(this);

    lAdd = new Label(this,true,100);
    lRemove = new Label(this,true,100);
    lOpenBox = new Label(this,true,100);
    lCloseBox = new Label(this,true,100);

    textValuePayBox = new QTextBrowser(this);

    pbBill = new QPushButton(this);

    wSeletorDate =  new SeletorDate(this);

    wSearchPayBox = new Search(this,tr("Pesquisar"));
    actionDate = new QAction(tr("Pesquisar por Data"),this);
    actionHour = new QAction(tr("Pesquisar por Hora"),this);
    actionDescrition = new QAction(tr("Pesquisar por Descrição"),this);

    wSpace = new QWidget(this);
    lBackgroundSpace = new QLabel(wSpace);

    wPlus = new QWidget(wSpace);
    lPlus = new QLabel(wPlus);

    DSBValue = new DoubleSpinBox(wPlus);
    cbTypeValue = new ComboBox(wPlus);

    eDate = new Edit(wPlus,true,tr("Data"));
    eHour = new Edit(wPlus,true,tr("Hora"));
    teNotes = new QTextEdit(wPlus);

    pbConfirm = new QPushButton(wPlus);
    pbLeave = new QPushButton(wPlus);

    wOpenBox = new QWidget(wSpace);
    lBgOpenBox = new QLabel(wOpenBox);

    DSBValueOpenCash = new DoubleSpinBox(wOpenBox);
    eDateOpenBox = new Edit(wOpenBox,true,tr("Data"));
    eHourOpenBox = new Edit(wOpenBox,true,tr("Hora"));
    pbConfirmOpenBox = new QPushButton(wOpenBox);
    pbLeaveOpenBox = new QPushButton(wOpenBox);

    wCloseBox = new QWidget(wSpace);
    lBgCloseBox = new QLabel(wCloseBox);
    lCloseValue = new QLabel(wCloseBox);
    eDateCloseBox = new Edit(wCloseBox,true,tr("Data"));
    eHourCloseBox = new Edit(wCloseBox,true,tr("Hora"));
    pbConfirmCloseBox = new QPushButton(wCloseBox);
    pbLeaveCloseBox = new QPushButton(wCloseBox);

    tablePayBox = new TableView(this);

    swPanels = new QStackedWidget(wSpace);

    splitter = new QSplitter(Qt::Vertical,this);

    wBetweenDate = new BetweenDate(this);
}

void PayBoxInterface::input()
{
    sizePanel = 280;

    lTop->setPixmap(QPixmap(":/subMenu/background"));
    lTop->setScaledContents(true);

    lAdd->setImage(":/subMenu/buttonAdd");
    lAdd->setHover(":/subMenu/buttonAdd-hover");
    lAdd->setAlternateImage(lAdd->imageHover);
    lAdd->setToolTip(tr("Adicionar"));

    lRemove->setImage(":/subMenu/buttonRemove");
    lRemove->setHover(":/subMenu/buttonRemove-hover");
    lRemove->setAlternateImage(lRemove->imageHover);
    lRemove->setToolTip(tr("Remover cliente selecionado"));

    lOpenBox->setImage(":/subMenu/buttonOpenBox");
    lOpenBox->setHover(":/subMenu/buttonOpenBox-hover");
    lOpenBox->setAlternateImage(lOpenBox->imageHover);
    lOpenBox->setToolTip(tr("Abrir o Caixa"));

    lCloseBox->setImage(":/subMenu/buttonCloseBox");
    lCloseBox->setHover(":/subMenu/buttonCloseBox-hover");
    lCloseBox->setAlternateImage(lCloseBox->imageHover);
    lCloseBox->setToolTip(tr("Fechar o caixa"));

    pbBill->setText("Contas");
    pbBill->setCursor(Qt::PointingHandCursor);
    pbBill->setFont(this->font());    
    pbBill->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                             "stop: 1 rgba(165, 47, 31, 240), stop: 0 rgba(159, 45, 29, 240));"
                             "color: rgb(230,230,230);"
                             "border: 1px solid rgba(123, 31, 31, 255);"
                             "border-radius: 5px;");

    QFont f;

#if defined(Q_WS_X11)
    f = QFont("Sans Serif",9);
#endif

#if defined(Q_WS_WIN)
    f = QFont("MS Shell Dlq 2", 11);
#endif

    f.setBold(true);

    qDebug() << "imput";

    textValuePayBox->setText(totalValueUpdate);
    textValuePayBox->setObjectName("textValuePayBox");
    textValuePayBox->setToolTip(tr("Valor do caixa"));
    textValuePayBox->setFont(f);
    textValuePayBox->setAlignment(Qt::AlignCenter);
    textValuePayBox->setStyleSheet(this->styleSheet());

    wSearchPayBox->setAction(actionDescrition);
    wSearchPayBox->setAction(actionDate);
    wSearchPayBox->setAction(actionHour);

    actionDate->setCheckable(true);
    actionDescrition->setCheckable(true);
    actionHour->setCheckable(true);

    actionDate->setEnabled(true);
    actionDate->setChecked(false);

    actionHour->setEnabled(true);
    actionHour->setChecked(false);

    actionDescrition->setEnabled(false);
    actionDescrition->setChecked(true);

    lBackgroundSpace->setStyleSheet("background-color: rgba(123, 31, 31, 255);");

    splitter->insertWidget(1,wSpace);
    splitter->insertWidget(2,tablePayBox);
    splitter->show();
    splitter->setHandleWidth(1);
    splitter->setStretchFactor(1,2);

    swPanels->addWidget(wPlus);
    swPanels->addWidget(wOpenBox);
    swPanels->addWidget(wCloseBox);

    wSpace->setGeometry(0,0,0,0);

    lPlus->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.498,"
                         "y1:0, x2:0.494636, y2:1, stop:0 rgba(255, 255, 255, 210),"
                         "stop:0.982955 rgba(255, 255, 255, 170));"
                         "border-radius: 5px;border: 1px solid rgba(255,255,255,255);");

    DSBValue->setPrefix(tr("Valor R$ "));
    DSBValue->setMaximum(99999999);

    cbTypeValue->add(tr("Movimentação"));
    cbTypeValue->add(tr("Entrada"));
    cbTypeValue->add(tr("Saída"));

    eDate->setInputMask("  /  /    ");
    eHour->setInputMask("  :  ");

    teNotes->setStyleSheet("background-color: qlineargradient"
                           "(spread:pad, x1:0, y1:1, x2:0.165045,"
                           "y2:0, stop:0 rgba(254, 255, 180, 255),"
                           "stop:0.721591 rgba(255, 250, 205, 255));"
                           "border-radius: 10px;border: 1px solid #C0C0C0;");

    pbConfirm->setText(tr("Confirmar"));
    pbLeave->setText(tr("Limpar"));

    lBgOpenBox->setStyleSheet(lPlus->styleSheet());

    DSBValueOpenCash->setPrefix(tr("Valor R$ "));
    DSBValueOpenCash->setMaximum(999999999);

    eDateOpenBox->setInputMask("  /  /    ");
    eHourOpenBox->setInputMask("  :  ");

    pbConfirmOpenBox->setText(tr("Confirmar"));
    pbLeaveOpenBox->setText(tr("Limpar"));

    lBgCloseBox->setStyleSheet(lPlus->styleSheet());

    f.setPointSize(f.pointSize() + 3);
    f.setBold(false);
    lCloseValue->setAlignment(Qt::AlignCenter);
    lCloseValue->setStyleSheet("color: rgba(0,0,0,100);");
    lCloseValue->setFont(f);
    lCloseValue->setText(tr("R$ 0,00"));

    eDateCloseBox->setInputMask("  /  /    ");
    eHourCloseBox->setInputMask("  :  ");

    pbConfirmCloseBox->setText(tr("Confirmar"));
    pbLeaveCloseBox->setText(tr("Limpar"));

    DSBValue->setNextComponent(cbTypeValue);
    cbTypeValue->setNextComponent(eDate);
    eDate->setNextComponent(eHour);
    eHour->setNextComponent(teNotes);

    DSBValueOpenCash->setNextComponent(eDateOpenBox);
    eDateOpenBox->setNextComponent(eHourOpenBox);

    eDateCloseBox->setNextComponent(eHourCloseBox);

    tablePayBox->setModel(cash->bd.getModel("movementCash"));

    wBetweenDate->hide();

    eDateOpenBox->setEnabled(false);
    eDateCloseBox->setEnabled(false);

    eHourOpenBox->setEnabled(false);
    eHourCloseBox->setEnabled(false);
}


void PayBoxInterface::connections()
{
    connect(lAdd, SIGNAL(onClick()), SLOT(onClick_lAdd()));

    connect(lRemove, SIGNAL(onClick()), SLOT(onClick_lRemove()));

    connect(lOpenBox, SIGNAL(onClick()), SLOT(onClick_lOpenBox()));

    connect(lCloseBox, SIGNAL(onClick()), SLOT(onClick_lCloseBox()));

    connect(pbBill,SIGNAL(clicked()),SLOT(onClick_pbAccount()));

    connect(splitter, SIGNAL(splitterMoved(int, int)), SLOT(adjustLayout()));

    connect(swPanels, SIGNAL(currentChanged(int)), SLOT(adjustLayout()));

    connect(pbConfirm, SIGNAL(clicked()), SLOT(onClick_pbConfirm()));

    connect(pbLeave, SIGNAL(clicked()), SLOT(onClick_pbLeave()));

    connect(pbConfirmOpenBox, SIGNAL(clicked()), SLOT(onClick_pbConfirmOpenBox()));

    connect(pbConfirmCloseBox, SIGNAL(clicked()), SLOT(onClick_pbConfirmCloseBox()));

    connect(pbLeaveOpenBox, SIGNAL(clicked()), SLOT(onClick_pbLeaveOpenBox()));

    connect(pbLeaveCloseBox, SIGNAL(clicked()), SLOT(onClick_pbLeaveCloseBox()));

    connect(cash, SIGNAL(totalValue(QString)), SLOT(updateTotalValue(QString)));

    connect(tablePayBox, SIGNAL(doubleClicked(QModelIndex)), SLOT(doubleClicked_tablePayBox(QModelIndex)));

    connect(actionDate,SIGNAL(triggered()),this,SLOT(onTriggered_actionDate()));

    connect(actionHour,SIGNAL(triggered()),this,SLOT(onTriggered_actionHour()));

    connect(actionDescrition,SIGNAL(triggered()),this,SLOT(onTriggered_actionDescrition()));

    connect(eHourOpenBox, SIGNAL(returnPressed()), SLOT(onClick_pbConfirmOpenBox()));

    connect(eHourCloseBox, SIGNAL(returnPressed()), SLOT(onClick_pbConfirmCloseBox()));

    connect(wSeletorDate,SIGNAL(selected(int)),SLOT(showBetweenDate(int)));

    connect(wBetweenDate, SIGNAL(selectedDates(QString,QString)), SLOT(selectedDates(QString, QString)));

    connect(wSeletorDate, SIGNAL(selected(int)), SLOT(seletorDate(int)));

    connect(wSearchPayBox, SIGNAL(textSearch(QString)), SLOT(getSearchPayBox(QString)));
}

void PayBoxInterface::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void PayBoxInterface::adjustLayout()
{
    lTop->setGeometry(0,0,this->width(),50);

    wSearchPayBox->setGeometry(this->width() - 220,11,200,28);

    textValuePayBox->setGeometry(lCloseBox->width() + lCloseBox->x() + 12,11,textValuePayBoxSize(),28);

    pbBill->setGeometry(textValuePayBox->width() + textValuePayBox->x() + 12,11,50,28);

    if(pbBill->isVisible())
    {
        if(pbBill->x() + pbBill->width() < this->width() / 2 - 140)
            wSeletorDate->setGeometry(this->width() / 2 - 140,11,280,28);
        else
            wSeletorDate->setGeometry(pbBill->width() + pbBill->x() + 12,11,280,28);
    }
    else
    {
        if(textValuePayBox->x() + textValuePayBox->width() < this->width() / 2 - 140)
            wSeletorDate->setGeometry(this->width() / 2 - 140,11,280,28);
        else
            wSeletorDate->setGeometry(textValuePayBox->width() + textValuePayBox->x() + 12,11,280,28);
    }

    splitter->setGeometry(0,lTop->height(),this->width(),this->height() - lTop->height());

    wSpace->setGeometry(0,0,this->width(),wSpace->height());

    swPanels->setGeometry(0,0,wSpace->width(), wSpace->height());

    lBackgroundSpace->setGeometry(0,0,wSpace->width(),wSpace->height());

    wPlus->setGeometry(wSpace->width()/2 - 300,wSpace->height()/2 - 113,600, 225);

    wOpenBox->setGeometry(wSpace->width()/2 - 150, wSpace->height()/2 - 100,300,200);

    wCloseBox->setGeometry(wSpace->width()/2 - 150, wSpace->height()/2 - 100,300,200);

    tablePayBox->setGeometry(0,wSpace->y() + wSpace->height(),this->width(),
                             this->height() - (wSpace->height() + lTop->height()));

    tablePayBox->setColumnHidden(0,true);

    tablePayBox->setColumnWidth(1,tablePayBox->width() * 0.15);
    tablePayBox->setColumnWidth(2,tablePayBox->width() * 0.15);
    tablePayBox->setColumnWidth(3,tablePayBox->width() * 0.15);
    tablePayBox->setColumnWidth(4,tablePayBox->width() * 0.15);
    tablePayBox->setColumnWidth(5,tablePayBox->width() * 0.4);

    wBetweenDate->setGeometry((wSeletorDate->x() + wSeletorDate->pbPeriod->width() * 4 + wSeletorDate->pbPeriod->width()/2) - wBetweenDate->W_/2,
                              wSeletorDate->y() + wSeletorDate->height() + 3,wBetweenDate->W_,wBetweenDate->H_);
}

void PayBoxInterface::adjustSplitter()
{
    QList<int> list;

    list << wSpace->height() << tablePayBox->height();

    splitter->setSizes(list);

    adjustLayout();
}

void PayBoxInterface::hidePanel(bool b)
{
    if(b)
    {
        animaUp = panelEffects->createAnimation(wSpace,QRect(wSpace->x(),
                                                             wSpace->y(), wSpace->width(), 0),800,QEasingCurve::InBack);

        animaDown = panelEffects->createAnimation(tablePayBox,QRect(tablePayBox->x(),
                                                                    0, tablePayBox->width(), this->height() - lTop->height()),800,QEasingCurve::InBack);
    }
    else
    {
        animaDown  = panelEffects->createAnimation(wSpace,QRect(wSpace->x(),
                                                                wSpace->y(), wSpace->width(), sizePanel),800,QEasingCurve::OutBack);

        animaUp  = panelEffects->createAnimation(tablePayBox,QRect(tablePayBox->x(),sizePanel,
                                                                   tablePayBox->width(),this->height() - (lTop->height() + sizePanel)),800,QEasingCurve::OutBack);
    }

    connect(animaDown,SIGNAL(valueChanged(QVariant)),SLOT(adjustSplitter()));

    connect(animaUp,SIGNAL(valueChanged(QVariant)),SLOT(adjustSplitter()));

    animaUp->start();

    animaDown->start();
}

void PayBoxInterface::gotoPanel(int index)
{
    bool b = (wSpace->height() > 0)? true:false;

    nextPanelIndex = index;

    if(index == swPanels->currentIndex())
    {
        hidePanel(b);
    }
    else
    {
        if(b)
        {
            hidePanel(b);

            connect(animaUp,SIGNAL(finished()),SLOT(nextPanel()));
        }
        else
        {
            swPanels->setCurrentIndex(index);

            hidePanel(false);
        }
    }

    switch(index)
    {
    case 0:{
        lAdd->setAlternate(!b);
        break;
    }

    case 1:{
        lOpenBox->setAlternate(!b);
        break;
    }

    case 2:{
        lCloseBox->setAlternate(!b);
        break;
    }
    }
}

void PayBoxInterface::onClick_lAdd()
{
    if (cash->canCloseCash()){
        eDate->setText(tools.getDate());

        eHour->setText(tools.getTime("hh:mm"));

        DSBValue->setFocus();

        gotoPanel(0);
    }else{
        int op = QMessageBox::question(this, tr("Operação bloqueada"), tr("Essa operação não pode ser realizada pois o caixa ainda não foi aberto.\nDeseja abrir o caixa agora?"), tr("Sim"), tr("Não"));

        if (op == 0){
            gotoPanel(1);
        }
    }
}

void PayBoxInterface::onClick_lRemove()
{
    QModelIndex index = tablePayBox->currentIndex();

    if (index.row() < 0)
        return; // caso não haja nada selecionado ou a tabela esteja vazia

    int op = QMessageBox::question(this, tr("Deletar movimentação"), tr("Deseja deletar a movimentação?"), tr("Sim"), tr("Não"));

    if (op == 0)
        cash->remove(tablePayBox->model()->index(index.row(), 0).data().toInt());
}

void PayBoxInterface::statics()
{
    lAdd->setGeometry(12,11,30,28);

    lRemove->setGeometry(lAdd->width() + lAdd->x() + 12,11,30,28);

    lOpenBox->setGeometry(lRemove->width() + lRemove->x() + 12,11,30,28);

    lCloseBox->setGeometry(lOpenBox->width() + lOpenBox->x() + 12,11,30,28);

    lPlus->setGeometry(0,0,600,225);

    DSBValue->setGeometry(25,20,170,28);

    cbTypeValue->setGeometry(25,DSBValue->y() + DSBValue->height() + 10,170,28);

    eDate->setGeometry(25,cbTypeValue->y() + cbTypeValue->height() + 10,170,28);

    eHour->setGeometry(25,eDate->y() + eDate->height() + 10,170,28);

    teNotes->setGeometry(210,20,370,180);

    pbConfirm->setGeometry(25,eHour->y() + eHour->height() + 10,83,28);

    pbLeave->setGeometry(pbConfirm->x() + pbConfirm->width() + 4,pbConfirm->y(),83,28);

    lBgOpenBox->setGeometry(0,0,300,200);

    DSBValueOpenCash->setGeometry(60,25,180,28);

    eDateOpenBox->setGeometry(60,DSBValueOpenCash->y() + DSBValueOpenCash->height() + 10,180,28);

    eHourOpenBox->setGeometry(60,eDateOpenBox->y() + eDateOpenBox->height() + 10,180,28);

    pbConfirmOpenBox->setGeometry(65,eHourOpenBox->y() + eHourOpenBox->height() + 10,80,28);

    pbLeaveOpenBox->setGeometry(pbConfirmOpenBox->x() + pbConfirmOpenBox->width() + 10,pbConfirmOpenBox->y(), 80,28);

    lBgCloseBox->setGeometry(0,0,300,200);

    lCloseValue->setGeometry(60,25,180,28);

    eDateCloseBox->setGeometry(60,lCloseValue->y() + lCloseValue->height() + 10,180,28);

    eHourCloseBox->setGeometry(60,eDateCloseBox->y() + eDateCloseBox->height() + 10,180,28);

    pbConfirmCloseBox->setGeometry(65,eHourCloseBox->y() + eHourCloseBox->height() + 10,80,28);

    pbLeaveCloseBox->setGeometry(pbConfirmCloseBox->x() + pbConfirmCloseBox->width() + 10,pbConfirmCloseBox->y(), 80,28);

}

void PayBoxInterface::onClick_pbAccount()
{
    emit gotoAccount(0);
}

void PayBoxInterface::nextPanel()
{
    disconnect(animaUp,SIGNAL(finished()),this,SLOT(nextPanel()));

    switch(swPanels->currentIndex())
    {
    case 0:lAdd->setAlternate(false);
        break;

    case 1:lOpenBox->setAlternate(false);
        break;

    case 2:lCloseBox->setAlternate(false);
        break;
    }

    switch(nextPanelIndex)
    {
    case 0:lAdd->setAlternate(true);
        break;

    case 1:lOpenBox->setAlternate(true);
        break;

    case 2:lCloseBox->setAlternate(true);
        break;
    }

    hidePanel(false);

    swPanels->setCurrentIndex(nextPanelIndex);
}

void PayBoxInterface::onClick_lOpenBox()
{
    if (cash->canOpenCash()){
        DSBValueOpenCash->setValue(0);

        eDateOpenBox->setText(tools.getDate());

        eHourOpenBox->setText(tools.getTime("hh:mm"));

        gotoPanel(1);

    }else{
        int op = QMessageBox::question(this, tr("Operação bloqueada"), tr("Essa operação não pode ser realizada pois o caixa já foi aberto.\nDeseja fechar o caixa agora?"), tr("Sim"), tr("Não"));

        if (op == 0){
            onClick_lCloseBox();
        }
    }
}

void PayBoxInterface::onClick_lCloseBox()
{
    if (cash->canCloseCash()){
        eDateCloseBox->setText(tools.getDate());

        eHourCloseBox->setText(tools.getTime("hh:mm"));

        QString valueClose = cash->valueCloseBox(eDateCloseBox->text(), eHourCloseBox->text());

        lCloseValue->setText(tools.convertMoney(valueClose));

        gotoPanel(2);
    }else{
        int op = QMessageBox::question(this, tr("Operação bloqueada"), tr("Essa operação não pode ser realizada pois o caixa ainda não foi aberto.\nDeseja abrir o caixa agora?"), tr("Sim"), tr("Não"));

        if (op == 0){
            onClick_lOpenBox();
        }
    }
}


void PayBoxInterface::onClick_pbConfirm()
{
    if (isAvailable()){
        cash->value = DSBValue->value();

        cash->type = cbTypeValue->currentText();

        cash->date = eDate->text();

        cash->hour = eHour->text();

        cash->note = teNotes->toPlainText();

        if (isAdd){
            if (! cash->insert()){
                QMessageBox::warning(this, tr("Movimentação não inserida"), tr("A movimentação não pode ser inserida\nVerifique os dados."), QMessageBox::Ok);

                return;
            }
        }else{
            if (! cash->update(idUpdate)){
                QMessageBox::warning(this, tr("Movimentação não alterada"), tr("A movimentação não pode ser alterada\nVerifique os dados."), QMessageBox::Ok);

                return;
            }
        }

        clearForm();
    }
}

void PayBoxInterface::onClick_pbLeave()
{
        clearForm();
}

void PayBoxInterface::onClick_pbConfirmOpenBox()
{
    if (isAvailableOpenBox()){
        // verifica se pode realmente abrir o caixa
        cash->value = DSBValueOpenCash->value();

        cash->type = tr("Abertura");

        cash->date = eDateOpenBox->text();

        cash->hour = eHourOpenBox->text();

        cash->note = tr("Abertura de caixa");

        if (isAdd){
            if (! cash->insert()){
                QMessageBox::warning(this, tr("Abertura não realizada"), tr("A abertura de caixa não pode ser feita\nVerifique os dados."), QMessageBox::Ok);

                return;
            }
        }else{
            if (! cash->update(idUpdate)){
                QMessageBox::warning(this, tr("Abertura não alterada"), tr("A abertura de caixa não pode ser alterada\nVerifique os dados."), QMessageBox::Ok);

                return;
            }
        }

        gotoPanel(1);

        clearForm();
    }

}

void PayBoxInterface::onClick_pbLeaveOpenBox()
{
    clearFormOpenBox();
}

void PayBoxInterface::onClick_pbConfirmCloseBox()
{
    if (isAvailableCloseBox()){
        // verifica se pode realmente fechar o caixa
        if (cash->canCloseCash()){

            cash->type = tr("Fechamento");

            cash->date = eDateCloseBox->text();

            cash->hour = eHourCloseBox->text();

            cash->value = cash->valueCloseBox(cash->date, cash->hour).toFloat();

            cash->note = tr("Fechamento de caixa");

            if (isAdd){
                if (! cash->insert()){
                    QMessageBox::warning(this, tr("Fechamento não realizado"), tr("O fechamento de caixa não pode ser feito\nVerifique os dados."), QMessageBox::Ok);

                    return;
                }
            }else{
                if (! cash->update(idUpdate)){
                    QMessageBox::warning(this, tr("Fechamento não alterado"), tr("O fechamento de caixa não pode ser alterado\nVerifique os dados."), QMessageBox::Ok);

                    return;
                }
            }

            gotoPanel(2);

            clearForm();

        }else{
            QMessageBox::warning(this, tr("Caixa não fechado"), tr("O caixa não pode ser fechado na data especificada por já estar fechado\nAbra o caixa antes de realizar esta operação"), QMessageBox::Ok);
        }
    }
}

void PayBoxInterface::onClick_pbLeaveCloseBox()
{
    clearFormCloseBox();
}

bool PayBoxInterface::isAvailable()
{
    return true;
}

bool PayBoxInterface::isAvailableOpenBox()
{
    if (DSBValueOpenCash->value() == 0){
        int op = QMessageBox::question(this, tr("Caixa vazio"), tr("Tem certeza que deseja abrir o caixa com valor zero?"), tr("Sim"), tr("Não"));

        if (op == 1){
            DSBValueOpenCash->setFocus();

            return false;
        }
    }

    if (! eDateOpenBox->isEmpty())
        if (! tools.isValidDate(eDateOpenBox->text())){
            QMessageBox::warning(this, tr("Data inválida"), tr("Por favor, digite uma data válida"), QMessageBox::Ok);

            eDateOpenBox->setFocus();

            return false;
        }

    if (! eHourOpenBox->isEmpty())
        if (! tools.isValidHour(eHourOpenBox->text())){
            QMessageBox::warning(this, tr("Hora inválida"), tr("Por favor, digite uma hora válida"), QMessageBox::Ok);

            eHourOpenBox->setFocus();

            return false;
        }

    return true;
}

bool PayBoxInterface::isAvailableCloseBox()
{
    if (! eDateCloseBox->isEmpty())
        if (! tools.isValidDate(eDateCloseBox->text())){
            QMessageBox::warning(this, tr("Data inválida"), tr("Por favor, digite uma data válida"), QMessageBox::Ok);

            eDateCloseBox->setFocus();

            return false;
        }

    if (! eHourCloseBox->isEmpty())
        if (! tools.isValidHour(eHourCloseBox->text())){
            QMessageBox::warning(this, tr("Hora inválida"), tr("Por favor, digite uma hora válida"), QMessageBox::Ok);

            eHourCloseBox->setFocus();

            return false;
        }

    return true;
}

void PayBoxInterface::updateTotalValue(QString value)
{    
    QFont f;

#if defined(Q_WS_X11)
    f = QFont("Sans Serif",9);
#endif

#if defined(Q_WS_WIN)
    f = QFont("MS Shell Dlq 2", 11);
#endif

    f.setBold(true);

    totalValueUpdate = tools.convertMoney(value);

    textValuePayBox->setText(tools.convertMoney(value));
    textValuePayBox->setFont(f);
    textValuePayBox->setAlignment(Qt::AlignHCenter);

    adjustLayout();
}

void PayBoxInterface::clearForm()
{
    isAdd = true;

    DSBValue->setValue(0);

    cbTypeValue->setCurrentIndex(0);

    eDate->Clear();

    eHour->Clear();

    teNotes->clear();

    pbConfirm->setText(tr("Confirmar"));

    pbLeave->setText(tr("Limpar"));
}

void PayBoxInterface::clearFormOpenBox()
{
    DSBValueOpenCash->setValue(0);

    eDateOpenBox->setText(tools.getDate());

    eHourOpenBox->setText(tools.getTime("hh:mm"));

    pbConfirmOpenBox->setText(tr("Confirmar"));

    pbLeaveOpenBox->setText(tr("Limpar"));
}

void PayBoxInterface::clearFormCloseBox(){
    QString date = tools.currentYear() + "-" + tools.currentMonth2() + "-" + tools.currentDay();

    lCloseValue->setText(tools.convertMoney(cash->valueCloseBox(date, tools.getTime("hh:mm"))));

    eDateCloseBox->setText(tools.getDate());

    eHourCloseBox->setText(tools.getTime("hh:mm"));

    pbConfirmCloseBox->setText(tr("Confirmar"));

    pbLeaveCloseBox->setText(tr("Limpar"));
}

void PayBoxInterface::doubleClicked_tablePayBox(QModelIndex index)
{
    if (index.row() < 0)
        return;

    idUpdate = tablePayBox->model()->index(index.row(), 0).data().toInt();

    DSBValue->setValue(tablePayBox->model()->index(index.row(), 1).data().toDouble());

    if (tablePayBox->model()->index(index.row(), 1).data().toString() == "Entrada")
        cbTypeValue->setCurrentIndex(1);
    else
        cbTypeValue->setCurrentIndex(2);

    eDate->setText(tools.convertDate2(tablePayBox->model()->index(index.row(), 3).data().toString()));

    eHour->setText(tablePayBox->model()->index(index.row(), 4).data().toString());

    teNotes->setText(tablePayBox->model()->index(index.row(), 5).data().toString());

    isAdd = false;

    pbConfirm->setText(tr("Salvar"));

    pbLeave->setText(tr("Cancelar"));
}

void PayBoxInterface::onTriggered_actionDate()
{
    actionDate->setEnabled(false);
    actionDate->setChecked(true);

    actionHour->setEnabled(true);
    actionHour->setChecked(false);

    actionDescrition->setEnabled(true);
    actionDescrition->setChecked(false);

    typeS = DATE;

    wSearchPayBox->eSearch->Clear();

    wSearchPayBox->eSearch->setInputMask("  /  /    ");
}

void PayBoxInterface::onTriggered_actionHour()
{
    actionDate->setEnabled(true);
    actionDate->setChecked(false);

    actionHour->setEnabled(false);
    actionHour->setChecked(true);

    actionDescrition->setEnabled(true);
    actionDescrition->setChecked(false);

    typeS = TIME;

    wSearchPayBox->eSearch->Clear();

    wSearchPayBox->eSearch->setInputMask("  :  ");
}

void PayBoxInterface::onTriggered_actionDescrition()
{
    actionDate->setEnabled(true);
    actionDate->setChecked(false);

    actionHour->setEnabled(true);
    actionHour->setChecked(false);

    actionDescrition->setEnabled(false);
    actionDescrition->setChecked(true);

    typeS = DESCRIPTION;

    wSearchPayBox->eSearch->Clear();

    wSearchPayBox->eSearch->clearMask();
}

int PayBoxInterface::textValuePayBoxSize()
{
    short int pointSize;

#if defined(Q_WS_X11)
    pointSize = 9;
#endif

#if defined(Q_WS_WIN)
    pointSize = 11;
#endif

    int value = 12 + textValuePayBox->toPlainText().length() * pointSize;

    return value;
}

void PayBoxInterface::showBetweenDate(int option)
{
    if (option == 4)
    {
        if(wBetweenDate->isVisible())
            wBetweenDate->hide();
        else
            wBetweenDate->show();

        wBetweenDate->setGeometry((wSeletorDate->x() + wSeletorDate->pbPeriod->width() * 4 + wSeletorDate->pbPeriod->width()/2) - wBetweenDate->W_/2,
                                  wSeletorDate->y() + wSeletorDate->height() + 3,wBetweenDate->W_,wBetweenDate->H_);
    }
    else
        wBetweenDate->hide();
}

void PayBoxInterface::seletorDate(int i)
{
    /*
        0 - dia
        1 - semana
        2 - mês
        3 - ano
        4 - período específico (ver como pegar período depois)
    */

    switch (i){

    case 0:{
        cash->getCashDays();

        wSeletorDate->pbWeek->setToolTip("");
        wSeletorDate->pbMonth->setToolTip("");
        wSeletorDate->pbYear->setToolTip("");
        wSeletorDate->pbPeriod->setToolTip("");

        wSeletorDate->pbToday->setToolTip(QString("Busca para hoje:\n%1").arg(cash->dateIFilter));

        break;
    }

    case 1:{
        cash->getCashWeek();

        wSeletorDate->pbToday->setToolTip("");
        wSeletorDate->pbMonth->setToolTip("");
        wSeletorDate->pbYear->setToolTip("");
        wSeletorDate->pbPeriod->setToolTip("");

        wSeletorDate->pbWeek->setToolTip(QString("Busca entre as datas:\n%1 e \n%2").arg(cash->dateIFilter, cash->dateFFilter));

        break;
    }

    case 2:{
        cash->getCashMonth();

        wSeletorDate->pbToday->setToolTip("");
        wSeletorDate->pbWeek->setToolTip("");
        wSeletorDate->pbYear->setToolTip("");
        wSeletorDate->pbPeriod->setToolTip("");

        wSeletorDate->pbMonth->setToolTip(QString("Busca entre as datas:\n%1 e \n%2").arg(cash->dateIFilter, cash->dateFFilter));

        break;
    }

    case 3:{
        cash->getCashYear();

        wSeletorDate->pbToday->setToolTip("");
        wSeletorDate->pbWeek->setToolTip("");
        wSeletorDate->pbMonth->setToolTip("");
        wSeletorDate->pbPeriod->setToolTip("");

        wSeletorDate->pbYear->setToolTip(QString("Busca entre as datas:\n%1 e \n%2").arg(cash->dateIFilter, cash->dateFFilter));

        break;
    }

    case 4:{

        break;
    }

    }
}

void PayBoxInterface::selectedDates(QString dateI, QString dateF)
{
    cash->getCashCustom(dateI, dateF);

    wSeletorDate->pbToday->setToolTip("");
    wSeletorDate->pbWeek->setToolTip("");
    wSeletorDate->pbMonth->setToolTip("");
    wSeletorDate->pbYear->setToolTip("");

    wSeletorDate->pbPeriod->setToolTip(QString("Busca entre as datas:\n%1 e \n%2").arg(dateI, dateF));

    wBetweenDate->hide();
}

void PayBoxInterface::getSearchPayBox(QString text)
{
    if (text.length() == 0){
        cash->setFilter();

        return;
    }

    QString filter;
    switch (typeS){
    case DESCRIPTION:
    {
        filter = QString("lower(note) like lower('%1%')").arg(text);

        cash->setFilter(filter);

        break;
    }

    case DATE:
    {
        if (text.length() == 10){
            filter = QString("datemovement = '%1'").arg(text);

            cash->setFilter(filter);
        }else{
            cash->setFilter();
        }

        break;
    }

    case TIME:
    {
        if (text.length() == 5){
            filter = QString("hourmovement = '%1'").arg(text + ":00");

            cash->setFilter(filter);
        }else
            cash->setFilter();

        break;
    }

    }
}

void PayBoxInterface::keyPressEvent(QKeyEvent *event)
{
    switch (event->modifiers()){

    case Qt::ControlModifier:{

        if (event->key() == Qt::Key_Tab)
            emit goToPanel(11);

        break;
    }
    }

    if (event->key() == Qt::Key_Backtab)
        emit goToPanel(9);

    if (event->key() == Qt::Key_F11)
        emit fullScreen();
}

void PayBoxInterface::logout()
{
    onClick_pbLeave();

    onClick_pbLeaveOpenBox();

    onClick_pbLeaveCloseBox();
}
