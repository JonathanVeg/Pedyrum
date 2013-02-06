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


 #include "Interface/functionaryinterface.h"
#include <QDebug>
#include <QMessageBox>

FunctionaryInterface::FunctionaryInterface(QWidget *parent) :
    QWidget(parent)
{
    createComponent();    
    connections();
    statics();
}

void FunctionaryInterface::createComponent()
{
    employee = new Employee();

    lTop = new QLabel(this);

    lAdd = new Label(this,true,100);
    lRemove = new Label(this,true,100);
    lProfile = new Label(this,true,100);
    lCalc = new Label(this,true,100);
    pbAvailable = new QPushButton(this);
    pbUnavailable = new QPushButton(this);
    cbSelectJobRole = new ComboBox(this);

    searchFunctionary = new Search(this,tr("Pesquisar funcionários"),true);

    wSpace = new QWidget(this);
    lBackgroundSpace = new QLabel(wSpace);

    tableEmployee = new TableView(this);
    splitter = new QSplitter(Qt::Vertical,this);
    swPanels = new QStackedWidget(wSpace);

    wGroupPlus = new QWidget(wSpace);
    lBackgroundPlus = new QLabel(wGroupPlus);

    eName = new Edit(wGroupPlus,true,tr("Nome"), 30);
    eCPF = new Edit(wGroupPlus,true,tr("CPF"), 20);
    eRG = new Edit(wGroupPlus,true,tr("RG"), 12);
    ePhone = new phoneEdit(wGroupPlus,true,tr("Telefone"));
    eMobile = new phoneEdit(wGroupPlus,true,tr("Celular"));
    eRadio = new Edit(wGroupPlus,true,tr("Rádio"));
    eCep = new Edit(wGroupPlus,true,tr("CEP"), 9);
    eStreet = new Edit(wGroupPlus,true,tr("Rua"), 50);
    eNumber = new Edit(wGroupPlus,true,tr("Nº"), 5);
    eComplement = new Edit(wGroupPlus,true,tr("Complemento"), 20);
    eDistrict = new Edit(wGroupPlus,true,tr("Bairro"), 30);

    cbJobRole = new ComboBox(wGroupPlus);

    textPercent = new QLabel(wGroupPlus);
    DSBComission = new DoubleSpinBox(wGroupPlus);

    pbConfirm = new QPushButton(wGroupPlus);
    pbLeave = new QPushButton(wGroupPlus);

    eBank = new Edit(wGroupPlus,true,tr("Banco"));
    eCC = new Edit(wGroupPlus,true,tr("Conta"));
    eAg = new Edit(wGroupPlus,true,tr("Agência"));

    teNoteFunctionary = new QTextEdit(wGroupPlus);

    wGroupProfile = new QWidget(wSpace);    
    tbProfile = new QTextBrowser(wGroupProfile);

    panelEffects = new GraphicEffects(this);

    wCalc = new QWidget(this);
    lCalcbg = new QLabel(wCalc);

    lValueCommission = new QLabel(wCalc);
    eBeforeDateCommission = new Edit(wCalc,true,tr("Data Inicial"));
    eAfterDateCommission = new Edit(wCalc,true,tr("Data Final"));
    pbCalculateCommission = new QPushButton(tr("Calcular"),wCalc);
}

void FunctionaryInterface::input()
{
    isAdd = true;

    nextPanelIndex = 0;

    sizePanel = 300;

    swPanels->addWidget(wGroupPlus);
    swPanels->addWidget(wGroupProfile);
    swPanels->addWidget(wCalc);

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

    lProfile->setImage(":/subMenu/buttonProfile");
    lProfile->setHover(":/subMenu/buttonProfile-hover");
    lProfile->setAlternateImage(lProfile->imageHover);
    lProfile->setToolTip(tr("Visualizar Perfil"));

    lCalc->setImage(":/subMenu/buttonCalc");
    lCalc->setHover(":/subMenu/buttonCalc-hover");
    lCalc->setAlternateImage(lCalc->imageHover);
    lCalc->setToolTip(tr("Calcular Comissão"));

    pbAvailable->setFont(this->font());
    pbAvailable->setText(tr("Disponível"));
    pbAvailable->setCursor(Qt::PointingHandCursor);

    pbUnavailable->setFont(this->font());
    pbUnavailable->setText(tr("Indisponível"));
    pbUnavailable->setCursor(Qt::PointingHandCursor);

    cbSelectJobRole->setFont(this->font());
    cbSelectJobRole->setObjectName("cbSelectJobRole");
    cbSelectJobRole->setToolTip(tr("Selecionar por cargo"));
    cbSelectJobRole->addItem(tr("Selecione o cargo"));
    cbSelectJobRole->addItem(tr("Entregador"));
    cbSelectJobRole->addItem(tr("Garçom"));

    pbConfirm->setText(tr("Confirmar"));
    pbConfirm->setToolTip(tr("Confirmar operação"));

    pbLeave->setText(tr("Limpar"));
    pbLeave->setToolTip(tr("Limpar todos os campos"));

    lBackgroundSpace->setStyleSheet("background-color: rgba(123, 31, 31, 255);");

    lBackgroundPlus->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.498,"
                                   "y1:0, x2:0.494636, y2:1, stop:0 rgba(255, 255, 255, 210),"
                                   "stop:0.982955 rgba(255, 255, 255, 170));"
                                   "border-radius: 5px;border: 1px solid rgba(255,255,255,255);");

    tbProfile->setStyleSheet(lBackgroundPlus->styleSheet());

    cbJobRole->setFont(this->font());
    cbJobRole->add(tr("Selecione o cargo"));
    cbJobRole->add(tr("Entregador"));
    cbJobRole->add(tr("Garçom"));

    textPercent->setText(tr("Comissão por porcentagem."));
    textPercent->setAlignment(Qt::AlignHCenter);
    textPercent->setStyleSheet("color: rgba(0,0,0,240);");
    textPercent->setFont(this->font());

    DSBComission->setFont(this->font());
    DSBComission->setSuffix("%");
    DSBComission->setMaximum(100);

    splitter->insertWidget(1,wSpace);
    splitter->insertWidget(2,tableEmployee);
    splitter->show();
    splitter->setHandleWidth(1);
    splitter->setStretchFactor(1,2);

    wSpace->setGeometry(0,0,0,0);

    eName->setNextComponent(eCPF);
    eCPF->setNextComponent(eRG);
    eRG->setNextComponent(ePhone);
    ePhone->setNextComponent(eMobile);
    eMobile->setNextComponent(eRadio);
    eRadio->setNextComponent(eCep);
    eCep->setNextComponent(eStreet);
    eStreet->setNextComponent(eNumber);
    eNumber->setNextComponent(eComplement);
    eComplement->setNextComponent(eDistrict);
    eDistrict->setNextComponent(cbJobRole);

    cbJobRole->setNextComponent(DSBComission);

    DSBComission->setNextComponent(eBank);

    eBank->setNextComponent(eCC);
    eCC->setNextComponent(eAg);

    teNoteFunctionary->setStyleSheet("background-color: qlineargradient"
                                     "(spread:pad, x1:0, y1:1, x2:0.165045,"
                                     "y2:0, stop:0 rgba(254, 255, 180, 255),"
                                     "stop:0.721591 rgba(255, 250, 205, 255));"
                                     "border-radius: 10px;border: 1px solid #C0C0C0;");

    eCPF->setAutomaticallyNextComponent(true);
    eRG->setAutomaticallyNextComponent(true);
    ePhone->setAutomaticallyNextComponent(true);
    eMobile->setAutomaticallyNextComponent(true);
    eRadio->setAutomaticallyNextComponent(true);
    eCep->setAutomaticallyNextComponent(true);

    tableEmployee->setModel(employee->bd.getModelQuery("employee"));

    tableEmployee->setColumnHidden(0,true);
    tableEmployee->setColumnHidden(11,true);
    tableEmployee->setColumnHidden(12,true);
    tableEmployee->setColumnHidden(13,true);
    tableEmployee->setColumnHidden(14,true);
    tableEmployee->setColumnHidden(15,true);
    tableEmployee->setColumnHidden(16,true);
    tableEmployee->setColumnHidden(17,true);
    tableEmployee->setColumnHidden(18,true);
    tableEmployee->setColumnHidden(19,true);
    tableEmployee->setColumnHidden(20,true);
    tableEmployee->setColumnHidden(21,true);
    tableEmployee->setColumnHidden(22,true);
    tableEmployee->setColumnHidden(23,true);
    tableEmployee->setColumnHidden(24,true);
    tableEmployee->setColumnHidden(25,true);

    eCPF->setInputMask("   .   .   -  ");
    eRG->setInputMask("  .   .   - ");
    eRadio->setInputMask("   *     ");
    eCep->setInputMask("     -   ");

    lCalcbg->setStyleSheet(lBackgroundPlus->styleSheet());

    QFont f;
    f.setPointSize(14);

    lValueCommission->setText(tr("R$ 0,00"));
    lValueCommission->setAlignment(Qt::AlignCenter);
    lValueCommission->setStyleSheet("color: rgba(0,0,0,100);");
    lValueCommission->setFont(f);

    eBeforeDateCommission->setInputMask("  /  /    ");
    eAfterDateCommission->setInputMask("  /  /    ");

    eBeforeDateCommission->setNextComponent(eAfterDateCommission);

    createProfile();
    onClick_pbAvailable();
}

void FunctionaryInterface::connections()
{
    connect(splitter, SIGNAL(splitterMoved(int, int)),SLOT(adjustLayout()));

    connect(swPanels, SIGNAL(currentChanged(int)),SLOT(adjustLayout()));

    connect(lAdd, SIGNAL(onClick()), SLOT(onClick_lAdd()));

    connect(lRemove, SIGNAL(onClick()), SLOT(onClick_lRemove()));

    connect(lProfile, SIGNAL(onClick()), SLOT(onClick_lProfile()));

    connect(lCalc,SIGNAL(onClick()),SLOT(onClick_lCalc()));

    connect(eCPF, SIGNAL(textChanged(QString)), SLOT(textChanged_eCPF(QString)));

    connect(pbConfirm, SIGNAL(clicked()), SLOT(onClick_pbConfirm()));

    connect(pbLeave, SIGNAL(clicked()), SLOT(onClick_pbLeave()));

    connect(tableEmployee, SIGNAL(doubleClicked(QModelIndex)), SLOT(doubleClicked_tableEmployee(QModelIndex)));

    connect(cbSelectJobRole, SIGNAL(currentIndexChanged(QString)), SLOT(currentIndexChanged_cbSelectJobRole(QString)));

    connect(pbAvailable,SIGNAL(clicked()),this,SLOT(onClick_pbAvailable()));

    connect(pbUnavailable,SIGNAL(clicked()),this,SLOT(onClick_pbUnavailable()));

    connect(searchFunctionary, SIGNAL(textSearch(QString)), SLOT(getSearchFunctionary(QString)));

    connect(pbCalculateCommission, SIGNAL(clicked()), SLOT(onClick_pbCalculateCommission()));

    connect(eAfterDateCommission, SIGNAL(returnPressed()), this, SLOT(onClick_pbCalculateCommission()));
}

void FunctionaryInterface::hidePanel(bool b)
{
    if(b)
    {
        animaUp = panelEffects->createAnimation(wSpace,QRect(wSpace->x(),
                                                             wSpace->y(), wSpace->width(), 0),800,QEasingCurve::InBack);

        animaDown = panelEffects->createAnimation(tableEmployee,QRect(tableEmployee->x(),
                                                                      0, tableEmployee->width(), this->height() - lTop->height()),800,QEasingCurve::InBack);
    }
    else
    {
        animaDown  = panelEffects->createAnimation(wSpace,QRect(wSpace->x(),
                                                                wSpace->y(), wSpace->width(), sizePanel),800,QEasingCurve::OutBack);

        animaUp  = panelEffects->createAnimation(tableEmployee,QRect(tableEmployee->x(),sizePanel,
                                                                     tableEmployee->width(),this->height() - (lTop->height() + sizePanel)),800,QEasingCurve::OutBack);
    }

    connect(animaDown,SIGNAL(valueChanged(QVariant)),SLOT(adjustSplitter()));

    connect(animaUp,SIGNAL(valueChanged(QVariant)),SLOT(adjustSplitter()));

    animaUp->start();

    animaDown->start();
}

void FunctionaryInterface::statics()
{
    lAdd->setGeometry(12,10,30,28);
    lRemove->setGeometry(lAdd->width() + lAdd->x() + 12,10,30,28);
    lProfile->setGeometry(lRemove->width() + lRemove->x() + 12,10,30,28);
    lCalc->setGeometry(lProfile->width() + lProfile->x() + 12,10,30,28);

    pbAvailable->setGeometry(lCalc->width() + lCalc->x() + 12, 10, 70, 28);
    pbUnavailable->setGeometry(pbAvailable->x() + pbAvailable->width() + 2, pbAvailable->y(), 70,28);

    cbSelectJobRole->setGeometry(pbUnavailable->width() + pbUnavailable->x() + 12,10,135,28);

    eName->setGeometry(15,15,210,25);
    eCPF->setGeometry(15,eName->y() + 32,115,25);
    eRG->setGeometry(eCPF->x() + eCPF->width() + 5,eName->y() + 32,90,25);

    ePhone->setGeometry(15,eCPF->y() + 32,102,25);
    eMobile->setGeometry(ePhone->x() + ePhone->width() + 5,eCPF->y() + 32,103,25);

    eRadio->setGeometry(15,ePhone->y() + 32,100,25);
    eCep->setGeometry(eRadio->x() + eRadio->width() + 5,ePhone->y() + 32,105,25);

    eStreet->setGeometry(15,eCep->y() + 32,210,25);
    eNumber->setGeometry(15,eStreet->y() + 32,70,25);
    eComplement->setGeometry(eNumber->x() + eNumber->width() + 5,eStreet->y() + 32,135,25);
    eDistrict->setGeometry(15,eComplement->y() + 32,210,25);

    cbJobRole->setGeometry(240,20,200,25);
    textPercent->setGeometry(240,cbJobRole->y() + cbJobRole->height() + 10,200,16);
    DSBComission->setGeometry(textPercent->x() + 25,textPercent->y() + 30,150, 27);

    eBank->setGeometry(textPercent->x(),DSBComission->y() + DSBComission->height() + 10,textPercent->width(),27);
    eCC->setGeometry(eBank->x(),eBank->y() + eBank->height() + 10,95,27);
    eAg->setGeometry(eCC->x() + eCC->width() + 10,eCC->y(),95,27);

    pbLeave->setGeometry(eCC->x(),eAg->y() + eAg->height() + 10,90,30);
    pbConfirm->setGeometry(pbLeave->x() + pbLeave->width() + 20,pbLeave->y(),90,30);

    teNoteFunctionary->setGeometry(eAg->x() + eAg->width() + 15, 15, 230, 220);

    lCalcbg->setGeometry(0,0,300,186);

    lValueCommission->setGeometry(lCalcbg->width()/2 - 70,20,140,28);

    eBeforeDateCommission->setGeometry(lCalcbg->width()/2 - 70,60,140,28);

    eAfterDateCommission->setGeometry(lCalcbg->width()/2 - 70,eBeforeDateCommission->y() + eBeforeDateCommission->height() + 10,140,28);

    pbCalculateCommission->setGeometry(lCalcbg->width()/2 - 45,eAfterDateCommission->y() + eAfterDateCommission->height() + 10,90,28);
}

void FunctionaryInterface::adjustLayout()
{
    lTop->setGeometry(0,0,this->width(),50);

    searchFunctionary->setGeometry(this->width() - 220,10,200,28);

    splitter->setGeometry(0,lTop->height(),this->width(),this->height() - lTop->height());

    wSpace->setGeometry(0,0,this->width(),wSpace->height());

    lBackgroundSpace->setGeometry(0,0,this->width(),wSpace->height());

    swPanels->setGeometry(0,0,wSpace->width(),wSpace->height());

    wGroupPlus->setGeometry(this->width()/2 - 350,wSpace->height()/2 - 123,700,246);
    wGroupProfile->setGeometry(this->width()/2 - 300,wSpace->height()/2 - 110,600,220);
    wCalc->setGeometry(this->width()/2 - 150,wSpace->height()/2 - 93,300,186);

    lBackgroundPlus->setGeometry(0,0,wGroupPlus->width(),wGroupPlus->height());

    tbProfile->setGeometry(0,0,wGroupProfile->width(),wGroupProfile->height());

    tableEmployee->setGeometry(0,wSpace->y() + wSpace->height(),this->width(),
                               this->height() - (wSpace->height() + lTop->height()));

    tableEmployee->setColumnWidth(1,tableEmployee->width() * 0.20);
    tableEmployee->setColumnWidth(2,tableEmployee->width() * 0.07);
    tableEmployee->setColumnWidth(3,tableEmployee->width() * 0.07);
    tableEmployee->setColumnWidth(4,tableEmployee->width() * 0.07);
    tableEmployee->setColumnWidth(5,tableEmployee->width() * 0.20);
    tableEmployee->setColumnWidth(6,tableEmployee->width() * 0.07);
    tableEmployee->setColumnWidth(7,tableEmployee->width() * 0.07);
    tableEmployee->setColumnWidth(8,tableEmployee->width() * 0.07);
    tableEmployee->setColumnWidth(9,tableEmployee->width() * 0.07);
    tableEmployee->setColumnWidth(10,tableEmployee->width() * 0.11);
}

void FunctionaryInterface::gotoPanel(int index)
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
    case 0: lAdd->setAlternate(!b);
        break;

    case 1: lProfile->setAlternate(!b);
        break;

    case 2: lCalc->setAlternate(!b);
        break;
    }
}

void FunctionaryInterface::nextPanel()
{
    disconnect(animaUp,SIGNAL(finished()),this,SLOT(nextPanel()));

    switch(swPanels->currentIndex())
    {
    case 0:lAdd->setAlternate(false);
        break;

    case 1:lProfile->setAlternate(false);
        break;

    case 2:lCalc->setAlternate(false);
        break;
    }

    switch(nextPanelIndex)
    {
    case 0:lAdd->setAlternate(true);
        break;

    case 1:lProfile->setAlternate(true);
        break;    

    case 2:lCalc->setAlternate(true);
        break;
    }

    hidePanel(false);

    swPanels->setCurrentIndex(nextPanelIndex);
}

void FunctionaryInterface::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void FunctionaryInterface::adjustSplitter()
{
    QList<int> list;

    list << wSpace->height() << tableEmployee->height();

    splitter->setSizes(list);

    adjustLayout();
}

void FunctionaryInterface::onClick_lAdd()
{
    if(swPanels->currentIndex() == 0)
        onClick_pbLeave();

    sizePanel = 300;

    gotoPanel(0);
}

void FunctionaryInterface::onClick_lRemove()
{
    if (row() >= 0){
        QString name = tableEmployee->model()->index(row(),1).data().toString();

        QString message = "Tem certeza que deseja remover o(a) funcionário: " + name;

        if (QMessageBox::question(this, tr("Deletar funcionário"),message, tr("Remover"), tr("Cancelar")) == 0){
            employee->remove(tableEmployee->model()->index(row(),0).data().toInt());

            employee->refreshModel();
        }
    }
}

void FunctionaryInterface::onClick_lProfile()
{
    sizePanel = 250;

    gotoPanel(1);
}

void FunctionaryInterface::onClick_lCalc()
{
    sizePanel = 220;

    gotoPanel(2);
}

void FunctionaryInterface::onClick_pbConfirm()
{
    if (isAvailable()){
        employee->name = eName->value();
        employee->phone = ePhone->value();
        employee->phoneMobile = eMobile->value();
        employee->radio = eRadio->value();
        employee->street = eStreet->value();
        employee->number = eNumber->value();
        employee->complement = eComplement->value();
        employee->district = eDistrict->value();
        employee->cep = eCep->value();
        employee->jobRole = cbJobRole->currentText();
        employee->commission = DSBComission->value();
        employee->cpf = eCPF->text();
        employee->rg = eRG->text();
        employee->cc = eCC->value();
        employee->ag = eAg->value();
        employee->bank = eBank->value();

        employee->note = teNoteFunctionary->toPlainText();

        if (isAdd){

            if (employee->insert()){
                employee->refreshModel();

                QMessageBox::information(this, "Inserido com sucesso", "O funcionário foi inserido com sucesso.", QMessageBox::Ok);

                onClick_pbLeave();
            }else{
                qDebug() << "[FunctionaryInterface] [Salvar Funcionário]" << "Não inserido";
            }

        }else{

            if (employee->update(idEmployeeUpdate)){
                employee->refreshModel();

                QMessageBox::information(this, "Alterado com sucesso", "Os dados do funcionário foram alterados com sucesso.", QMessageBox::Ok);

                onClick_pbLeave();
            }else{
                qDebug() << "[FunctionaryInterface] [Alterar funcionário]" << "Não alterado";
            }

        }
    }
}

void FunctionaryInterface::onClick_pbLeave()
{
    employee->clearData();

    eName->Clear();
    ePhone->Clear();
    eMobile->Clear();
    eRadio->Clear();
    eStreet->Clear();
    eNumber->Clear();
    eComplement->Clear();
    eDistrict->Clear();
    eCep->Clear();
    cbJobRole->setCurrentIndex(0);
    DSBComission->setValue(0);
    eCPF->Clear();
    eRG->Clear();
    eCC->Clear();
    eAg->Clear();
    eBank->Clear();
    teNoteFunctionary->clear();

    isAdd = true;

    pbConfirm->setText(tr("Confirmar"));
    pbLeave->setText("Limpar");

    eName->setFocus();
}



void FunctionaryInterface::textChanged_eCPF(QString text)
{
    if (text.length() == 14){
        text = text.replace(".","").replace("-","");

        if (tools.validateCPF(text)){

            eCPF->setToolTip("CPF válido");

            eCPF->setStyleSheet(CSS_VALID_EDIT_EMPLOYEE);

            eRG->setFocus();
        }else{

            eCPF->setToolTip("CPF inválido");

            eCPF->setStyleSheet(CSS_INVALID_EDIT_EMPLOYEE);
        }
    }else{
        eCPF->setStyleSheet(CSS_VALID_EDIT_EMPLOYEE);
    }
}

bool FunctionaryInterface::isAvailable()
{
    if ((! eCPF->text().isEmpty()) && (! tools.validateCPF(eCPF->text().replace(".","").replace("-","")))){
        QMessageBox::warning(this, tr("CPF inválido"), tr("O CPF digitado é inválido."), QMessageBox::Ok);

        eCPF->setFocus();

        return false;
    }

    if (eName->isEmpty()){
        QMessageBox::warning(this, tr("Cadastro incompleto"), tr("Nome é um campo obrigatório."), QMessageBox::Ok);

        eName->setFocus();

        return false;
    }

    if (ePhone->isEmpty() && eMobile->isEmpty() && eRadio->isEmpty()){
        QMessageBox::warning(this, tr("Cadastro incompleto"), tr("Preencha pelo menos um telefone para o funcionário."), QMessageBox::Ok);

        ePhone->setFocus();

        return false;
    }

    if (eStreet->isEmpty()){
        QMessageBox::warning(this, tr("Cadastro incompleto"), tr("Rua é um campo obrigatório."), QMessageBox::Ok);

        eStreet->setFocus();

        return false;
    }

    if (eNumber->isEmpty()){
        QMessageBox::warning(this, tr("Cadastro incompleto"), tr("Número é um campo obrigatório."), QMessageBox::Ok);

        eNumber->setFocus();

        return false;
    }

    if (eDistrict->isEmpty()){
        QMessageBox::warning(this, tr("Cadastro incompleto"), tr("Bairro é um campo obrigatório."), QMessageBox::Ok);

        eDistrict->setFocus();

        return false;
    }

    if (eCep->isEmpty()){
        QMessageBox::warning(this, tr("Cadastro incompleto"), tr("CEP é um campo obrigatório."), QMessageBox::Ok);

        eCep->setFocus();

        return false;
    }

    if (cbJobRole->currentIndex() == 0){
        QMessageBox::warning(this, tr("Cadastro incompleto"), tr("Selecione um cargo para o funcionário."), QMessageBox::Ok);

        cbJobRole->setFocus();

        return false;
    }

    return true;
}

void FunctionaryInterface::doubleClicked_tableEmployee(QModelIndex index){
    idEmployeeUpdate = tableEmployee->model()->index(row(), 0).data().toInt();

    if (employee->loadData(idEmployeeUpdate))
        createProfileWithData();
    else
        qDebug() << "[FunctionaryInterface] [createProfileWithData] Dados do funcionário não carregado";

    isAdd = false;

    eName->setText(employee->name);
    if (eName->isEmpty())
        eName->Clear();

    ePhone->setText(employee->phone);
    if (ePhone->isEmpty())
        ePhone->Clear();

    eMobile->setText(employee->phoneMobile);
    if (eMobile->isEmpty())
        eMobile->Clear();

    eRadio->setText(employee->radio);
    if (eRadio->isEmpty())
        eRadio->Clear();

    eStreet->setText(employee->street);
    if (eStreet->isEmpty())
        eStreet->Clear();

    eNumber->setText(employee->number);
    if (eNumber->isEmpty())
        eNumber->Clear();

    eComplement->setText(employee->complement);
    if (eComplement->isEmpty())
        eComplement->Clear();

    eDistrict->setText(employee->district);
    if (eDistrict->isEmpty())
        eDistrict->Clear();

    eCep->setText(employee->cep);
    if (eCep->isEmpty())
        eCep->Clear();

    cbJobRole->setCurrentText(employee->jobRole);

    DSBComission->setValue(employee->commission);

    eCPF->setText(employee->cpf);
    if (eCPF->isEmpty())
        eCPF->Clear();

    eRG->setText(employee->rg);
    if (eRG->isEmpty())
        eRG->Clear();

    eCC->setText(employee->cc);
    if (eCC->isEmpty())
        eCC->Clear();

    eAg->setText(employee->ag);
    if (eAg->isEmpty())
        eAg->Clear();

    eBank->setText(employee->bank);
    if (eBank->isEmpty())
        eBank->Clear();

    teNoteFunctionary->setText(employee->note);

    pbConfirm->setText(tr("Editar"));
    pbLeave->setText(tr("Cancelar"));

    bool b = (wSpace->height() > 0)? true:false;

    if (! b)
        gotoPanel(0);
}

void FunctionaryInterface::currentIndexChanged_cbSelectJobRole(QString text){
    if (cbSelectJobRole->currentIndex() > 0)
        employee->setFilterJobRole(text);
    else
        employee->clearFilterJobRole();
}

void FunctionaryInterface::createProfile()
{
    QString html;

    QString text = tr("Selecione um funcionário");

    html += "<html><head></head><body><br><br>"
            "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px;"
            " margin-left:0px; margin-right:0px; -qt-block-indent:0;"
            "text-indent:0px;\"><span style=\" font-size:20pt;\">" + text + "</span></p>"
            "</body></html>";

    tbProfile->setHtml(html);
}

void FunctionaryInterface::createProfileWithData()
{    
    QString name = employee->name;
    QString CPF = employee->cpf;
    QString RG = employee->rg;
    QString phone = employee->phone;
    QString phoneMobile = employee->phoneMobile;
    QString radio = employee->radio;

    QString address;

    if (! employee->complement.isEmpty())
        address = QString("%1, %2, %3").arg(employee->street, employee->number, employee->complement);
    else
        address = QString("%1, %2").arg(employee->street, employee->number);

    QString district = employee->district;
    QString jobRole = employee->jobRole;
    QString commission = QString::number(employee->commission);
    QString bank = employee->bank;
    QString account = employee->cc;
    QString agency = employee->ag;

    QString note = employee->note;

    QString html = "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">"
            "p, li { white-space: pre-wrap; }"
            "</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">"
            "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
            "-qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; color:#636363;\">" + name + "</span></p>"
            "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px;"
            "margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:12pt; color:#636363;\"></p>"
            "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
            "text-indent:0px;\"><span style=\" font-size:9pt; color:#636363;\">" + tr("CPF: ") + "</span><span style=\" font-size:9pt;"
            "color:#121315;\">" + CPF + "</span></p>"
            "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
            "text-indent:0px;\"><span style=\" font-size:9pt; color:#636363;\">" + tr("RG: ") + "</span><span style=\" font-size:9pt;"
            "color:#121315;\">" + RG + "</span></p>"
            "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;";

    if (! phone.isEmpty())
        html += "text-indent:0px;\"><span style=\" font-size:9pt; color:#636363;\">" + tr("Telefone: ") + "</span><span style=\" font-size:9pt;"
                "color:#121315;\">" + phone + "</span></p>"
                "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;";

    if (! phoneMobile.isEmpty())
        html += "text-indent:0px;\"><span style=\" font-size:9pt; color:#636363;\">" + tr("Celular: ") + "</span><span style=\" font-size:9pt;"
                "color:#121315;\">" + phoneMobile + "</span></p>"
                "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;";

    if (! radio.isEmpty())
        html+= "text-indent:0px;\"><span style=\" font-size:9pt; color:#636363;\">" + tr("Rádio: ") + "</span><span style=\" font-size:9pt;"
                "color:#121315;\">" + radio + "</span></p>"
                "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;";

    html += "text-indent:0px;\"><span style=\" font-size:9pt; color:#636363;\">" + tr("Endereço: ") + "</span><span style=\" font-size:9pt;"
            "color:#121315;\">" + address + "</span></p>"
            "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
            "text-indent:0px;\"><span style=\" font-size:9pt; color:#636363;\">" + tr("Bairro: ") + "</span><span style=\" font-size:9pt;"
            "color:#121315;\">" + district + "</span></p>"
            "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
            "text-indent:0px;\"><span style=\" font-size:9pt; color:#636363;\">" + tr("Cargo: ") + "</span><span style=\" font-size:9pt;"
            "color:#121315;\">" + jobRole + "</span></p>"
            "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
            "text-indent:0px;\"><span style=\" font-size:9pt; color:#636363;\">" + tr("Comissão: ") + "</span><span style=\" font-size:9pt;"
            "color:#121315;\">" + commission + "%</span></p>"
            "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;";

    if (! bank.isEmpty()){
        html += "text-indent:0px;\"><span style=\" font-size:9pt; color:#636363;\">" + tr("Banco: ") + "</span><span style=\" font-size:9pt;"
                "color:#121315;\">" + bank + "</span></p>"
                "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
                "text-indent:0px;\"><span style=\" font-size:9pt; color:#636363;\">" + tr("Conta: ") + "</span><span style=\" font-size:9pt;"
                "color:#121315;\">" + account + "</span></p>"
                "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
                "text-indent:0px;\"><span style=\" font-size:9pt; color:#636363;\">" + tr("Agência: ") + "</span><span style=\" font-size:9pt;"
                "color:#121315;\">" + agency + "</span></p>"
                "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                "-qt-block-indent:0; text-indent:0px; font-size:9pt; color:#636363;\"></p>"
                "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;";
    }

    if (! note.isEmpty()){
        html += "-qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:9pt; color:#636363;\">" + tr("Anotação") + "</span></p>"
                "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px;"
                "margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt; color:#636363;\"></p>"
                "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                "-qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:9pt; color:#121315;\">" + note + "</span></p>"
                "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px;"
                "margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt; color:#636363;\"></p></body></html>";
    }

    tbProfile->setHtml(html);
}

void FunctionaryInterface::onClick_pbAvailable()
{
    QString white = "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            "stop: 0 rgba(255, 255, 255, 50), stop: 1 rgba(0, 0, 0, 50));"
            "color: rgb(50,50,50);"
            "border: 1px solid rgba(0, 0, 0, 50);"
            "border-radius: 5px;";

    QString red = "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            "stop: 1 rgba(165, 47, 31, 240), stop: 0 rgba(159, 45, 29, 240));"
            "color: rgb(230,230,230);"
            "border: 1px solid rgba(123, 31, 31, 255);"
            "border-radius: 5px;";

    pbAvailable->setStyleSheet(red);

    pbUnavailable->setStyleSheet(white);

    employee->setFilterStatus(true);
}

void FunctionaryInterface::onClick_pbUnavailable()
{
    QString white = "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            "stop: 0 rgba(255, 255, 255, 50), stop: 1 rgba(0, 0, 0, 50));"
            "color: rgb(50,50,50);"
            "border: 1px solid rgba(0, 0, 0, 50);"
            "border-radius: 5px;";

    QString red = "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            "stop: 1 rgba(165, 47, 31, 240), stop: 0 rgba(159, 45, 29, 240));"
            "color: rgb(230,230,230);"
            "border: 1px solid rgba(123, 31, 31, 255);"
            "border-radius: 5px;";

    pbUnavailable->setStyleSheet(red);

    pbAvailable->setStyleSheet(white);

    employee->setFilterStatus(false);
}

void FunctionaryInterface::getSearchFunctionary(QString text)
{
    if (text.length() > 0)
        employee->setFilter("lower(name) like lower('" + text + "%')");
    else
        employee->clearFilter();
}

void FunctionaryInterface::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete)
        onClick_lRemove();

    if (event->key() == Qt::Key_F11)
        emit fullScreen();

    switch (event->modifiers()){

    case Qt::ControlModifier:{
        if (event->key() == Qt::Key_S)
            onClick_pbConfirm();

        if (event->key() == Qt::Key_L)
            onClick_pbLeave();

        if (event->key() == Qt::Key_P)
            onClick_lProfile();

        if (event->key() == Qt::Key_Tab)
            emit goToPanel(9);

        break;
    }
    }

    if (event->key() == Qt::Key_Backtab)
        emit goToPanel(7);
}



void FunctionaryInterface::onClick_pbCalculateCommission()
{
    if (tableEmployee->currentIndex().row() < 0){
        QMessageBox::warning(this, tr("Selecione um funcionário"), tr("Você precisa selecionar um funcionário para calcular sua comissão do período."), QMessageBox::Ok);

        return;
    }

    QString id = tableEmployee->model()->index(row(), 0).data().toString();

    double commission = tableEmployee->model()->index(row(), 11).data().toDouble();

    if (commission == 0){
        QMessageBox::warning(this, tr("Funcionário não recebe comissão"), tr("O funcionário selecionado não recebe comissões."), QMessageBox::Ok);

        return;
    }

    double valueCommission = employee->getValueCommission(id, commission, eBeforeDateCommission->text(), eAfterDateCommission->text());

    lValueCommission->setText(tools.convertMoney(QString::number(valueCommission)));

}

int FunctionaryInterface::row()
{
    return tableEmployee->currentIndex().row();
}

void FunctionaryInterface::logout()
{
    onClick_pbLeave();
}
