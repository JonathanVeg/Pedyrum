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


 #include "accountfinanceinterface.h"
#include <QList>
#include <QDebug>
#include <QMessageBox>
#include <QDebug>

AccountFinanceInterface::AccountFinanceInterface(QWidget *parent) :
    QWidget(parent)
{
    createComponent();
    statics();
    connections();

    isAdd = true;
}

void AccountFinanceInterface::createComponent()
{
    bill = new Bill();

    lTop = new QLabel(this);

    lAdd = new Label(this,true,100);
    lRemove = new Label(this,true,100);

    textValueAccounts = new QTextBrowser(this);

    pbPayBox = new QPushButton(this);

    cbShowType = new ComboBox(this);

    wSeletorDate =  new SeletorDate(this);

    searchAccount = new Search(this,tr("Pesquisar"));
    actionName = new QAction(tr("Pesquisar por Nome"),this);
    actionDescription = new QAction(tr("Pesquisar por Descrição"),this);
    actionDatePay = new QAction(tr("Pesquisar por Data de Pagamento"),this);
    actionDateDeadLine = new QAction(tr("Pesquisar por Data de Vencimento"),this);

    tableAccount = new TableView(this);

    wSpace = new QWidget(this);
    lBackgroundSpace = new QLabel(wSpace);

    swPanels = new QStackedWidget(wSpace);

    wPlus = new QWidget(wSpace);
    lPlus = new QLabel(wPlus);

    eName = new Edit(wPlus,true,tr("Nome da conta"), 30);
    DSBValue = new DoubleSpinBox(wPlus);
    cbType = new ComboBox(wPlus);
    cbStatus = new ComboBox(wPlus);
    eDateDeadLine = new Edit(wPlus,true,tr("Data do vencimento"));
    eDatePay = new Edit(wPlus,true,tr("Data do pagamento"));
    teNotes = new QTextEdit(wPlus);

    pbConfirm = new QPushButton(wPlus);
    pbLeave = new QPushButton (wPlus);

    splitter = new QSplitter(Qt::Vertical,this);

    wBetweenDate = new BetweenDate(this);
}

void AccountFinanceInterface::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Delete){
        onClick_lRemove();
    }

    switch (event->modifiers()){

    case Qt::ControlModifier:{
        if (event->key() == Qt::Key_S)
            onClick_pbConfirm();

        if (event->key() == Qt::Key_L)
            onClick_pbLeave();

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

void AccountFinanceInterface::input()
{
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

    pbPayBox->setText("Caixa");
    pbPayBox->setCursor(Qt::PointingHandCursor);
    pbPayBox->setFont(this->font());
    pbPayBox->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                            "stop: 1 rgba(165, 47, 31, 240), stop: 0 rgba(159, 45, 29, 240));"
                            "color: rgb(230,230,230);"
                            "border: 1px solid rgba(123, 31, 31, 255);"
                            "border-radius: 5px;");

    cbShowType->add("Exibir Contas");
    cbShowType->add(tr("Todas as Entrada"));
    cbShowType->add(tr("Todas as Saída"));
    cbShowType->add(tr("Pago"));
    cbShowType->add(tr("Recebido"));
    cbShowType->add(tr("A pagar"));
    cbShowType->add(tr("A receber"));

    QFont f;

#if defined(Q_WS_X11)
    f = QFont("Sans Serif",9);
#endif

#if defined(Q_WS_WIN)
    f = QFont("MS Shell Dlq 2", 11);
#endif

    f.setBold(true);

    textValueAccounts->setText("R$ 0,00");
    textValueAccounts->setObjectName("textCountAccount");
    textValueAccounts->setToolTip(tr("Valor da Conta"));
    textValueAccounts->setFont(f);
    textValueAccounts->setAlignment(Qt::AlignCenter);
    textValueAccounts->setStyleSheet(this->styleSheet());

    searchAccount->setAction(actionName);
    searchAccount->setAction(actionDescription);
    searchAccount->setAction(actionDatePay);
    searchAccount->setAction(actionDateDeadLine);

    actionName->setCheckable(true);
    actionDescription->setCheckable(true);
    actionDatePay->setCheckable(true);
    actionDateDeadLine->setCheckable(true);

    actionName->setEnabled(false);
    actionName->setChecked(true);

    actionDescription->setEnabled(true);
    actionDescription->setChecked(false);

    actionDatePay->setEnabled(true);
    actionDatePay->setChecked(false);

    actionDateDeadLine->setEnabled(true);
    actionDateDeadLine->setChecked(false);

    lBackgroundSpace->setStyleSheet("background-color: rgba(123, 31, 31, 255);");

    lPlus->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.498,"
                         "y1:0, x2:0.494636, y2:1, stop:0 rgba(255, 255, 255, 210),"
                         "stop:0.982955 rgba(255, 255, 255, 170));"
                         "border-radius: 5px;border: 1px solid rgba(255,255,255,255);");

    DSBValue->setPrefix(tr("Valor R$ "));
    DSBValue->setMaximum(1000000000);

    cbType->add(tr("Movimentação"));
    cbType->add(tr("Entrada"));
    cbType->add(tr("Saída"));

    eDatePay->setInputMask("  /  /    ");
    eDateDeadLine->setInputMask("  /  /    ");

    cbStatus->add(tr("Status da Conta"));

    teNotes->setStyleSheet("background-color: qlineargradient"
                           "(spread:pad, x1:0, y1:1, x2:0.165045,"
                           "y2:0, stop:0 rgba(254, 255, 180, 255),"
                           "stop:0.721591 rgba(255, 250, 205, 255));"
                           "border-radius: 10px;border: 1px solid #C0C0C0;");

    pbConfirm->setText(tr("Confirmar"));
    pbLeave->setText(tr("Limpar"));

    splitter->insertWidget(1,wSpace);
    splitter->insertWidget(2,tableAccount);
    splitter->show();
    splitter->setHandleWidth(1);
    splitter->setStretchFactor(1,2);

    wBetweenDate->hide();

    swPanels->addWidget(wPlus);

    wSpace->setGeometry(0,0,0,0);

    eName->setNextComponent(DSBValue);

    DSBValue->setNextComponent(cbType);

    cbType->setNextComponent(cbStatus);

    cbStatus->setNextComponent(eDateDeadLine);

    eDateDeadLine->setNextComponent(eDatePay);

    eDatePay->setNextComponent(teNotes);

    tableAccount->setModel(bill->bd.getModel("bill"));

    bill->getTotalValue();
}

void AccountFinanceInterface::connections()
{
    connect(lAdd, SIGNAL(onClick()), SLOT(onClick_lAdd()));

    connect(lRemove, SIGNAL(onClick()), SLOT(onClick_lRemove()));

    connect(pbPayBox,SIGNAL(clicked()),SLOT(onClick_pbPayBox()));

    connect(splitter, SIGNAL(splitterMoved(int, int)), SLOT(adjustLayout()));

    connect(swPanels, SIGNAL(currentChanged(int)), SLOT(adjustLayout()));

    connect(pbConfirm, SIGNAL(clicked()), SLOT(onClick_pbConfirm()));

    connect(pbLeave, SIGNAL(clicked()), SLOT(onClick_pbLeave()));

    connect(cbType, SIGNAL(currentIndexChanged(int)), SLOT(currentIndexChanged_cbType(int)));

    connect(tableAccount, SIGNAL(doubleClicked(QModelIndex)), SLOT(doubleClicked_tableAccount(QModelIndex)));

    connect(cbShowType, SIGNAL(currentIndexChanged(int)), SLOT(currentIndexChanged_cbShowType(int)));

    connect(bill, SIGNAL(totalValue(QString)), SLOT(updateTotalValue(QString)));

    connect(actionName,SIGNAL(triggered()),this,SLOT(onTriggered_actionName()));

    connect(actionDescription,SIGNAL(triggered()),this,SLOT(onTriggered_actionDescription()));

    connect(actionDatePay,SIGNAL(triggered()),this,SLOT(onTriggered_actionDatePay()));

    connect(actionDateDeadLine,SIGNAL(triggered()),this,SLOT(onTriggered_actionDateDeadLine()));

    connect(wSeletorDate,SIGNAL(selected(int)),SLOT(showBetweenDate(int)));

    connect(wBetweenDate, SIGNAL(selectedDates(QString,QString)), SLOT(selectedDates(QString,QString)));

    connect(wSeletorDate, SIGNAL(selected(int)), SLOT(seletorDate(int)));
}

void AccountFinanceInterface::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void AccountFinanceInterface::adjustLayout()
{
    lTop->setGeometry(0,0,this->width(),50);

    searchAccount->setGeometry(this->width() - 220,11,200,28);

    textValueAccounts->setGeometry(lRemove->width() + lRemove->x() + 12,11,textValueAccountsSize(),28);

    cbShowType->setGeometry(textValueAccounts->width() + textValueAccounts->x() + 12,11,110,28);

    pbPayBox->setGeometry(cbShowType->width() + cbShowType->x() + 12,11,50,28);

    if((pbPayBox->x() + pbPayBox->width()) >= (this->width() / 2 - 140))
        wSeletorDate->setGeometry(pbPayBox->width() + pbPayBox->x() + 12,11,280,28);
    else
        wSeletorDate->setGeometry(this->width() / 2 - 140,11,280,28);

    splitter->setGeometry(0,lTop->height(),this->width(),this->height() - lTop->height());

    wSpace->setGeometry(0,0,this->width(),wSpace->height());

    swPanels->setGeometry(0,0,wSpace->width(), wSpace->height());

    lBackgroundSpace->setGeometry(0,0,wSpace->width(),wSpace->height());

    wPlus->setGeometry(wSpace->width()/2 - 300, wSpace->height()/2 - 150, 600,300);

    tableAccount->setGeometry(0,wSpace->y() + wSpace->height(),this->width(),
                              this->height() - (wSpace->height() + lTop->height()));

    tableAccount->setColumnHidden(0,true);

    tableAccount->setColumnWidth(1,tableAccount->width() * 0.15);
    tableAccount->setColumnWidth(2,tableAccount->width() * 0.09);
    tableAccount->setColumnWidth(3,tableAccount->width() * 0.11);
    tableAccount->setColumnWidth(4,tableAccount->width() * 0.09);
    tableAccount->setColumnWidth(5,tableAccount->width() * 0.13);
    tableAccount->setColumnWidth(6,tableAccount->width() * 0.13);
    tableAccount->setColumnWidth(7,tableAccount->width() * 0.3);

    wBetweenDate->setGeometry((wSeletorDate->x() + wSeletorDate->pbPeriod->width() * 4 + wSeletorDate->pbPeriod->width()/2) - wBetweenDate->W_/2,
                              wSeletorDate->y() + wSeletorDate->height() + 3,wBetweenDate->W_,wBetweenDate->H_);

}

void AccountFinanceInterface::adjustSplitter()
{
    QList<int> list;

    list << wSpace->height() << tableAccount->height();

    splitter->setSizes(list);

    adjustLayout();
}

void AccountFinanceInterface::hidePanel(bool b)
{
    if(b)
    {
        animaUp = panelEffects->createAnimation(wSpace,QRect(wSpace->x(),
                                                             wSpace->y(), wSpace->width(), 0),800,QEasingCurve::InBack);

        animaDown = panelEffects->createAnimation(tableAccount,QRect(tableAccount->x(),
                                                                     0, tableAccount->width(), this->height() - lTop->height()),800,QEasingCurve::InBack);
    }
    else
    {
        animaDown  = panelEffects->createAnimation(wSpace,QRect(wSpace->x(),
                                                                wSpace->y(), wSpace->width(), 360),800,QEasingCurve::OutBack);

        animaUp  = panelEffects->createAnimation(tableAccount,QRect(tableAccount->x(),360,
                                                                    tableAccount->width(),this->height() - (lTop->height() + 360)),800,QEasingCurve::OutBack);
    }

    connect(animaDown,SIGNAL(valueChanged(QVariant)),SLOT(adjustSplitter()));

    connect(animaUp,SIGNAL(valueChanged(QVariant)),SLOT(adjustSplitter()));

    animaUp->start();

    animaDown->start();
}

void AccountFinanceInterface::gotoPanel()
{
    bool b = (wSpace->height() > 0)? true:false;

    if(b)
        hidePanel(true);

    else
        hidePanel(false);

    lAdd->setAlternate(!b);
}

void AccountFinanceInterface::onClick_lAdd()
{
    gotoPanel();
}

void AccountFinanceInterface::onClick_lRemove()
{
    QModelIndex index = tableAccount->currentIndex();

    if (index.row() >= 0){
        int op = QMessageBox::question(this, tr("Apagar conta"), tr("Tem certeza que deseja deletar essa conta?"), tr("Sim"), tr("Não"));

        if (op >= 0)
            bill->remove(tableAccount->model()->index(index.row(), 0).data().toInt());
    }
}

void AccountFinanceInterface::statics()
{
    lAdd->setGeometry(12,11,30,28);

    lRemove->setGeometry(lAdd->width() + lAdd->x() + 12,11,30,28);

    lPlus->setGeometry(0,0,600,300);

    eName->setGeometry(25,20,170,28);

    DSBValue->setGeometry(25,eName->y() + eName->height() + 10,170,28);

    cbType->setGeometry(25,DSBValue->y() + DSBValue->height() + 10,170,28);

    cbStatus->setGeometry(25,cbType->y() + cbType->height() + 10,170,28);

    eDateDeadLine->setGeometry(25,cbStatus->y() + cbStatus->height() + 10,170,28);

    eDatePay->setGeometry(25,eDateDeadLine->y() + eDateDeadLine->height() + 10,170,28);

    teNotes->setGeometry(220,20,355,255);

    pbConfirm->setGeometry(25,eDatePay->y() + eDatePay->height() + 10,83,28);

    pbLeave->setGeometry(pbConfirm->x() + pbConfirm->width() + 4,pbConfirm->y(),83,28);
}

void AccountFinanceInterface::onClick_pbPayBox()
{
    emit gotoPayBox(1);
}

bool AccountFinanceInterface::isAvailable()
{
    if (eName->isEmpty()){
        QMessageBox::warning(this, tr("Preencha todos os campos."), tr("Nome é um campo obrigatório"), QMessageBox::Ok);

        eName->setFocus();

        return false;
    }

    if (DSBValue->value() == 0){
        QMessageBox::warning(this, tr("Preencha todos os campos."), tr("Valor é um campo obrigatório"), QMessageBox::Ok);

        DSBValue->setFocus();

        return false;
    }

    if (cbType->currentIndex() == 0){
        QMessageBox::warning(this, tr("Preencha todos os campos."), tr("Selecione o tipo de movimentação"), QMessageBox::Ok);

        cbType->setFocus();

        return false;
    }

    if (cbStatus->currentIndex() == 0){
        QMessageBox::warning(this, tr("Preencha todos os campos."), tr("Selecione o status da movimentação"), QMessageBox::Ok);

        cbStatus->setFocus();

        return false;
    }

    if (! eDatePay->isEmpty())
        if (! tools.isValidDate(eDatePay->text())){
            QMessageBox::warning(this, tr("Data inválida"), tr("Data de pagamento digitada é inválida"), QMessageBox::Ok);

            eDatePay->setFocus();

            return false;
        }

    if (! eDateDeadLine->isEmpty())
        if (! tools.isValidDate(eDateDeadLine->text())){
            QMessageBox::warning(this, tr("Data inválida"), tr("Data de vencimento digitada é inválida"), QMessageBox::Ok);

            eDateDeadLine->setFocus();

            return false;
        }

    if (eDateDeadLine->isEmpty() || eDatePay->isEmpty()){
        int op = QMessageBox::question(this, tr("Preencha todos os campos."), tr("Deseja salvar sem a data de vencimento e/ou a data de pagamento?"), tr("Sim"), tr("Não"));

        if (op == 1){
            cbStatus->setFocus();

            return false;
        }
    }

    return true;
}

void AccountFinanceInterface::onClick_pbConfirm()
{
    if (isAvailable()){
        bill->name = eName->text();

        bill->value = DSBValue->value();

        bill->type = cbType->currentText();

        bill->status = cbStatus->currentText();

        bill->dateExpiration = eDateDeadLine->text();

        bill->datePayment = eDatePay->text();

        bill->note = teNotes->toPlainText();

        if (isAdd){
            if (bill->insert())
                clearBill();
            else
                QMessageBox::warning(this, tr("Conta não inserida"), tr("Conta não pode ser inserida, favor verificar os dados."), QMessageBox::Ok);
        }else{
            if (bill->update(idUpdate))
                clearBill();
            else
                QMessageBox::warning(this, tr("Conta não alterada"), tr("Conta não pode ser alterada, favor verificar os dados."), QMessageBox::Ok);
        }
    }
}

void AccountFinanceInterface::onClick_pbLeave()
{
    clearBill();
}

void AccountFinanceInterface::clearBill()
{
    eName->Clear();

    DSBValue->setValue(0);

    cbType->setCurrentIndex(0);

    cbStatus->setCurrentIndex(0);

    eDatePay->Clear();

    eDateDeadLine->Clear();

    teNotes->clear();

    isAdd = true;

    pbConfirm->setText(tr("Confirmar"));

    pbLeave->setText(tr("Limpar"));
}

void AccountFinanceInterface::currentIndexChanged_cbType(int index)
{
    cbStatus->clear();

    cbStatus->add(tr("Status da Conta"));

    if (index == 1){
        cbStatus->add(tr("A receber"));
        cbStatus->add(tr("Recebido"));
    }else if (index == 2){
        cbStatus->add(tr("A pagar"));
        cbStatus->add(tr("Pago"));
    }
}

void AccountFinanceInterface::doubleClicked_tableAccount(QModelIndex index)
{
    if (index.row() < 0)
        return; // acho que ele NUNCA chegaria aqui, mas na duvida eu testo

    isAdd = false;

    pbConfirm->setText("Editar");

    pbLeave->setText("Cancelar");

    idUpdate = tableAccount->model()->index(index.row(), 0).data().toInt();
    eName->setText(tableAccount->model()->index(index.row(), 1).data().toString());
    DSBValue->setValue(tableAccount->model()->index(index.row(), 2).data().toDouble());

    if (tableAccount->model()->index(index.row(), 3).data().toString() == "Entrada")
        cbType->setCurrentIndex(1);
    else
        cbType->setCurrentIndex(2);

    if (tableAccount->model()->index(index.row(), 4).data().toString() == "A receber" || tableAccount->model()->index(index.row(), 4).data().toString() == "A pagar")
        cbStatus->setCurrentIndex(1);
    else
        cbStatus->setCurrentIndex(2);

    eDatePay->setText(tools.convertDate2(tableAccount->model()->index(index.row(), 5).data().toString()));
    eDateDeadLine->setText(tools.convertDate2(tableAccount->model()->index(index.row(), 6).data().toString()));
    teNotes->setText(tableAccount->model()->index(index.row(), 7).data().toString());
}

void AccountFinanceInterface::currentIndexChanged_cbShowType(int index)
{
    // filtros

    switch(index){
    case 0:
        bill->areFilterType = false;
        bill->setFilter();
        break;

    case 1:
        bill->filterType = "typebill = 'Entrada'";
        bill->areFilterType = true;
        bill->setFilter();
        break;

    case 2:
        bill->filterType = "typebill = 'Saída'";
        bill->areFilterType = true;
        bill->setFilter();
        break;

    case 3:
        bill->filterType = "typebill = 'Saída' and status = 'Pago'";
        bill->areFilterType = true;
        bill->setFilter();
        break;

    case 4:
        bill->filterType = "typebill = 'Entrada' and status = 'Recebido'";
        bill->areFilterType = true;
        bill->setFilter();
        break;

    case 5:
        bill->filterType = "typebill = 'Saída' and status = 'A pagar'";
        bill->areFilterType = true;
        bill->setFilter();
        break;

    case 6:
        bill->filterType = "typebill = 'Entrada' and status = 'A receber'";
        bill->areFilterType = true;
        bill->setFilter();
        break;
    }
}

void AccountFinanceInterface::updateTotalValue(QString value)
{    
    QFont f;

#if defined(Q_WS_X11)
    f = QFont("Sans Serif",9);
#endif

#if defined(Q_WS_WIN)
    f = QFont("MS Shell Dlq 2", 11);
#endif

    f.setBold(true);

    textValueAccounts->setText(tools.convertMoney(value));
    textValueAccounts->setFont(f);
    textValueAccounts->setAlignment(Qt::AlignHCenter);

    adjustLayout();
}

void AccountFinanceInterface::onTriggered_actionName()
{
    actionName->setEnabled(false);
    actionName->setChecked(true);

    actionDescription->setEnabled(true);
    actionDescription->setChecked(false);

    actionDatePay->setEnabled(true);
    actionDatePay->setChecked(false);

    actionDateDeadLine->setEnabled(true);
    actionDateDeadLine->setChecked(false);
}

void AccountFinanceInterface::onTriggered_actionDescription()
{
    actionDescription->setEnabled(false);
    actionDescription->setChecked(true);

    actionName->setEnabled(true);
    actionName->setChecked(false);

    actionDatePay->setEnabled(true);
    actionDatePay->setChecked(false);

    actionDateDeadLine->setEnabled(true);
    actionDateDeadLine->setChecked(false);
}

void AccountFinanceInterface::onTriggered_actionDatePay()
{
    actionDatePay->setEnabled(false);
    actionDatePay->setChecked(true);

    actionName->setEnabled(true);
    actionName->setChecked(false);

    actionDescription->setEnabled(true);
    actionDescription->setChecked(false);

    actionDateDeadLine->setEnabled(true);
    actionDateDeadLine->setChecked(false);
}

void AccountFinanceInterface::onTriggered_actionDateDeadLine()
{
    actionDateDeadLine->setEnabled(false);
    actionDateDeadLine->setChecked(true);

    actionDescription->setEnabled(true);
    actionDescription->setChecked(false);

    actionDatePay->setEnabled(true);
    actionDatePay->setChecked(false);

    actionName->setEnabled(true);
    actionName->setChecked(false);
}

int AccountFinanceInterface::textValueAccountsSize()
{
    short int pointSize;

#if defined(Q_WS_X11)
    pointSize = 9;
#endif

#if defined(Q_WS_WIN)
    pointSize = 11;
#endif

    int value = 12 + textValueAccounts->toPlainText().length() * pointSize;

    return value;
}

void AccountFinanceInterface::showBetweenDate(int option)
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

void AccountFinanceInterface::selectedDates(QString dateS, QString dateF)
{
    bill->getBillsCustom(dateS, dateF);

    wSeletorDate->pbToday->setToolTip("");
    wSeletorDate->pbWeek->setToolTip("");
    wSeletorDate->pbMonth->setToolTip("");
    wSeletorDate->pbYear->setToolTip("");

    wSeletorDate->pbPeriod->setToolTip(QString("Busca entre as datas:\n%1 e \n%2").arg(dateS, dateF));

    wBetweenDate->hide();
}

void AccountFinanceInterface::seletorDate(int i)
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
        bill->getBillsDays();

        wSeletorDate->pbWeek->setToolTip("");
        wSeletorDate->pbMonth->setToolTip("");
        wSeletorDate->pbYear->setToolTip("");
        wSeletorDate->pbPeriod->setToolTip("");

        wSeletorDate->pbToday->setToolTip(QString("Busca para hoje:\n%1").arg(bill->dateIFilter));

        break;
    }

    case 1:{
        bill->getBillsWeek();

        wSeletorDate->pbToday->setToolTip("");
        wSeletorDate->pbMonth->setToolTip("");
        wSeletorDate->pbYear->setToolTip("");
        wSeletorDate->pbPeriod->setToolTip("");

        wSeletorDate->pbWeek->setToolTip(QString("Busca entre as datas:\n%1 e \n%2").arg(bill->dateIFilter, bill->dateFFilter));

        break;
    }

    case 2:{
        bill->getBillsMonth();

        wSeletorDate->pbToday->setToolTip("");
        wSeletorDate->pbWeek->setToolTip("");
        wSeletorDate->pbYear->setToolTip("");
        wSeletorDate->pbPeriod->setToolTip("");

        wSeletorDate->pbMonth->setToolTip(QString("Busca entre as datas:\n%1 e \n%2").arg(bill->dateIFilter, bill->dateFFilter));

        break;
    }

    case 3:{
        bill->getBillsYear();

        wSeletorDate->pbToday->setToolTip("");
        wSeletorDate->pbWeek->setToolTip("");
        wSeletorDate->pbMonth->setToolTip("");
        wSeletorDate->pbPeriod->setToolTip("");

        wSeletorDate->pbYear->setToolTip(QString("Busca entre as datas:\n%1 e \n%2").arg(bill->dateIFilter, bill->dateFFilter));

        break;
    }

    case 4:{

        break;
    }

    }
}

void AccountFinanceInterface::logout()
{
    onClick_pbLeave();
}
