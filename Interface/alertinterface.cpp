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


 #include "alertinterface.h"
#include <QDebug>
#include <QMessageBox>

AlertInterface::AlertInterface(QWidget *parent) :
    QWidget(parent)
{
    createComponent();
    connections();
    statics();
}

void AlertInterface::createComponent()
{
    alert = new Alert();

    lTop = new QLabel(this);

    lAdd = new Label(this,true,100);
    lRemove = new Label(this,true,100);
    lProfile = new Label(this,true,100);
    textCountAlert = new QTextBrowser(this);

    pbNewsAlerts = new QPushButton(this);
    pbAllAlerts = new QPushButton(this);

    cbIsRead = new QCheckBox(tr("Marcar como lido"),this);

    pbAlterStatus = new QPushButton(tr("Marcar como lido"), this);

    wSeletorDate =  new SeletorDate(this);

    searchAlert = new Search(this,tr("Pesquisar Alertas"));
    actionTitle = new QAction(tr("Pesquisar por Título"),this);
    actionSubject = new QAction(tr("Pesquisar por Assunto"),this);
    actionDate = new QAction(tr("Pesquisar por Data"),this);
    actionHour = new QAction(tr("Pesquisar por Hora"),this);

    tableAlert = new TableView(this);

    wSpace = new QWidget(this);
    lBackgroundSpace = new QLabel(wSpace);

    splitter = new QSplitter(Qt::Vertical,this);
    swPanels = new QStackedWidget(wSpace);

    wGroupPlus = new QWidget(wSpace);
    lGroupPlus = new QLabel(wGroupPlus);
    lAlertDateText = new QLabel(wGroupPlus);
    eTitle = new Edit(wGroupPlus,true,tr("Título")), 80;
    eSubject = new Edit(wGroupPlus,true,tr("Assunto"), 50);
    eDate = new Edit(wGroupPlus,true,tr("Data"), 10);
    eHour = new Edit(wGroupPlus, true, tr("Hora"), 5);
    eDateAlert = new Edit(wGroupPlus,true,tr("Data"), 10);
    eHourAlert = new Edit(wGroupPlus,true, tr("Hora"), 5);
    teNotes = new QTextEdit(wGroupPlus);
    cbPriority = new ComboBox(wGroupPlus);

    pbLeave = new QPushButton(wGroupPlus);
    pbConfirm = new QPushButton(wGroupPlus);

    wGroupProfile = new QWidget(wSpace);
    tbProfile = new QTextBrowser(wGroupProfile);


    wBar = new QWidget(this);
    lBar = new QLabel(wBar);
    lHours = new QLabel(wBar);
    lDate = new QLabel(wBar);
    cwCalendar = new QCalendarWidget(wBar);

    swTasks = new QStackedWidget(wBar);

    wlistTasks = new QWidget(wBar);
    listText = new QLabel(wBar);
    listTasks = new ListView(wlistTasks);
    pbAddList = new QPushButton(wlistTasks);
    pbRemoveList = new QPushButton(wlistTasks);

    wTaskEdit = new QWidget(wBar);
    teEditTask = new QTextEdit(wTaskEdit);
    pbSaveEditList = new QPushButton(wTaskEdit);

    timer = new QTimer();
    verificationAlert = new QTimer();        

    wBetweenDate = new BetweenDate(this);
}

void AlertInterface::keyPressEvent(QKeyEvent *event){

    if (event->key() == Qt::Key_F12){
        cbIsRead->setVisible(! cbIsRead->isVisible());

        pbAlterStatus->setVisible(! pbAlterStatus->isVisible());
    }

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
            emit goToPanel(10);

        break;
    }
    }

    if (event->key() == Qt::Key_Backtab)
        emit goToPanel(8);
}

void AlertInterface::input()
{
    isAdd = true;

    typeS = TITLE;

    nextPanelIndex = 0;

    timer->setInterval(60000);
    timer->start();

    verificationAlert->setInterval(60000);
    verificationAlert->start();

    lHours->setText(tools.currentTime());

    lDate->setText(tools.upFirstLetter(tools.getDate(Qt::SystemLocaleLongDate)));

    tableAlert->setModel(alert->bd.getModel("alert"));

    tableAlert->setColumnHidden(0,true);
    tableAlert->setColumnHidden(5,true);
    tableAlert->setColumnHidden(6,true);
    tableAlert->setColumnHidden(8,true);

    listTasks->setModel(alert->bd.getModel("taskSimple"));
    listTasks->setModelColumn(2);

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

    QFont f;

#if defined(Q_WS_X11)
    f = QFont("Sans Serif",9);
#endif

#if defined(Q_WS_WIN)
    f = QFont("MS Shell Dlq 2", 11);
#endif

    f.setBold(true);

    textCountAlert->setObjectName("textCountAlert");
    textCountAlert->setToolTip(tr("Número de Alertas"));
    textCountAlert->setFont(f);
    textCountAlert->setAlignment(Qt::AlignHCenter);
    textCountAlert->setStyleSheet(this->styleSheet());

    searchAlert->setAction(actionTitle);
    searchAlert->setAction(actionSubject);
    searchAlert->setAction(actionDate);
    searchAlert->setAction(actionHour);

    actionTitle->setCheckable(true);
    actionTitle->setChecked(true);
    actionTitle->setEnabled(false);

    actionSubject->setCheckable(true);
    actionSubject->setChecked(false);
    actionSubject->setEnabled(true);

    actionDate->setCheckable(true);
    actionDate->setChecked(false);
    actionDate->setEnabled(true);

    actionHour->setCheckable(true);
    actionHour->setChecked(false);
    actionHour->setEnabled(true);

    lBackgroundSpace->setStyleSheet("background-color: rgba(123, 31, 31, 255);");

    swPanels->addWidget(wGroupPlus);
    swPanels->addWidget(wGroupProfile);

    lGroupPlus->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.498,"
                              "y1:0, x2:0.494636, y2:1, stop:0 rgba(255, 255, 255, 210),"
                              "stop:0.982955 rgba(255, 255, 255, 170));"
                              "border-radius: 5px;border: 1px solid rgba(255,255,255,255);");

    tbProfile->setStyleSheet(lGroupPlus->styleSheet());

    f.setBold(false);

    f.setPointSize(f.pointSize() - 2);

    cbIsRead->setFont(f);
    cbIsRead->setEnabled(false);

    f.setPointSize(f.pointSize() + 1);

    pbAlterStatus->setVisible(false);
    pbAlterStatus->setEnabled(false);

    lAlertDateText->setText(tr("Data que deseja ser alertado: "));
    lAlertDateText->setFont(f);

    eDate->setInputMask("  /  /    ");
    eHour->setInputMask("  :  ");
    eDateAlert->setInputMask("  /  /    ");
    eHourAlert->setInputMask("  :  ");

    teNotes->setStyleSheet("background-color: qlineargradient"
                           "(spread:pad, x1:0, y1:1, x2:0.165045,"
                           "y2:0, stop:0 rgba(254, 255, 180, 255),"
                           "stop:0.721591 rgba(255, 250, 205, 255));"
                           "border-radius: 10px;border: 1px solid #C0C0C0;");

    cbPriority->add(tr("Prioridade"));
    cbPriority->add(tr("Baixa"));
    cbPriority->add(tr("Média"));
    cbPriority->add(tr("Normal"));
    cbPriority->add(tr("Alta"));
    cbPriority->add(tr("Muito alta"));


    pbLeave->setText(tr("Limpar"));
    pbLeave->setFont(this->font());

    pbConfirm->setText(tr("Confirmar"));
    pbConfirm->setFont(this->font());

    pbNewsAlerts->setText(tr("Novos"));
    pbNewsAlerts->setFont(this->font());
    pbNewsAlerts->setCursor(Qt::PointingHandCursor);
    pbNewsAlerts->setObjectName("pbNewsAlerts");

    pbAllAlerts->setText(tr("Todos"));
    pbAllAlerts->setFont(this->font());
    pbAllAlerts->setCursor(Qt::PointingHandCursor);
    pbAllAlerts->setObjectName("pbAllAlerts");    

    pbAllAlerts->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                               "stop: 0 rgba(255, 255, 255, 50), stop: 1 rgba(0, 0, 0, 50));"
                               "color: rgb(50,50,50);"
                               "border: 1px solid rgba(0, 0, 0, 50);"
                               "border-radius: 5px;");

    pbNewsAlerts->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                "stop: 1 rgba(165, 47, 31, 240), stop: 0 rgba(159, 45, 29, 240));"
                                "color: rgb(230,230,230);"
                                "border: 1px solid rgba(123, 31, 31, 255);"
                                "border-radius: 5px;");


    lBar->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.498,"
                        "y1:0, x2:0.494636, y2:1, stop:0 rgba(230, 230, 230, 230),"
                        "stop:0.982955 rgba(200, 200, 200, 200));"
                        "border-radius: 5px;border: 1px solid rgba(255,255,255,255);");

    swTasks->addWidget(wlistTasks);
    swTasks->addWidget(wTaskEdit);



#if defined(Q_WS_X11)
    f.setPointSize(18);
#endif

#if defined(Q_WS_WIN)
    f.setPointSize(20);
#endif

    f.setBold(false);

    lHours->setFont(f);

    lHours->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0,"
                          "y1:0, x2:1, y2:0.5, stop:1 rgba(0, 0, 0, 210),"
                          "stop:0 rgba(0, 0, 0, 120));"
                          "border-radius: 8px; border: 1px solid rgba(0,0,0,0);"
                          "color: white;");


    lHours->setAlignment(Qt::AlignCenter);

    f.setPointSize(f.pointSize() - 9);

    lDate->setFont(f);
    lDate->setAlignment(Qt::AlignCenter);



    cwCalendar->setStyleSheet("alternate-background-color: rgb(212, 209, 209);"
                              "background-color: white;"
                              "color: rgb(67, 67, 67);"
                              "selection-color: rgb(223, 223, 223);"
                              "selection-background-color:qlineargradient"
                              "(spread:pad, x1:0.5, y1:0.00568182, x2:0.494,"
                              "y2:1, stop:0 rgba(81, 81, 81, 255), stop:1 rgba(61, 57, 50, 255));");

    listText->setText(tr("Lista de Tarefas"));
    listText->setFont(f);
    listText->setAlignment(Qt::AlignCenter);

    pbAddList->setText(tr("Adicionar"));
    pbAddList->setFont(this->font());

    pbRemoveList->setText(tr("Remover"));
    pbRemoveList->setFont(this->font());

    pbSaveEditList->setText(tr("Concluido"));
    pbSaveEditList->setFont(this->font());

    teEditTask->setStyleSheet("background-color: qlineargradient"
                              "(spread:pad, x1:0, y1:1, x2:0.165045,"
                              "y2:0, stop:0 rgba(254, 255, 180, 255),"
                              "stop:0.721591 rgba(255, 250, 205, 255));"
                              "border-radius: 10px;border: 1px solid #C0C0C0;");


    splitter->insertWidget(1,wSpace);
    splitter->insertWidget(2,tableAlert);
    splitter->show();
    splitter->setHandleWidth(1);
    splitter->setStretchFactor(1,2);

    wSpace->setGeometry(0,0,0,0);

    wBetweenDate->hide();

    eTitle->setNextComponent(eSubject);
    eSubject->setNextComponent(eDate);
    eDate->setNextComponent(eHour);
    eHour->setNextComponent(eDateAlert);
    eDateAlert->setNextComponent(eHourAlert);
    eHourAlert->setNextComponent(cbPriority);
    cbPriority->setNextComponent(teNotes);

    alert->getCount();

    eTitle->setFocus();

    createProfile();
}

void AlertInterface::statics()
{
    lAdd->setGeometry(12,11,30,28);
    lRemove->setGeometry(lAdd->width() + lAdd->x() + 12,11,30,28);
    lProfile->setGeometry(lRemove->width() + lRemove->x() + 12,11,30,28);

    lDate->setGeometry(140 - 125,85, 250, 20);

    cwCalendar->setGeometry(140 - 120,lDate->height() + lDate->y() + 10 , 240, 170);

    listText->setGeometry(15,cwCalendar->height() + cwCalendar->y() + 10, 250, 20);

    pbAddList->setGeometry(50,10, 80, 23);

    pbRemoveList->setGeometry(pbAddList->x() + pbAddList->width() + 20, pbAddList->y(), 80, 23);

    pbSaveEditList->setGeometry(100,pbAddList->y(),80,23);

    lGroupPlus->setGeometry(0,0,540,290);

    eTitle->setGeometry(20,20,500,28);

    eSubject->setGeometry(20,eTitle->y() + eTitle->height() + 10,200,28);

    eDate->setGeometry(20,eSubject->y() + eSubject->height() + 10,98,28);

    eHour->setGeometry(eDate->x() + eDate->width() + 5,eDate->y(),97,28);

    lAlertDateText->setGeometry(20,eHour->y() + eHour->height() + 10,205,16);

    eDateAlert->setGeometry(20,lAlertDateText->y() + lAlertDateText->height() + 10,98,28);

    eHourAlert->setGeometry(eDateAlert->x() + eDateAlert->width() + 5,eDateAlert->y(),97,28);

    teNotes->setGeometry(eHour->x() + eHour->width() + 10,eTitle->y() + eTitle->height() + 10,290,213);

    cbPriority->setGeometry(20,eHourAlert->y() + eHourAlert->height() + 10,200,28);

    pbLeave->setGeometry(20,cbPriority->y() + cbPriority->height() + 15,90,28);

    pbConfirm->setGeometry(pbLeave->x() + pbLeave->width() + 20,pbLeave->y(),90,28);

    tbProfile->setGeometry(0,0,500,265);
}

void AlertInterface::adjustLayout()
{
    lTop->setGeometry(0,0,this->width(),50);

    searchAlert->setGeometry(this->width() - 220,11,200,28);

    textCountAlert->setGeometry(lProfile->width() + lProfile->x() + 12,11,textCountAlertSize(),28);

    pbAllAlerts->setGeometry(textCountAlert->width() + textCountAlert->x() + 12,11,50,28);

    pbNewsAlerts->setGeometry(pbAllAlerts->width() + pbAllAlerts->x() + 2,11,50,28);

    cbIsRead->setGeometry(pbNewsAlerts->width() + pbNewsAlerts->x() + 12,11,125,28);

    pbAlterStatus->setGeometry(pbNewsAlerts->width() + pbNewsAlerts->x() + 12,11,125,28);

    if((cbIsRead->x() + cbIsRead->width()) >= (this->width() / 2 - 140))
        wSeletorDate->setGeometry(cbIsRead->width() + cbIsRead->x() + 12,11,280,28);
    else
        wSeletorDate->setGeometry(this->width() / 2 - 140,11,280,28);

    wBar->setGeometry(this->width() - 280, lTop->height(), 280, this->height() - lTop->height());

    lBar->setGeometry(5,5, 270, wBar->height() - 10);

    lHours->setGeometry(140 - 50, 25, 100, 50);

    swTasks->setGeometry(0,listText->height() + listText->y() + 10,280,
                         wBar->height() - (listText->height() + listText->y() + 20));

    wlistTasks->setGeometry(0,0,swTasks->width(),swTasks->height());

    wTaskEdit->setGeometry(0,0,swTasks->width(),swTasks->height());

    listTasks->setGeometry(20,pbSaveEditList->height() + pbSaveEditList->y() + 10 ,
                           240, wTaskEdit->height() - (pbRemoveList->height() + pbRemoveList->y() + 20));

    teEditTask->setGeometry(listTasks->geometry());

    splitter->setGeometry(0,lTop->height(),this->width() - wBar->width(),this->height() - lTop->height());

    wSpace->setGeometry(0,0,this->width() - wBar->width(),wSpace->height());

    lBackgroundSpace->setGeometry(0,0,wSpace->width(),wSpace->height());

    swPanels->setGeometry(0,0,wSpace->width(),wSpace->height());

    wGroupPlus->setGeometry(wSpace->width()/2 - 250,wSpace->height()/2 - 145,540,290);

    wGroupProfile->setGeometry(wSpace->width()/2 - 250,wSpace->height()/2 - 132,500,265);

    tableAlert->setGeometry(0, wSpace->y() + wSpace->height(),wSpace->width(),
                            this->height() - (wSpace->height() + lTop->height()));

    tableAlert->setColumnWidth(1,tableAlert->width() * 0.25);
    tableAlert->setColumnWidth(2,tableAlert->width() * 0.25);
    tableAlert->setColumnWidth(3,tableAlert->width() * 0.12);
    tableAlert->setColumnWidth(4,tableAlert->width() * 0.12);
    tableAlert->setColumnWidth(7,tableAlert->width() * 0.13);
    tableAlert->setColumnWidth(9,tableAlert->width() * 0.13);

    wBetweenDate->setGeometry((wSeletorDate->x() + wSeletorDate->pbPeriod->width() * 4 + wSeletorDate->pbPeriod->width()/2) - wBetweenDate->W_/2,
                              wSeletorDate->y() + wSeletorDate->height() + 3,wBetweenDate->W_,wBetweenDate->H_);
}

void AlertInterface::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void AlertInterface::connections()
{
    connect(lAdd, SIGNAL(onClick()), SLOT(onClick_lAdd()));

    connect(lRemove, SIGNAL(onClick()), SLOT(onClick_lRemove()));

    connect(lProfile, SIGNAL(onClick()), SLOT(onClick_lProfile()));

    connect(pbNewsAlerts,SIGNAL(clicked()),SLOT(onClick_pbNewsAlerts()));

    connect(pbAllAlerts,SIGNAL(clicked()),SLOT(onClick_pbAllAlerts()));

    connect(pbAddList,SIGNAL(clicked()),SLOT(onClick_pbAddList()));

    connect(pbRemoveList,SIGNAL(clicked()),SLOT(onClick_pbRemoveList()));

    connect(pbSaveEditList,SIGNAL(clicked()),SLOT(onClick_pbSaveEditList()));

    connect(splitter, SIGNAL(splitterMoved(int, int)), SLOT(adjustLayout()));

    connect(pbConfirm,SIGNAL(clicked()),SLOT(onClick_pbConfirm()));

    connect(pbLeave,SIGNAL(clicked()),SLOT(onClick_pbLeave()));

    connect(tableAlert, SIGNAL(doubleClicked(QModelIndex)), SLOT(onDoubleClick_tbAlert(QModelIndex)));

    connect(tableAlert, SIGNAL(clicked(QModelIndex)), SLOT(onClick_tbAlert(QModelIndex)));

    connect(tableAlert, SIGNAL(rowChanged(int)), SLOT(rowChanged_tbAlert(int)));

    connect(alert, SIGNAL(count(int)), this, SLOT(count(int)));

    connect(wSeletorDate, SIGNAL(selected(int)), this, SLOT(seletorDate(int)));

    connect(timer,SIGNAL(timeout()),this,SLOT(updateHours()));

    connect(verificationAlert,SIGNAL(timeout()),this,SLOT(updateVerificationAlert()));

    connect(actionTitle,SIGNAL(triggered()),this,SLOT(onTriggered_actionTitle()));

    connect(actionSubject,SIGNAL(triggered()),this,SLOT(onTriggered_actionSubject()));

    connect(actionDate,SIGNAL(triggered()),this,SLOT(onTriggered_actionDate()));

    connect(actionHour,SIGNAL(triggered()),this,SLOT(onTriggered_actionHour()));

    connect(searchAlert, SIGNAL(textSearch(QString)), SLOT(getSearchAlert(QString)));

    connect(pbAlterStatus, SIGNAL(clicked()), SLOT(onClick_pbAlterStatus()));

    connect(cbIsRead, SIGNAL(clicked()), SLOT(onClick_cbIsRead()));

    connect(wSeletorDate,SIGNAL(selected(int)),SLOT(showBetweenDate(int)));

    connect(wBetweenDate, SIGNAL(selectedDates(QString,QString)), this, SLOT(selectedDates(QString,QString)));
}

void AlertInterface::onClick_pbAddList()
{
    swTasks->setCurrentIndex(1);
}

void AlertInterface::onClick_pbSaveEditList()
{
    if (teEditTask->toPlainText().isEmpty())
        QMessageBox::warning(this, "Tarefa inválida", "Por favor, digite uma tarefa válida.");

    swTasks->setCurrentIndex(0);

    alert->task = teEditTask->toPlainText();

    alert->insertTask();

    alert->refreshModels();

    teEditTask->clear();
}

void AlertInterface::onClick_pbNewsAlerts()
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

    pbNewsAlerts->setStyleSheet(red);

    pbAllAlerts->setStyleSheet(white);

    alert->getAlertsNews(true);
}

void AlertInterface::onClick_pbAllAlerts()
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

    pbAllAlerts->setStyleSheet(red);

    pbNewsAlerts->setStyleSheet(white);

    alert->getAlertsNews(false);
}

void AlertInterface::onClick_lProfile()
{
    gotoPanel(1);
}

void AlertInterface::onClick_lRemove()
{
    QModelIndex index = tableAlert->currentIndex();

    int row = index.row();

    if (row >= 0){
        if (QMessageBox::question(this, tr("Deletar alerta"),tr("Tem certeza que deseja deletar este alerta?"), tr("Remover"), tr("Cancelar")) == 0){
            alert->remove(tableAlert->model()->index(index.row(),0).data().toInt());

            alert->refreshModels();
        }
    }
}

void AlertInterface::onClick_lAdd()
{
    if(swPanels->currentIndex() == 0)
        onClick_pbLeave();

    gotoPanel(0);
}

void AlertInterface::onClick_pbRemoveList()
{
    if (QMessageBox::question(this, tr("Deletar tarefa"),tr("Tem certeza que deseja deletar esta tarefa?"), tr("Remover"), tr("Cancelar")) == 0){
        alert->removeTask(listTasks->model()->index(listTasks->currentIndex().row(), 0).data().toInt());

        alert->refreshModels();
    }

    swTasks->setCurrentIndex(0);
}

void AlertInterface::adjustSplitter()
{
    QList<int> list;

    list << wSpace->height() << tableAlert->height();

    splitter->setSizes(list);

    adjustLayout();
}

void AlertInterface::gotoPanel(int index)
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
    }

}

void AlertInterface::hidePanel(bool b)
{
    if(b)
    {
        animaUp = panelEffects->createAnimation(wSpace,QRect(wSpace->x(),
                                                             wSpace->y(), wSpace->width(), 0),800,QEasingCurve::InBack);

        animaDown = panelEffects->createAnimation(tableAlert,QRect(tableAlert->x(),
                                                                   0, tableAlert->width(), this->height() - lTop->height()),800,QEasingCurve::InBack);
    }
    else
    {
        animaDown  = panelEffects->createAnimation(wSpace,QRect(wSpace->x(),
                                                                wSpace->y(), wSpace->width(), 360),800,QEasingCurve::OutBack);

        animaUp  = panelEffects->createAnimation(tableAlert,QRect(tableAlert->x(),360,
                                                                  tableAlert->width(),this->height() - (lTop->height() + 360)),800,QEasingCurve::OutBack);
    }

    connect(animaDown,SIGNAL(valueChanged(QVariant)),SLOT(adjustSplitter()));

    connect(animaUp,SIGNAL(valueChanged(QVariant)),SLOT(adjustSplitter()));

    animaUp->start();

    animaDown->start();
}

void AlertInterface::nextPanel()
{
    disconnect(animaUp,SIGNAL(finished()),this,SLOT(nextPanel()));

    switch(swPanels->currentIndex())
    {
    case 0:lAdd->setAlternate(false);
        break;

    case 1:lProfile->setAlternate(false);
        break;
    }

    switch(nextPanelIndex)
    {
    case 0:lAdd->setAlternate(true);
        break;

    case 1:lProfile->setAlternate(true);
        break;
    }

    hidePanel(false);

    swPanels->setCurrentIndex(nextPanelIndex);
}

void AlertInterface::onClick_pbLeave()
{
    eTitle->Clear();
    eSubject->Clear();
    eDate->Clear();
    eHour->Clear();
    eDateAlert->Clear();
    eHourAlert->Clear();
    cbPriority->setCurrentIndex(0);
    teNotes->clear();

    alert->clearData();

    isAdd = true;

    pbConfirm->setText(tr("Confirmar"));
    pbLeave->setText(tr("Limpar"));
}

void AlertInterface::onClick_pbConfirm()
{
    if (isAvailable()){
        alert->title = eTitle->text();
        alert->subject = eSubject->text();
        alert->date = eDate->text();
        alert->hour = eHour->text();
        alert->dateAlert = eDateAlert->text();
        alert->hourAlert = eHourAlert->text();
        alert->priority = cbPriority->currentText();
        alert->note = teNotes->toPlainText();

        if (isAdd)
            alert->insert();
        else
            alert->update(idAlertUpdate);

        alert->refreshModels();

        onClick_pbLeave();
    }
}

void AlertInterface::onDoubleClick_tbAlert(QModelIndex index){
    int row = index.row();

    if (row >= 0){
        idAlertUpdate = tableAlert->model()->index(row, 0).data().toInt();
        eTitle->setText(tableAlert->model()->index(row, 1).data().toString());
        eSubject->setText(tableAlert->model()->index(row, 2).data().toString());
        eDate->setText(tableAlert->model()->index(row, 3).data().toString());
        eHour->setText(tableAlert->model()->index(row, 4).data().toString());
        eDateAlert->setText(tableAlert->model()->index(row, 5).data().toString());
        eHourAlert->setText(tableAlert->model()->index(row, 6).data().toString());
        cbPriority->setCurrentText(tableAlert->model()->index(row, 7).data().toString());
        teNotes->setText(tableAlert->model()->index(row, 9).data().toString());

        pbConfirm->setText(tr("Editar"));
        pbLeave->setText(tr("Cancelar"));


        createProfile(
                    tableAlert->model()->index(row, 1).data().toString(),
                    tableAlert->model()->index(row, 2).data().toString(),
                    tableAlert->model()->index(row, 3).data().toString(),
                    tableAlert->model()->index(row, 4).data().toString(),
                    tableAlert->model()->index(row, 5).data().toString(),
                    tableAlert->model()->index(row, 6).data().toString(),
                    tableAlert->model()->index(row, 9).data().toString()
                    );

        isAdd = false;
    }
}

void AlertInterface::onClick_tbAlert(QModelIndex index){
    int row = index.row();

    if (row < 0)
        return;

    if (tableAlert->model()->index(row, 8).data().toInt() == NOTREAD){
        pbAlterStatus->setEnabled(true);
        cbIsRead->setEnabled(true);

        pbAlterStatus->setText(tr("Marcar como lido"));
        cbIsRead->setChecked(false);
    }else if (tableAlert->model()->index(row, 8).data().toInt() == READ){
        pbAlterStatus->setEnabled(true);
        cbIsRead->setEnabled(true);

        pbAlterStatus->setText(tr("Marcar como não lido"));
        cbIsRead->setChecked(true);
    }else{
        pbAlterStatus->setEnabled(false);
        cbIsRead->setEnabled(false);

        pbAlterStatus->setText(tr("Marcar como lido"));
        cbIsRead->setChecked(false);
    }
}

void AlertInterface::rowChanged_tbAlert(int row){
    if (row >= 0){
        createProfile(
                    tableAlert->model()->index(row, 1).data().toString(),
                    tableAlert->model()->index(row, 2).data().toString(),
                    tableAlert->model()->index(row, 3).data().toString(),
                    tableAlert->model()->index(row, 4).data().toString(),
                    tableAlert->model()->index(row, 5).data().toString(),
                    tableAlert->model()->index(row, 6).data().toString(),
                    tableAlert->model()->index(row, 9).data().toString()
                    );

    }
}

bool AlertInterface::isAvailable()
{
    if (eTitle->isEmpty()){
        QMessageBox::warning(this, tr("Título inválido"), tr("Por favor, digite um título."), tr("Ok"));

        eTitle->setFocus();

        return false;
    }

    if (eSubject->isEmpty()){
        QMessageBox::warning(this, tr("Título inválido"), tr("Por favor, digite um título."), tr("Ok"));

        eSubject->setFocus();

        return false;
    }

    if (eDate->isEmpty() || (! tools.isValidDate(eDate->text()))){
        QMessageBox::warning(this, tr("Data inválida"), tr("Por favor, digite um data válida."), tr("Ok"));

        eDate->setFocus();

        return false;
    }

    if (eHour->isEmpty() || (! tools.isValidHour(eHour->text()))){
        QMessageBox::warning(this, tr("Hora inválida"), tr("Por favor, digite um hora válida."), tr("Ok"));

        eHour->setFocus();

        return false;
    }

    if (cbPriority->currentIndex() == 0){
        QMessageBox::warning(this, tr("Prioridade inválida"), tr("Por favor, selecione uma prioridade."), tr("Ok"));

        cbPriority->setFocus();

        return false;
    }

    if (eDateAlert->isEmpty() || (! tools.isValidDate(eDateAlert->text()))){
        QMessageBox::warning(this, tr("Data inválida"), tr("Por favor, digite um data válida."), tr("Ok"));

        eDateAlert->setFocus();

        return false;
    }

    if (eHourAlert->isEmpty() || (! tools.isValidHour(eHourAlert->text()))){
        QMessageBox::warning(this, tr("Hora inválida"), tr("Por favor, digite um hora válida."), tr("Ok"));

        eHourAlert->setFocus();

        return false;
    }

    if (tools.compareDate(eDate->value(), tools.getDate()) == QChar('1')){
        QMessageBox::warning(this, tr("Data do evento inválida"), tr("A data do evento digita já passou.\nOperação não pode ser realizada."), tr("Ok"));

        eDate->setFocus();

        return false;
    }else if (tools.compareDate(eDate->value(), tools.getDate()) == QChar('-1')){

        if (tools.compareHour(eHour->value(), tools.getTime("hh:mm")) == QChar('1')){
            QMessageBox::warning(this, tr("Data do evento inválida"), tr("A data do evento digita já passou.\nOperação não pode ser realizada."), tr("Ok"));

            eHour->setFocus();

            return false;
        }

    }

    if (tools.compareDate(eDate->value(), eDateAlert->value()) == QChar('1')){
        QMessageBox::warning(this, tr("Data de alerta inválida"), tr("A data de alerta digita é após a data do evento.\nOperação não pode ser realizada."), tr("Ok"));

        eDateAlert->setFocus();

        return false;
    }else if (tools.compareDate(eDate->value(), eDateAlert->value()) == QChar('-1')){
        // datas iguais, ai ele verifica a hora

        if (tools.compareHour(eHour->value(), eHourAlert->value()) == QChar('1')){
            QMessageBox::warning(this, tr("Data de alerta inválida"), tr("A data de alerta digita é após a data do evento.\nOperação não pode ser realizada."), tr("Ok"));

            eHourAlert->setFocus();

            return false;
        }else if(tools.compareHour(eHour->value(), eHourAlert->value()) == QChar('-1')){
            QMessageBox::warning(this, tr("Data de alerta inválida"), tr("A data de alerta digita é exatamente a data do evento.\nOperação não pode ser realizada."), tr("Ok"));

            eHourAlert->setFocus();

            return false;
        }
    }


    return true;
}

void AlertInterface::count(int c)
{
    QFont f;

#if defined(Q_WS_X11)
    f = QFont("Sans Serif",9);
#endif

#if defined(Q_WS_WIN)
    f = QFont("MS Shell Dlq 2", 11);
#endif

    f.setBold(true);

    textCountAlert->setText(QString::number(c));
    textCountAlert->setFont(f);
    textCountAlert->setAlignment(Qt::AlignHCenter);
}

void AlertInterface::seletorDate(int i)
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
        alert->getAlertsDays();

        wSeletorDate->pbWeek->setToolTip("");
        wSeletorDate->pbMonth->setToolTip("");
        wSeletorDate->pbYear->setToolTip("");
        wSeletorDate->pbPeriod->setToolTip("");

        wSeletorDate->pbToday->setToolTip(QString("Busca para hoje:\n%1").arg(alert->dateIFilter));

        break;
    }

    case 1:{
        alert->getAlertsWeek();

        wSeletorDate->pbToday->setToolTip("");
        wSeletorDate->pbMonth->setToolTip("");
        wSeletorDate->pbYear->setToolTip("");
        wSeletorDate->pbPeriod->setToolTip("");

        wSeletorDate->pbWeek->setToolTip(QString("Busca entre as datas:\n%1 e \n%2").arg(alert->dateIFilter, alert->dateFFilter));

        break;
    }

    case 2:{
        alert->getAlertsMonth();

        wSeletorDate->pbToday->setToolTip("");
        wSeletorDate->pbWeek->setToolTip("");
        wSeletorDate->pbYear->setToolTip("");
        wSeletorDate->pbPeriod->setToolTip("");

        wSeletorDate->pbMonth->setToolTip(QString("Busca entre as datas:\n%1 e \n%2").arg(alert->dateIFilter, alert->dateFFilter));

        break;
    }

    case 3:{
        alert->getAlertsYear();

        wSeletorDate->pbToday->setToolTip("");
        wSeletorDate->pbWeek->setToolTip("");
        wSeletorDate->pbMonth->setToolTip("");
        wSeletorDate->pbPeriod->setToolTip("");

        wSeletorDate->pbYear->setToolTip(QString("Busca entre as datas:\n%1 e \n%2").arg(alert->dateIFilter, alert->dateFFilter));

        break;
    }

    case 4:{

        break;
    }

    }
}

void AlertInterface::createProfile(QString title, QString subject, QString date, QString hour, QString dateAlert, QString hourAlert, QString body)
{
    QString dateCreation;
    QString dateForAlert;

    if(date == dateAlert)
        dateForAlert = tr("Serei avisado às ") + hourAlert + tr(" do mesmo dia");
    else
    {
        dateForAlert = tr("Serei avisado às ") + hourAlert + " ";
        dateForAlert += tr("do dia ") + dateAlert.mid(0,2) + tr(" de ") + tools.month(dateAlert.mid(3,2).toInt());
        dateForAlert += tr(" de ") + dateAlert.mid(6,4);
    }

    dateCreation = tr("Alerta criado às ") + hour + " " + tr("do dia ");
    dateCreation += date.mid(0,2) + tr(" de ") + tools.month(date.mid(3,2).toInt());
    dateCreation += tr(" de ") + dateAlert.mid(6,4);

    QString html = "<html></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">"
            "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:20pt;\">" + title + "</span></p>"
            "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:20pt;\"></p>"
            "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt;\">" + subject + "</span></p>"
            "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:16pt;\"></p>"
            "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; color:#7d7d7d;\">" + dateCreation + "</span></p>";

    if(dateAlert != "" && hourAlert != "")
    {
        html += "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; color:#565656;\">" + dateForAlert + "</span></p>";
    }

    html += "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:12pt; color:#565656;\"></p>"
            "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; color:#91352e;\">" + tr("Aviso") + "</span></p>"
            "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p>"
            "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">" + body + "</span><span style=\" font-size:14pt;\"><br /></span></p></body></html>";

    tbProfile->setHtml(html);
}

void AlertInterface::createProfile()
{
    QString html;

    QString text = tr("Selecione um aviso");

    html += "<html><head></head><body><br><br>"
            "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px;"
            " margin-left:0px; margin-right:0px; -qt-block-indent:0;"
            "text-indent:0px;\"><span style=\" font-size:20pt;\">" + text + "</span></p>"
            "</body></html>";

    tbProfile->setHtml(html);
}

void AlertInterface::updateHours()
{
    lHours->setText(tools.currentTime());

    if(lHours->text() == "00:00")
        lDate->setText(tools.currentDay() + " de " + tools.currentMonth() + " de " + tools.currentYear());
}

void AlertInterface::onTriggered_actionTitle()
{
    actionTitle->setChecked(true);
    actionTitle->setEnabled(false);

    actionSubject->setChecked(false);
    actionSubject->setEnabled(true);

    actionDate->setChecked(false);
    actionDate->setEnabled(true);

    actionHour->setChecked(false);
    actionHour->setEnabled(true);

    typeS = TITLE;
}

void AlertInterface::onTriggered_actionSubject()
{
    actionTitle->setChecked(false);
    actionTitle->setEnabled(true);

    actionSubject->setChecked(true);
    actionSubject->setEnabled(false);

    actionDate->setChecked(false);
    actionDate->setEnabled(true);

    actionHour->setChecked(false);
    actionHour->setEnabled(true);

    typeS = SUBJECT;
}

void AlertInterface::onTriggered_actionDate()
{
    actionTitle->setChecked(false);
    actionTitle->setEnabled(true);

    actionSubject->setChecked(false);
    actionSubject->setEnabled(true);

    actionDate->setChecked(true);
    actionDate->setEnabled(false);

    actionHour->setChecked(false);
    actionHour->setEnabled(true);

    typeS = DATE;
}

void AlertInterface::onTriggered_actionHour()

{
    actionTitle->setChecked(false);
    actionTitle->setEnabled(true);

    actionSubject->setChecked(false);
    actionSubject->setEnabled(true);

    actionDate->setChecked(false);
    actionDate->setEnabled(true);

    actionHour->setChecked(true);
    actionHour->setEnabled(false);

    typeS = HOUR;
}


void AlertInterface::updateCountOrdered(QString qnt)
{
    QFont f;

#if defined(Q_WS_X11)
    f = QFont("Sans Serif",9);
#endif

#if defined(Q_WS_WIN)
    f = QFont("MS Shell Dlq 2", 11);
#endif

    f.setBold(true);


    textCountAlert->setText(qnt);
    textCountAlert->setFont(f);
    textCountAlert->setAlignment(Qt::AlignHCenter);

    adjustLayout();
}

int AlertInterface::textCountAlertSize()
{
    short int pointSize;

#if defined(Q_WS_X11)
    pointSize = 9;
#endif

#if defined(Q_WS_WIN)
    pointSize = 11;
#endif

    int value;

    if(textCountAlert->toPlainText().length() <= 1)
        value = 30;
    else
        value = 12 + textCountAlert->toPlainText().length() * pointSize;

    return value;
}

void AlertInterface::updateVerificationAlert()
{
    alert->verifyAlert();
}

void AlertInterface::getSearchAlert(QString text)
{
    if (searchAlert->eSearch->isEmpty()){
        alert->setFilter();

        return;
    }

    QString filter = "";

    switch(typeS){

    case TITLE:{
        filter = QString("lower(title) like lower('" + text + "%')");

        break;
    }

    case SUBJECT:{
        filter = QString("lower(subject) like lower('" + text + "%')");

        break;
    }

    case DATE:{
        filter = QString("lower(dateevent) like lower('" + text + "%')");

        break;
    }

    case HOUR:{
        filter = QString("lower(hourevent) like lower('" + text + "%')");

        break;
    }

    }

    alert->setFilter(filter);

}

void AlertInterface::onClick_pbAlterStatus()
{
    int row = tableAlert->currentIndex().row();

    if (tableAlert->model()->index(row, 8).data().toInt() == NOTREAD){

        alert->alterStatus(tableAlert->model()->index(row, 0).data().toString(), QString::number(READ));

        emit subAlert();

    }else if (tableAlert->model()->index(row, 8).data().toInt() == READ){

        alert->alterStatus(tableAlert->model()->index(row, 0).data().toString(), QString::number(NOTREAD));

        emit plusAlert();
    }

    pbAlterStatus->setEnabled(false);

    tableAlert->clearSelection();
}

void AlertInterface::onClick_cbIsRead()
{
    int row = tableAlert->currentIndex().row();

    if (tableAlert->model()->index(row, 8).data().toInt() == NOTREAD){

        alert->alterStatus(tableAlert->model()->index(row, 0).data().toString(), QString::number(READ));

        emit subAlert();

    }else if (tableAlert->model()->index(row, 8).data().toInt() == READ){

        alert->alterStatus(tableAlert->model()->index(row, 0).data().toString(), QString::number(NOTREAD));

        emit plusAlert();
    }

    pbAlterStatus->setEnabled(false);

    tableAlert->clearSelection();
}

void AlertInterface::showBetweenDate(int option)
{
    if (option == 4)
    {
        if(wBetweenDate->isVisible())
            wBetweenDate->hide();
        else
            wBetweenDate->show();

        wBetweenDate->setGeometry((wSeletorDate->x() + wSeletorDate->pbPeriod->width() * 4 + wSeletorDate->pbPeriod->width()/2) - wBetweenDate->W_/2,
                                  wSeletorDate->y() + wSeletorDate->height() + 3,wBetweenDate->W_,wBetweenDate->H_);

    }else
        wBetweenDate->hide();

}

void AlertInterface::selectedDates(QString dateS, QString dateF)
{
    // não tratarei as datas, o tratamento estará na própria função que chama as datas

    alert->getAlertsCustom(dateS, dateF);

    wSeletorDate->pbToday->setToolTip("");
    wSeletorDate->pbWeek->setToolTip("");
    wSeletorDate->pbMonth->setToolTip("");
    wSeletorDate->pbYear->setToolTip("");

    wSeletorDate->pbPeriod->setToolTip(QString("Busca entre as datas:\n%1 e \n%2").arg(dateS, dateF));

    wBetweenDate->hide();
}

void AlertInterface::logout()
{
    onClick_pbLeave();
}
