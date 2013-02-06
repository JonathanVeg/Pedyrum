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


 #include "orderedinterface.h"
#include <QMessageBox>
#include <QPropertyAnimation>

OrderedInterface::OrderedInterface(QWidget *parent) :
    QWidget(parent)
{
    createComponent();

    connections();

    statics();
}

void OrderedInterface::createComponent()
{
    makeHTML = new MakeHTML();

    ordered = new Ordered();

    lTop = new QLabel(this);

    lAdd = new Label(this,true,100);
    lRemove = new Label(this,true,100);
    lProfile = new Label(this,true,100);
    textCountOrdered = new QTextBrowser(this);

    searchOrdered = new Search(this,tr("Pesquisar Pedidos"));
    actionPhone = new QAction(tr("Pesquisar por Telefone"),this);
    actionDate = new QAction(tr("Pesquisar por Data"),this);
    actionHour = new QAction(tr("Pesquisar por Hora"),this);

    tableOrdered = new TableView(this);

    cbShowType = new ComboBox(this);
    cbShowStatus = new ComboBox(this);

    pbToday = new QPushButton(this);
    pbAll = new QPushButton(this);

    wViewOrdered = new QWidget(this);
    lViewOrdered = new QLabel(wViewOrdered);
    lStatusStatic = new QLabel(wViewOrdered);
    lStatus = new QLabel(wViewOrdered);
    pbPDF = new QPushButton(wViewOrdered);
    pbPrint = new QPushButton(wViewOrdered);
    teView = new QTextBrowser(wViewOrdered);

    wFunctionary = new QWidget(this);
    lFunctionary = new QLabel(wFunctionary);
    searchFunctionary = new Search(wFunctionary,tr("Pesquisar Funcionário"), true);
    cbStatus = new ComboBox(wFunctionary);
    pbFree = new QPushButton(wFunctionary);
    pbOccupied = new QPushButton(wFunctionary);
    tableFunctionary = new TableView(wFunctionary);

    selectDeliverer = new SelectDeliverer(this);
}

void OrderedInterface::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Delete)
        onClick_lRemove();

    if (event->key() == Qt::Key_F3)
        onClick_lProfile();

    switch (event->modifiers()){

    case Qt::ControlModifier:{

        if (event->key() == Qt::Key_H)
            onClick_pbToday();

        if (event->key() == Qt::Key_T)
            onClick_pbAll();

        if (event->key() == Qt::Key_P)
            onClick_pbPrint();

        if (event->key() == Qt::Key_S)
            onClick_pbPDF();

        if (event->key() == Qt::Key_N)
            onClick_lAdd();

        if (event->key() == Qt::Key_Tab)
            emit goToPanel(8);

        break;
    }
    }

    if (event->key() == Qt::Key_Backtab)
        emit goToPanel(6);

    if (event->key() == Qt::Key_F11)
        emit fullScreen();
}

void OrderedInterface::input()
{
    setProfileVisible(false);

    isShowDeliverer = false;

    lViewOrdered->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.498,"
                                "y1:0, x2:0.494636, y2:1, stop:0 rgba(230, 230, 230, 230),"
                                "stop:0.982955 rgba(200, 200, 200, 200));"
                                "border-radius: 5px;border: 1px solid rgba(255,255,255,255);");

    lFunctionary->setStyleSheet(lViewOrdered->styleSheet());

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

    searchOrdered->setAction(actionPhone);
    searchOrdered->setAction(actionDate);
    searchOrdered->setAction(actionHour);

    actionPhone->setCheckable(true);
    actionPhone->setChecked(true);
    actionPhone->setEnabled(false);

    actionDate->setCheckable(true);
    actionDate->setChecked(false);
    actionDate->setEnabled(true);

    actionHour->setCheckable(true);
    actionHour->setChecked(false);
    actionHour->setEnabled(true);

    QFont f;

#if defined(Q_WS_X11)
    f = QFont("Sans Serif",9);
#endif

#if defined(Q_WS_WIN)
    f = QFont("MS Shell Dlq 2", 11);
#endif

    f.setBold(true);

    textCountOrdered->setText("0");
    textCountOrdered->setObjectName("textCountOrdered");
    textCountOrdered->setToolTip(tr("Número de clientes"));
    textCountOrdered->setFont(f);
    textCountOrdered->setAlignment(Qt::AlignCenter);
    textCountOrdered->setStyleSheet(this->styleSheet());

    cbShowType->addItem(tr("Tipo de Pedido"));
    cbShowType->addItem(tr("Entrega"));
    cbShowType->addItem(tr("Balcão"));
    //cbShowType->addItem(tr("Mesa"));
    cbShowType->setFont(this->font());

    cbShowStatus->addItem(tr("Exibir por Status"));
    cbShowStatus->addItem(tr("Em andamento"));
    cbShowStatus->addItem(tr("A caminho"));
    cbShowStatus->addItem(tr("Atrasado"));
    cbShowStatus->addItem(tr("Fechado"));
    cbShowStatus->addItem(tr("Cancelado"));
    cbShowStatus->addItem(tr("Agendado"));
    cbShowStatus->setFont(this->font());

    pbToday->setText(tr("Hoje"));
    pbToday->setFont(this->font());
    pbToday->setCursor(Qt::PointingHandCursor);

    pbAll->setText(tr("Tudo"));
    pbAll->setFont(this->font());
    pbAll->setCursor(Qt::PointingHandCursor);

    pbToday->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                           "stop: 0 rgba(255, 255, 255, 50), stop: 1 rgba(0, 0, 0, 50));"
                           "color: rgb(50,50,50);"
                           "border: 1px solid rgba(0, 0, 0, 50);"
                           "border-radius: 5px;");

    pbAll->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                         "stop: 1 rgba(165, 47, 31, 240), stop: 0 rgba(159, 45, 29, 240));"
                         "color: rgb(230,230,230);"
                         "border: 1px solid rgba(123, 31, 31, 255);"
                         "border-radius: 5px;");

    pbPrint->setText(tr("Imprimir"));
    pbPrint->setCursor(Qt::PointingHandCursor);

    pbPDF->setText(tr("PDF"));
    pbPDF->setCursor(Qt::PointingHandCursor);

#if defined(Q_WS_X11)
    f = QFont("Sans Serif",9);
#endif

#if defined(Q_WS_WIN)
    f = QFont("MS Shell Dlq 2", 11);
#endif

    lStatusStatic->setText(tr("Status"));
    lStatusStatic->setFont(f);

    f.setPixelSize(f.pointSize() + 3);

    lStatus->setText(tr("Status"));
    lStatus->setFont(f);

    lStatus->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.498,"
                           "y1:0, x2:0.494636, y2:1, stop:0 rgba(0, 0, 0, 210),"
                           "stop:0.982955 rgba(0, 0, 0, 170));"
                           "border-radius: 15px;border: 1px solid rgba(0,0,0,0);"
                           "color: white;");

    lStatus->setAlignment(Qt::AlignCenter);

    teView->setStyleSheet("background-color: qlineargradient"
                          "(spread:pad, x1:0, y1:1, x2:0.165045,"
                          "y2:0, stop:0 rgba(254, 255, 180, 255),"
                          "stop:0.721591 rgba(255, 250, 205, 255));"
                          "border-radius: 10px;border: 1px solid #C0C0C0;");

    cbStatus->add(tr("Status"));
    cbStatus->add(tr("Disponível"));
    cbStatus->add(tr("Ocupado"));

    pbFree->setText(tr("Livre"));
    pbFree->setCursor(Qt::PointingHandCursor);
    pbFree->setStyleSheet("border-width: 1px;"
                          "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                          "stop:1 rgba(165, 47, 31, 255), stop:0 rgba(180, 60, 40, 255));"
                          "color: rgba(255, 255, 255, 240);"
                          "border: 1px solid rgba(0, 0, 0,15);"
                          "border-bottom: 1px solid rgba(0, 0, 0,40);"
                          "border-radius: 3px;");

    pbOccupied->setText(tr("Ocupado"));
    pbOccupied->setCursor(Qt::PointingHandCursor);

    tableOrdered->setModel(ordered->bd.getModelQuery("ordered"));
    tableOrdered->setColumnHidden(0, true);

    insertItemsInTable();

    onClick_pbToday();

    isToday = true;

    tableFunctionary->setModel(ordered->bd.getModelQuery("statusEmployee"));

    tableFunctionary->setColumnHidden(0, true);

    searchFunctionary->eSearch->setCompleter(ordered->bd.getCompleter("statusEmployee"));

    selectDeliverer->setGeometry(this->width()/2 - 120,0,240,0);

    onClick_lProfile();
}

void OrderedInterface::connections()
{
    connect(pbToday,SIGNAL(clicked()),SLOT(onClick_pbToday()));

    connect(pbAll,SIGNAL(clicked()),SLOT(onClick_pbAll()));

    connect(lAdd, SIGNAL(onClick()), SLOT(onClick_lAdd()));

    connect(lRemove, SIGNAL(onClick()), SLOT(onClick_lRemove()));

    connect(lProfile, SIGNAL(onClick()), SLOT(onClick_lProfile()));

    connect(tableOrdered, SIGNAL(clicked(QModelIndex)), SLOT(clicked_tableOrdered(QModelIndex)));

    connect(tableFunctionary, SIGNAL(clicked(QModelIndex)), SLOT(clicked_tableFunctionary(QModelIndex)));

    connect(cbShowType, SIGNAL(currentIndexChanged(int)), SLOT(setFilterModel()));

    connect(cbShowStatus, SIGNAL(currentIndexChanged(int)), SLOT(setFilterModel()));

    connect(selectDeliverer, SIGNAL(sendDeliverer(QString)), SLOT(updateDataFuncionary(QString)));

    connect(pbFree, SIGNAL(clicked()), SLOT(onClick_pbFree()));

    connect(pbOccupied, SIGNAL(clicked()), SLOT(onClick_pbOccupied()));

    connect(cbStatus, SIGNAL(currentIndexChanged(QString)), SLOT(currentIndexChanged_cbStatus(QString)));

    connect(searchFunctionary->eSearch, SIGNAL(textChanged(QString)), SLOT(getSearchEmployee(QString)));

    connect(actionPhone,SIGNAL(triggered()),this,SLOT(onTriggered_actionPhone()));

    connect(actionDate,SIGNAL(triggered()),this,SLOT(onTriggered_actionDate()));

    connect(actionHour,SIGNAL(triggered()),this,SLOT(onTriggered_actionHour()));

    connect(tableOrdered, SIGNAL(doubleClicked(QModelIndex)), SLOT(doubleClicked_tableOrdered(QModelIndex)));

    connect(pbPrint, SIGNAL(clicked()), SLOT(onClick_pbPrint()));

    connect(pbPDF, SIGNAL(clicked()), SLOT(onClick_pbPDF()));

    connect(tableOrdered, SIGNAL(rowChanged(int)), SLOT(rowChanged_tableOrdered(int)));
}

void OrderedInterface::statics()
{
    lAdd->setGeometry(12,11,30,28);
    lRemove->setGeometry(lAdd->width() + lAdd->x() + 12,11,30,28);
    lProfile->setGeometry(lRemove->width() + lRemove->x() + 12,11,30,28);

    lStatusStatic->setGeometry(30,15,100,20);
    lStatus->setGeometry(50,lStatusStatic->y() + lStatusStatic->height() + 10,180,40);

    pbPDF->setGeometry(70,lStatus->y() + lStatus->height() + 10,60,28);
    pbPrint->setGeometry(pbPDF->x()+ pbPDF->width() + 20,pbPDF->y(),60,28);

    searchFunctionary->setGeometry(15, 15, 250, 26);
    cbStatus->setGeometry(80, searchFunctionary->y() + searchFunctionary->height() + 12, 120, 28);

    pbFree->setGeometry(59, cbStatus->y() + cbStatus->height() + 12, 80, 28);

    pbOccupied->setGeometry(pbFree->x() + pbFree->width() + 2,pbFree->y(),80,28);

}

void OrderedInterface::insertItemsInTable(){
    for (int i = 0; i < tableOrdered->model()->rowCount(); i ++){
    }
}

void OrderedInterface::adjustLayout()
{
    lTop->setGeometry(0,0,this->width(),50);

    searchOrdered->setGeometry(this->width() - 220,11,200,28);

    textCountOrdered->setGeometry(lProfile->width() + lProfile->x() + 12,11,textCountOrderedSize(),28);

    cbShowType->setGeometry(textCountOrdered->width() + textCountOrdered->x() + 12,11,120,28);

    cbShowStatus->setGeometry(cbShowType->width() + cbShowType->x() + 12,11,120,28);

    pbToday->setGeometry(cbShowStatus->width() + cbShowStatus->x() + 12,11,50,28);

    pbAll->setGeometry(pbToday->width() + pbToday->x() + 2,11,50,28);

    if(isProfileShow)
    {
        wViewOrdered->setGeometry(this->width() - 280, lTop->height(), 280,this->height() - lTop->height());

        tableOrdered->setGeometry(280,lTop->height(),this->width() - (wViewOrdered->width() + 280),this->height() - lTop->height());
    }
    else
        tableOrdered->setGeometry(280,lTop->height(),this->width() - 280,this->height() - lTop->height());

    lViewOrdered->setGeometry(5,5,wViewOrdered->width() - 10, wViewOrdered->height() - 10);

    teView->setGeometry(10,130,260,this->height() - (lTop->height() + 140));

    wFunctionary->setGeometry(0,lTop->height(),280,this->height() - lTop->height());

    lFunctionary->setGeometry(5,5,wFunctionary->width() - 10, wFunctionary->height() - 10);

    tableFunctionary->setGeometry(10,130,260,this->height() - (lTop->height() + 140));

    tableOrdered->setColumnWidth(1, tableOrdered->width() * 0.15);
    tableOrdered->setColumnWidth(2, tableOrdered->width() * 0.15);
    tableOrdered->setColumnWidth(3, tableOrdered->width() * 0.15);
    tableOrdered->setColumnWidth(4, tableOrdered->width() * 0.15);
    tableOrdered->setColumnWidth(5, tableOrdered->width() * 0.20);
    tableOrdered->setColumnWidth(6, tableOrdered->width() * 0.20);

    tableFunctionary->setColumnWidth(1, tableFunctionary->width() * 0.65 -1);
    tableFunctionary->setColumnWidth(2, tableFunctionary->width() * 0.35 -1);

    if(isShowDeliverer)
        selectDeliverer->setGeometry(this->width()/2 - 120,200,240,180);
    else
        selectDeliverer->setGeometry(this->width()/2 - 120,-180,240,180);
}

void OrderedInterface::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void OrderedInterface::onClick_pbAll()
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

    pbToday->setStyleSheet(white);

    pbAll->setStyleSheet(red);

    isToday = false;

    setFilterModel();
}

void OrderedInterface::onClick_pbToday()
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

    pbToday->setStyleSheet(red);

    pbAll->setStyleSheet(white);

    isToday = true;

    setFilterModel();
}

void OrderedInterface::setProfileVisible(bool b)
{
    isProfileShow = b;

    lProfile->setAlternate(isProfileShow);

    wViewOrdered->setVisible(b); 
}

void OrderedInterface::onClick_lAdd()
{
    emit gotoPanel(3, 0, 0);
}

void OrderedInterface::onClick_lRemove()
{
    if (tableOrdered->currentIndex().row() < 0){
        QMessageBox::warning(this, tr("Selecione um pedido"), tr("Selecione algum pedido para excluir."), QMessageBox::Ok);

        return;
    }

    int op = QMessageBox::question(this, tr("Apagar pedido"), tr("Tem certeza que deseja apagar o pedido selecionado?"), tr("Sim"), tr("Não"));

    if (op == 1)
        return;

    if (! ordered->remove(tableOrdered->model()->index(tableOrdered->currentIndex().row(), 0).data().toString())){

        QMessageBox::warning(this, tr("Pedido não deletado"), tr("Seu pedido não pode ser apagado."), QMessageBox::Ok);

    }
}

void OrderedInterface::onClick_lProfile()
{
    teView->setHtml(makeHTML->createHTMLOrder(currentIdOrder.toInt()));

    if(isProfileShow)
        setProfileVisible(false);
    else
        setProfileVisible(true);

    adjustLayout();

}

void OrderedInterface::clicked_tableOrdered(QModelIndex index){
    currentIdOrder = tableOrdered->model()->index(index.row(), 0).data().toString();

    cbChangeStatusDelegate = new ComboBox();

    cbChangeStatusDelegate->setAutoFillBackground(true);

    cbChangeStatusDelegate->add(tr("Em andamento"));
    cbChangeStatusDelegate->add(tr("A caminho"));
    cbChangeStatusDelegate->add(tr("Atrasado"));
    cbChangeStatusDelegate->add(tr("Fechado"));
    cbChangeStatusDelegate->add(tr("Cancelado"));

    cbChangeStatusDelegate->setFont(this->font());

    tableOrdered->setIndexWidget(tableOrdered->model()->index(index.row(), 5), cbChangeStatusDelegate);

    lStatus->setText(tableOrdered->model()->index(index.row(), 5).data().toString());

    cbChangeStatusDelegate->setCurrentText(tableOrdered->model()->index(index.row(), 5).data().toString());

    connect(cbChangeStatusDelegate, SIGNAL(currentIndexChanged(QString)), SLOT(currentIndexChanged_cbChangeStatusDelegate(QString)));
}

void OrderedInterface::currentIndexChanged_cbChangeStatusDelegate(QString text){

    try{
        delete cbChangeStatusDelegate;
    }catch(...){
        qDebug() << "[OrderedInterface] [delete cbChangeStatusDelegate]";
    }

    if (text == "A caminho"){

        QString sql;

        sql = QString("select * from employeeorder where idorder = '%1'").arg(currentIdOrder);

        if(! ordered->bd.search(sql)){

            sql = "select e.idEmployee, e.name from employee e, statusEmployee se where e.idemployee = se.idemployee and se.status = 'Disponível' and e.jobrole = 'Entregador'";

            if (ordered->bd.search(sql)){

                selectDeliverer->setWaiters(ordered->bd.loadColumn(1), ordered->bd.loadColumn(0));

                ordered->changeStatusOrder(currentIdOrder, text);

                showSelectDeliveres();

            }
            else{
                QMessageBox::warning(this, tr("Indisponível"), tr("Nenhum entregador disponível no momento."), QMessageBox::Ok);
            }
        }
    }else{
        if (text == "Fechado"){
            if (ordered->bd.search("select * from employeeorder where idorder = " + currentIdOrder, 1)){
                ordered->bd.update("statusemployee", "status", "Disponível", "idemployee", ordered->bd.loadSimple(1, 1));

                ordered->bd.refreshQueryModel("statusEmployee");
            }
        }

        ordered->changeStatusOrder(currentIdOrder, text);

    }
}

void OrderedInterface::setFilterModel()
{
    QString sql;

    sql = "select idOrder, dateorder, timeorder, typeorder, valueorder, statusOrder, numberorder from ordered ";

    if (cbShowStatus->currentIndex() > 0){
        sql += "where statusorder = '" + cbShowStatus->currentText() + "' and ";

        if (cbShowType->currentIndex() > 0){
            sql += "typeorder = '" + cbShowType->currentText() + "' and ";
        }
    }else if (cbShowType->currentIndex() > 0){
        sql += " where typeorder = '" + cbShowType->currentText() + "' and ";
    }

    if ((! isToday) && (sql.indexOf("where") > 0))
        sql = sql.mid(0, sql.length() - 5);
    else if(isToday){
        if (sql.indexOf("where") < 0)
            sql += " where ";

        sql += " dateorder = '" + tools.convertDate(tools.getDate(Qt::SystemLocaleShortDate)) + "'";
    }

    sql += " order by dateorder";

    ordered->bd.updateModelQuery("ordered", sql);
}

void OrderedInterface::updateDataFuncionary(QString id)
{
    /// não põe mais o cara como ocupado automaticamente, pois ele pode fazer mais de uma entrega ao mesmo tempo.
    //    ordered->changeStatusEmployee(id.toInt(), "Ocupado");

    /// verificar se ja tem esse idorder cadastrado. se tiver, edita-lo

    QString sql = "insert into employeeorder values(" + currentIdOrder + ", " + id + ")";

    ordered->bd.exec(sql);
}

void OrderedInterface::onClick_pbFree()
{
    if (tableFunctionary->currentIndex().row() >= 0){
        ordered->bd.update("statusEmployee", "status", "Disponível", "idemployee", tableFunctionary->model()->index(tableFunctionary->currentIndex().row(), 0).data().toString());

        ordered->bd.refreshQueryModel("statusEmployee");

        pbFree->setStyleSheet("border-width: 1px;"
                              "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                              "stop:1 rgba(165, 47, 31, 255), stop:0 rgba(180, 60, 40, 255));"
                              "color: rgba(255, 255, 255, 240);"
                              "border: 1px solid rgba(0, 0, 0,15);"
                              "border-bottom: 1px solid rgba(0, 0, 0,40);"
                              "border-radius: 3px;");

        pbOccupied->setStyleSheet(this->styleSheet());
    }
}

void OrderedInterface::onClick_pbOccupied()
{
    if (tableFunctionary->currentIndex().row() >= 0){
        ordered->bd.update("statusEmployee", "status", "Ocupado", "idemployee", tableFunctionary->model()->index(tableFunctionary->currentIndex().row(), 0).data().toString());

        ordered->bd.refreshQueryModel("statusEmployee");

        pbOccupied->setStyleSheet("border-width: 1px;"
                                  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                  "stop:1 rgba(165, 47, 31, 255), stop:0 rgba(180, 60, 40, 255));"
                                  "color: rgba(255, 255, 255, 240);"
                                  "border: 1px solid rgba(0, 0, 0,15);"
                                  "border-bottom: 1px solid rgba(0, 0, 0,40);"
                                  "border-radius: 3px;");

        pbFree->setStyleSheet(this->styleSheet());
    }
}

void OrderedInterface::currentIndexChanged_cbStatus(QString text)
{
    Q_UNUSED(text);

    int index = cbStatus->currentIndex();

    switch (index){

    case 0:{
        ordered->bd.updateModelQuery("statusEmployee", "select e.idEmployee, e.name, se.status from employee e, statusEmployee se where lower(e.jobrole) = 'entregador' and e.idemployee = se.idemployee order by e.name");

        break;
    }

    case 1:{
        ordered->bd.updateModelQuery("statusEmployee", "select e.idEmployee, e.name, se.status from employee e, statusEmployee se where lower(e.jobrole) = 'entregador' and e.idemployee = se.idemployee and se.status = 'Disponível' order by e.name");

        break;
    }

    case 2:{
        ordered->bd.updateModelQuery("statusEmployee", "select e.idEmployee, e.name, se.status from employee e, statusEmployee se where lower(e.jobrole) = 'entregador' and e.idemployee = se.idemployee and se.status = 'Ocupado' order by e.name");

        break;
    }

    }
}

void OrderedInterface::getSearchEmployee(QString text)
{
    cbStatus->setCurrentIndex(0);

    if (searchFunctionary->eSearch->isEmpty()){
        ordered->bd.updateModelQuery("statusEmployee", "select e.idEmployee, e.name, se.status from employee e, statusEmployee se where lower(e.jobrole) = 'entregador' and e.idemployee = se.idemployee order by e.name");

        return;
    }

    ordered->bd.updateModelQuery("statusEmployee", "select e.idEmployee, e.name, se.status from employee e, statusEmployee se where lower(e.jobrole) = 'entregador' and e.idemployee = se.idemployee and lower(e.name) like lower('" + text + "%') order by e.name");

}

void OrderedInterface::doubleClicked_tableOrdered(QModelIndex index)
{
    int row = index.row();

    if (row < 0)
        return;

    QString idOrder = tableOrdered->model()->index(row, 0).data().toString();

    if (ordered->bd.search("select * from treatmenttable where idorder = " + idOrder)){
        QMessageBox::warning(this, "Não editável", "Pedido de mesa não pode ser editado.", QMessageBox::Ok);

        return;
    }


    int op = QMessageBox::question(this, tr("Editar pedido"), tr("Tem certeza que deseja editar esse pedido?"), tr("Sim"), tr("Não"));

    if (op == 1)
        return;

    QString typeOrder = tableOrdered->model()->index(row, 3).data().toString();

    if (typeOrder == "Delivery"){
        emit gotoPanel(3, 0, tableOrdered->model()->index(row, 0).data().toInt());
    }else if (typeOrder == "Mesa"){
        return; // se for mesa, ele nem sai da tela.
    }else if (typeOrder == "Balcão"){
        emit gotoPanel(3, 2, tableOrdered->model()->index(row, 0).data().toInt());
    }
}

void OrderedInterface::clicked_tableFunctionary(QModelIndex index)
{
    if (tableFunctionary->model()->index(index.row(), 2).data().toString() == "Disponível")
        onClick_pbFree();
    else
        onClick_pbOccupied();

    tableFunctionary->setFocus();
    tableFunctionary->setCurrentIndex(index);
}

void OrderedInterface::onTriggered_actionPhone()
{
    actionPhone->setChecked(true);
    actionPhone->setEnabled(false);

    actionDate->setChecked(false);
    actionDate->setEnabled(true);

    actionHour->setChecked(false);
    actionHour->setEnabled(true);
}

void OrderedInterface::onTriggered_actionDate()
{
    actionPhone->setChecked(false);
    actionPhone->setEnabled(true);

    actionDate->setChecked(true);
    actionDate->setEnabled(false);

    actionHour->setChecked(false);
    actionHour->setEnabled(true);
}

void OrderedInterface::onTriggered_actionHour()
{
    actionPhone->setChecked(false);
    actionPhone->setEnabled(true);

    actionDate->setChecked(false);
    actionDate->setEnabled(true);

    actionHour->setChecked(true);
    actionHour->setEnabled(false);
}

void OrderedInterface::updateCountOrdered(QString qnt)
{
    QFont f;

#if defined(Q_WS_X11)
    f = QFont("Sans Serif",9);
#endif

#if defined(Q_WS_WIN)
    f = QFont("MS Shell Dlq 2", 11);
#endif

    f.setBold(true);

    textCountOrdered->setText(qnt);
    textCountOrdered->setFont(f);
    textCountOrdered->setAlignment(Qt::AlignHCenter);

    adjustLayout();
}

int OrderedInterface::textCountOrderedSize()
{
    short int pointSize;

#if defined(Q_WS_X11)
    pointSize = 9;
#endif

#if defined(Q_WS_WIN)
    pointSize = 11;
#endif

    int value;

    if(textCountOrdered->toPlainText().length() <= 1)
        value = 30;
    else
        value = 12 + textCountOrdered->toPlainText().length() * pointSize;

    return value;
}

void OrderedInterface::onClick_pbPrint()
{
    qDebug() << "currentIdOrder" << currentIdOrder;
    makeHTML->loadNumberOfCopy();
    makeHTML->print(makeHTML->createHTMLOrder(currentIdOrder.toInt()));
}

void OrderedInterface::onClick_pbPDF()
{
    makeHTML->createPDF(makeHTML->createHTMLOrder(currentIdOrder.toInt()));
}

void OrderedInterface::rowChanged_tableOrdered(int row)
{
    if (row < 0)
        return;

    currentIdOrder = tableOrdered->model()->index(row, 0).data().toString();

    lStatus->setText(tableOrdered->model()->index(row, 5).data().toString());

    teView->setHtml(makeHTML->createHTMLOrder(currentIdOrder.toInt()));
}

void OrderedInterface::showSelectDeliveres()
{
    QPropertyAnimation* show;

    show = new QPropertyAnimation(selectDeliverer, "geometry");

    isShowDeliverer = true;

    show->setStartValue(QRect(this->width()/2 - 120,0,240,0));
    show->setEndValue(QRect(this->width()/2 - 120,200,240,180));
    show->setEasingCurve(QEasingCurve::OutSine);
    show->setDuration(1250);
    show->start();
}
