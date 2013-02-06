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


 #include "Interface/productinterface.h"
#include <QDebug>
#include <QMessageBox>

ProductInterface::ProductInterface(QWidget *parent) :
    QWidget(parent)
{
    createComponent();
    statics();
    connections();    
}

void ProductInterface::createComponent()
{
    lTop = new QLabel(this);

    lAdd = new Label(this,true,100);
    lRemove = new Label(this,true,100);
    lProfile = new Label(this,true,100);    
    textCountProduct = new QTextBrowser(this);
    cbCategory = new ComboBox(this);
    cbSection = new ComboBox(this);
    searchProduct = new Search(this,tr("Pesquisar Produtos"));

    wSpace = new QWidget(this);
    lBackgroundSpace = new QLabel(wSpace);

    tableProduct = new TableView(this);

    splitter = new QSplitter(Qt::Vertical,this);

    swPanels = new QStackedWidget(wSpace);

    wGroupPlus = new QWidget(wSpace);
    lBackgroundPlus = new QLabel(wGroupPlus);
    eName  = new Edit(wGroupPlus,true,tr("Nome"), 60);
    eNickname = new Edit(wGroupPlus,true,tr("Apelido"), 60);

    eSize = new Edit(wGroupPlus,true,tr("Tamanho"), 20);
    DSBValue = new DoubleSpinBox(wGroupPlus);
    eCategory = new Edit(wGroupPlus,true,tr("Categoria"), 30);
    eSection = new Edit(wGroupPlus,true,tr("Seção"), 30);
    tableValues = new TableView(wGroupPlus);

    lTextNotes = new QLabel(wGroupPlus);
    teNotes = new QTextEdit(wGroupPlus);

    pbConfirm = new QPushButton(wGroupPlus);
    pbLeave = new QPushButton(wGroupPlus);

    // cria os botões no final para não alterar o tabOrder do jeito q eu quero
    pbAddValue = new QPushButton(wGroupPlus);
    pbRemoveValue = new QPushButton(wGroupPlus);

    // --------------------- Profile --------------------

    wGroupProfile = new QWidget(this);
    lBackgroundProfile = new QLabel(wGroupProfile);
    textPreview = new QTextBrowser(wGroupProfile);
    lEmpty = new QLabel(wGroupProfile);

    product = new Product();

    QList<QString> headers;

    headers << tr("Tamanho") << tr("Valor");

    dataModel = new DataModel(this, headers);

    actionName = new QAction(tr("Pesquisar por nome"),this);
    actionNickName = new QAction(tr("Pesquisar por Apelido"),this);
}

void ProductInterface::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Delete){
        onClick_lRemove();
    }

    switch (event->modifiers()){

    case Qt::ControlModifier:{
        if (event->key() == Qt::Key_S)
            onClick_pbConfirm();

        if (event->key() == Qt::Key_L)
            onClick_pbLeave();

        if (event->key() == Qt::Key_P)
            onClick_lProfile();

        if (event->key() == Qt::Key_Tab)
            emit goToPanel(7);

        break;
    }
    }

    if (event->key() == Qt::Key_Backtab)
        emit goToPanel(5);

    if (event->key() == Qt::Key_F11)
        emit fullScreen();
}

void ProductInterface::input()
{
    typeS = NAME;

    nextPanelIndex = 0;

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

    textCountProduct->setText(product->countProducts());
    textCountProduct->setObjectName("textCountProduct");
    textCountProduct->setToolTip(tr("Número de produtos"));
    textCountProduct->setFont(f);
    textCountProduct->setAlignment(Qt::AlignCenter);
    textCountProduct->setStyleSheet(this->styleSheet());

    f.setBold(false);

    cbCategory->setToolTip(tr("Categoria"));
    cbCategory->setFont(this->font());
    cbCategory->addItem(tr("Categorias"));
    updateCbCategory();

    cbSection->setToolTip(tr("Seção"));
    cbSection->setFont(this->font());
    cbSection->hide();

    lBackgroundSpace->setStyleSheet("background-color: rgba(123, 31, 31, 255);");

    swPanels->addWidget(wGroupPlus);
    swPanels->addWidget(wGroupProfile);

    lBackgroundPlus->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.498,"
                                   "y1:0, x2:0.494636, y2:1, stop:0 rgba(255, 255, 255, 210),"
                                   "stop:0.982955 rgba(255, 255, 255, 170));"
                                   "border-radius: 5px;border: 1px solid rgba(255,255,255,255);");

    teNotes->setStyleSheet("background-color: qlineargradient"
                           "(spread:pad, x1:0, y1:1, x2:0.165045,"
                           "y2:0, stop:0 rgba(254, 255, 180, 255),"
                           "stop:0.721591 rgba(255, 250, 205, 255));"
                           "border-radius: 10px;border: 1px solid #C0C0C0;");

    lTextNotes->setFont(this->font());
    lTextNotes->setAlignment(Qt::AlignHCenter);
    lTextNotes->setText(tr("Anotações"));
    lTextNotes->setStyleSheet("color: rgba(50, 50, 50, 255);");

    teNotes->setToolTip(tr("Anotações"));

    DSBValue->setPrefix(tr("R$ "));
    DSBValue->setMaximum(99999999);

    pbAddValue->setText(tr("Adicionar a lista"));
    pbAddValue->setToolTip(tr("Adicionar valor e tamanho"));

    pbRemoveValue->setText(tr("Remover"));
    pbRemoveValue->setToolTip(tr("Remover valor e tamanho"));

    pbConfirm->setText(tr("Adicionar"));
    pbLeave->setText(tr("Limpar"));

    splitter->insertWidget(1,wSpace);
    splitter->insertWidget(2,tableProduct);
    splitter->show();
    splitter->setHandleWidth(1);
    splitter->setStretchFactor(1,2);

    lBackgroundProfile->setStyleSheet(lBackgroundPlus->styleSheet());

    textPreview->setStyleSheet("background-color: rgba(0,0,0,0);"
                               "border: 1px solid rgba(0,0,0,0);");

    textPreview->setHtml(createPreview());

    wSpace->setGeometry(0,0,0,0);

    eName->setNextComponent(eNickname);
    eNickname->setNextComponent(eSize);
    eSize->setNextComponent(DSBValue);
    eCategory->setNextComponent(eSection);
    eSection->setNextComponent(teNotes);


    // funções ligadas a classe product
    isAdd = true;

    tableProduct->setModel(product->bd.getModelQuery("product"));
    tableProduct->hideColumn(0);

    try{
        eCategory->setCompleter(product->bd.getCompleter("category"));
    }catch(QString e){
        qDebug() << "[ProductInterface] [input]" << e << "category";
    }

    try{
        eSection->setCompleter(product->bd.getCompleter("subcategory"));
    }catch(QString e){
        qDebug() << "[ProductInterface] [input]" << e << "subcategory";
    }

    try{
        eSize->setCompleter(product->bd.getCompleter("size"));
    }catch(QString e){
        qDebug() << "[ProductInterface] [input]" << e << "size";
    }

    f.setPointSize(14);
    lEmpty->setFont(f);
    lEmpty->setText(tr("Selecione um produto"));
    lEmpty->setAlignment(Qt::AlignHCenter);

    isSelectEmpty();

    tableValues->setModel(dataModel);

    tableValues->setColumnWidth(0,tableValues->width() * 0.6 -1);
    tableValues->setColumnWidth(1,tableValues->width() * 0.4 -1);

    searchProduct->setCompleter(product->bd.getCompleter("product"));

    actionName->setCheckable(true);
    actionName->setChecked(true);
    actionName->setEnabled(false);

    actionNickName->setCheckable(true);
    actionNickName->setChecked(false);
    actionNickName->setEnabled(true);

    searchProduct->setAction(actionName);
    searchProduct->setAction(actionNickName);


}

void ProductInterface::connections()
{
    connect(splitter, SIGNAL(splitterMoved(int, int)), SLOT(adjustLayout()));

    connect(lAdd, SIGNAL(onClick()), SLOT(onClick_lAdd()));

    connect(lRemove, SIGNAL(onClick()), SLOT(onClick_lRemove()));

    connect(lProfile, SIGNAL(onClick()), SLOT(onClick_lProfile()));

    connect(pbConfirm, SIGNAL(clicked()), SLOT(onClick_pbConfirm()));

    connect(pbLeave, SIGNAL(clicked()), SLOT(onClick_pbLeave()));

    connect(swPanels, SIGNAL(currentChanged(int)), SLOT(adjustLayout()));

    connect(pbAddValue, SIGNAL(clicked()), SLOT(onClick_addValue()));

    connect(pbRemoveValue, SIGNAL(clicked()), SLOT(onClick_removeValue()));

    connect(eCategory, SIGNAL(textChanged(QString)), SLOT(textChanged_eCategory(QString)));

    connect(eSize, SIGNAL(textChanged(QString)), SLOT(textChanged_eSize(QString)));

    connect(tableProduct, SIGNAL(doubleClicked(QModelIndex)), SLOT(onDoubleClick_tbProduct(QModelIndex)));

    connect(tableProduct, SIGNAL(clicked(QModelIndex)), SLOT(onClick_tbProduct(QModelIndex)));

    connect(tableProduct, SIGNAL(rowChanged(int)), SLOT(rowChanged_tbProduct(int)));

    connect(tableValues, SIGNAL(clicked(QModelIndex)), SLOT(onClick_tbValue(QModelIndex)));

    connect(product, SIGNAL(count(QString)), SLOT(updateCountProducts(QString)));

    connect(cbCategory, SIGNAL(currentIndexChanged(QString)), SLOT(updateCbSubCategory(QString)));

    connect(cbSection, SIGNAL(currentIndexChanged(QString)), SLOT(filterSubCategory(QString)));

    connect(searchProduct, SIGNAL(textSearch(QString)), SLOT(getSearchProduct(QString)));

    connect(DSBValue, SIGNAL(returnPressed()), SLOT(onClick_addValue()));

    connect(actionName,SIGNAL(triggered()),SLOT(onTriggered_actionName()));

    connect(actionNickName,SIGNAL(triggered()),SLOT(onTriggered_actionNickName()));
}

void ProductInterface::hidePanel(bool b)
{
    if(b)
    {
        animaUp = panelEffects->createAnimation(wSpace,QRect(wSpace->x(),
                                                             wSpace->y(), wSpace->width(), 0),800,QEasingCurve::InBack);

        animaDown = panelEffects->createAnimation(tableProduct,QRect(tableProduct->x(),
                                                                     0, tableProduct->width(), this->height() - lTop->height()),800,QEasingCurve::InBack);
    }
    else
    {
        animaDown  = panelEffects->createAnimation(wSpace,QRect(wSpace->x(),
                                                                wSpace->y(), wSpace->width(), 360),800,QEasingCurve::OutBack);

        animaUp  = panelEffects->createAnimation(tableProduct,QRect(tableProduct->x(),360,
                                                                    tableProduct->width(),this->height() - (lTop->height() + 360)),800,QEasingCurve::OutBack);
    }

    connect(animaDown,SIGNAL(valueChanged(QVariant)),SLOT(adjustSplitter()));

    connect(animaUp,SIGNAL(valueChanged(QVariant)),SLOT(adjustSplitter()));

    animaUp->start();

    animaDown->start();
}

void ProductInterface::statics()
{
    lAdd->setGeometry(12,11,30,28);
    lRemove->setGeometry(lAdd->width() + lAdd->x() + 12,11,30,28);
    lProfile->setGeometry(lRemove->width() + lRemove->x() + 12,11,30,28);

    // ----------------------- Plus e Edit -----------------------

    wGroupPlus->setMinimumSize(700,300);
    wGroupPlus->setMaximumSize(700,300);

    wGroupProfile->setMinimumSize(280,330);
    wGroupProfile->setMaximumSize(280,330);

    eName->setGeometry(20,30,200,27);
    eNickname->setGeometry(20,eName->y() + eName->height() + 8,200,27);

    eSize->setGeometry(20,eNickname->y() + eNickname->height() + 8,100,27);
    DSBValue->setGeometry(eSize->x() + eSize->width() + 6,eSize->y(),93,27);

    pbAddValue->setGeometry(20,eSize->y() + eSize->height() + 8,100,27);
    pbRemoveValue->setGeometry(pbAddValue->x() + pbAddValue->width() + 6,pbAddValue->y(),93,27);

    eCategory->setGeometry(20,pbAddValue->y() + pbAddValue->height() + 8,200,27);
    eSection->setGeometry(20,eCategory->y() + eCategory->height() + 8,200,27);

    pbConfirm->setGeometry(20,eSection->y() + eSection->height() + 10,97,30);

    pbLeave->setGeometry(pbConfirm->width() + pbConfirm->x() + 6,
                         pbConfirm->y(),97,30);

    tableValues->setGeometry(eSection->x() + eSection->width() + 5, 30, 210,242);

    teNotes->setGeometry(tableValues->x() + tableValues->width() + 5,30,240,243);
    lTextNotes->setGeometry(teNotes->x(),10,teNotes->width(),18);

    textPreview->setGeometry(0,0,wGroupProfile->width(),wGroupProfile->height());

    lEmpty->setGeometry(0,wGroupProfile->height()/2 - 12,wGroupProfile->width(),24);


}

QString ProductInterface::createPreview()
{
    QString name;
    QString nickname;
    QString category;
    QString section;

    name = product->getName();
    nickname = product->getNickname();
    category = product->getNameCategory();
    section = product->getNameSubCategory();

    if(name != "")
        isSelect = true;
    else
        isSelect = false;

    QString html = "<html><body>"
            "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px;"
            "margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:12pt; color:#4b4b4b;\"></p>"
            "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
            "text-indent:0px;\"><span style=\" font-size:15pt; color:#4b4b4b;\">" + name + "</span><span style=\" font-size:8pt;\"> </span></p>"
            "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px;"
            "margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:12pt; color:#4b4b4b;\"></p>"
            "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
            "text-indent:0px;\"><span style=\" font-size:9pt; font-weight:600; color:#4b4b4b;\">" + tr("Apelido") + "</span>"
            "<span style=\" font-size:10pt; color:#4b4b4b;\">:</span><span style=\" font-size:10pt; color:#000000;\">" + nickname + "</span></p>"
            "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
            "-qt-block-indent:0; text-indent:0px; font-size:10pt; color:#4b4b4b;\"></p>"
            "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
            "text-indent:0px;\"><span style=\" font-size:9pt; font-weight:600; color:#4b4b4b;\">" +  tr("Categoria") + "</span>"
            "<span style=\" font-size:10pt; color:#4b4b4b;\">: </span><span style=\" font-size:10pt; color:#000000;\">" + category + "</span></p>"
            "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
            "-qt-block-indent:0; text-indent:0px; font-size:10pt; color:#4b4b4b;\"></p>"
            "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
            "-qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:9pt; font-weight:600; color:#4b4b4b;\">" + tr("Seção") + "</span>"
            "<span style=\" font-size:10pt; color:#4b4b4b;\">: </span><span style=\" font-size:10pt; color:#000000;\">" + section + "</span></p>"
            "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
            "-qt-block-indent:0; text-indent:0px; font-size:10pt; color:#4b4b4b;\"></p>"
            "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
            "text-indent:0px;\"><span style=\" font-size:11pt; font-weight:600; font-style:italic; color:#000000;\">---------------- </span>"
            "<span style=\" font-size:10pt; font-weight:600; font-style:italic; color:#000000;\">" + tr("Tamanhos") + "</span>"
            "<span style=\" font-size:11pt; font-weight:600; font-style:italic; color:#000000;\"> ----------------</span></p>"
            "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px;"
            "margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt; color:#4b4b4b;\"></p>";

    QList<QString> sizes = product->getSizes();
    QList<QString> values = product->getValues();

    for (int i = 0; i < sizes.length(); i++)
        html += "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                "-qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-style:italic; color:#030303;\">" + sizes[i] + "</span>"
                "<span style=\" font-size:10pt; color:#4b4b4b;\">: </span><span style=\" font-size:10pt; color:#09324b;\">" + values[i] + tr(" Reais") +  "</span></p>";

    html += "</body></html>";

    return html;

}

void ProductInterface::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

bool ProductInterface::isSelectEmpty()
{
    bool b = isSelect;

    textPreview->setVisible(b);

    lEmpty->setVisible(!b);

    return b;
}

void ProductInterface::onClick_lAdd()
{
    if(swPanels->currentIndex() == 0)
        onClick_pbLeave();

    gotoPanel(0);
}

void ProductInterface::onClick_lRemove()
{
    QModelIndex index = tableProduct->currentIndex();

    int row = index.row();

    if (row >= 0){

        QString name = tableProduct->model()->index(index.row(),1).data().toString();

        QString message = "Tem certeza que deseja remover o produto: " + name;

        if (QMessageBox::question(this, tr("Deletar produto"),message, tr("Remover"), tr("Cancelar")) == 0){

            product->remove(tableProduct->model()->index(index.row(),0).data().toInt());

            onClick_pbLeave();
        }
    }else{
        QMessageBox::warning(this, tr("Selecione um produto"), tr("Selecione um produto para excluir."), tr("Ok"));
    }
}

void ProductInterface::onClick_lProfile()
{
    gotoPanel(1);
}


void ProductInterface::onClick_pbConfirm()
{
    if (isAvailable()){
        product->setName(eName->text());
        product->setNickname(eNickname->text());
        product->setSizes(listSizes);
        product->setValues(listValues);

        if (!eCategory->isEmpty()){
            product->setNameCategory(eCategory->text());
            if (!eSection->isEmpty())
                product->setNameSubCategory(eSection->text());
            else
                product->setNameSubCategory("");
        }

        product->setNote(teNotes->toPlainText());
        if (isAdd){
            product->insert();
        }else{
            product->update(idProductUpdate);
        }

        updateCbCategory();

        this->onClick_pbLeave();
    }
}

void ProductInterface::onClick_pbLeave()
{
    eNickname->Clear();
    eSize->Clear();
    DSBValue->setValue(0);
    eCategory->Clear();
    eSection->Clear();
    teNotes->clear();
    eName->Clear(); // limpa o name por ultimo para ele por direto o foco nele

    listSizes.clear();
    listValues.clear();

    addDataValue();

    pbConfirm->setText("Adicionar");
    pbLeave->setText("Limpar");
    isAdd = true;

//    gotoPanel(0);
}

void ProductInterface::onClick_addValue(){
    // primeiro verifica se tem algo digitado na caixa de valor
    if (eSize->isEmpty()){
        eSize->setFocus();

        return;
    }

    // primeiro verifica se esse tamanho não existe na lista (se existir, ele edita o valor)
    int index = -1;
    for (int i = 0; i < listSizes.length(); i ++){
        if (listSizes[i].toLower() == eSize->text().toLower())
            index = i;
    }

    if (index >= 0)

        listValues[index] = QString::number(DSBValue->value());

    else{

        listSizes << eSize->text();

        listValues << QString::number(DSBValue->value());
    }

    addDataValue();

    eSize->Clear();

    DSBValue->setValue(0);

    eSize->setFocus();
}

void ProductInterface::onClick_removeValue(){
    /// fazer depois, quando a tabela existir

    if (tableValues->currentIndex().row() >= 0){

        int i = tableValues->currentIndex().row();

        listSizes.removeAt(i);

        listValues.removeAt(i);

        addDataValue();
    }
}

void ProductInterface::textChanged_eCategory(QString text){
    QString sql = "select * from subcategory where idcategory = ";
    sql += "(select idcategory from category where lower(name) = lower('" + text + "'))";
    // o lower é para ser case insensitive

    product->bd.updateModelQuery("subcategory", sql);
}

void ProductInterface::textChanged_eSize(QString text){
    int index = -1;
    for (int i = 0; i < listSizes.length(); i ++){
        if (listSizes[i].toLower() == text.toLower())
            index = i;
    }

    if (index >= 0){
        pbAddValue->setText(tr("Editar valor"));
        DSBValue->setValue(listValues[index].toDouble());
    }else{
        DSBValue->setValue(0);
        pbAddValue->setText(tr("Adicionar a lista"));
    }
}

void ProductInterface::onDoubleClick_tbProduct(QModelIndex index){

    product->setIdProduct(tableProduct->model()->index(index.row(), 0).data().toInt());

    idProductUpdate = tableProduct->model()->index(index.row(),0).data().toInt();

    eName->setText(tableProduct->model()->index(index.row(), 1).data().toString());
    eNickname->setText(tableProduct->model()->index(index.row(), 2).data().toString());

    eCategory->setText(tableProduct->model()->index(index.row(), 3).data().toString());
    eSection->setText(tableProduct->model()->index(index.row(), 4).data().toString());

    teNotes->setText(tableProduct->model()->index(index.row(), 5).data().toString());

    product->getPrices();
    product->loadData(product->getIdProduct());

    listSizes = product->getSizes();
    listValues = product->getValues();

    addDataValue();

    isAdd = false;

    pbConfirm->setText(tr("Editar"));
    pbLeave->setText(tr("Cancelar"));

    textPreview->setHtml(createPreview());

    lEmpty->setVisible(false);

    textPreview->setVisible(true);

    bool b = (wSpace->height() > 0)? true:false;

    if (! b)
        gotoPanel(0);
}

void ProductInterface::onClick_tbProduct(QModelIndex index){
}

void ProductInterface::rowChanged_tbProduct(int row){
    product->setIdProduct(tableProduct->model()->index(row, 0).data().toInt());

    product->getPrices();
    product->loadData(product->getIdProduct());

    textPreview->setHtml(createPreview());
}

void ProductInterface::onClick_tbValue(QModelIndex index){
    eSize->setText(tableValues->model()->index(index.row(), 0).data().toString());

    DSBValue->setValue(tableValues->model()->index(index.row(), 1).data().toDouble());
}

void ProductInterface::adjustSplitter()
{
    QList<int> list;

    list << wSpace->height() << tableProduct->height();

    splitter->setSizes(list);

    adjustLayout();
}

void ProductInterface::adjustLayout()
{
    lTop->setGeometry(0,0,this->width(),50);

    searchProduct->setGeometry(this->width() - 220,11,200,28);

    splitter->setGeometry(0,lTop->height(),this->width(),this->height() - lTop->height());

    textCountProduct->setGeometry(lProfile->width() + lProfile->x() + 12,11,textCountProductSize(),28);

    cbCategory->setGeometry(textCountProduct->width() + textCountProduct->x() + 12,11,110,28);

    cbSection->setGeometry(cbCategory->width() + cbCategory->x() + 12,11,100,28);

    wSpace->setGeometry(0,0,this->width(),wSpace->height());

    lBackgroundSpace->setGeometry(0,0,this->width(),wSpace->height());

    swPanels->setGeometry(0,0,wSpace->width(),wSpace->height());

    wGroupPlus->setGeometry(this->width()/2 - 350,wSpace->height()/2 - 150,700,300);

    wGroupProfile->setGeometry(this->width()/2 - 140,wSpace->height()/2 - 165,280,330);

    lBackgroundPlus->setGeometry(0,0,wGroupPlus->width(),wGroupPlus->height());
    lBackgroundProfile->setGeometry(0,0,wGroupProfile->width(),wGroupProfile->height());

    tableProduct->setGeometry(0,wSpace->y() + wSpace->height(),this->width(),
                             this->height() - (wSpace->height() + lTop->height()));

    tableProduct->setColumnWidth(1,tableProduct->width() * 0.20);
    tableProduct->setColumnWidth(2,tableProduct->width() * 0.15);
    tableProduct->setColumnWidth(3,tableProduct->width() * 0.15);
    tableProduct->setColumnWidth(4,tableProduct->width() * 0.15);
    tableProduct->setColumnWidth(5,tableProduct->width() * 0.35);
}

void ProductInterface::gotoPanel(int index)
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

void ProductInterface::nextPanel()
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

bool ProductInterface::isAvailable()
{
    if (eName->isEmpty()){
        QMessageBox::warning(this, tr("Nome inválido"), tr("Por favor, digite um nome para o produto."), tr("Ok"));

        eName->setFocus();

        return false;
    }

    if (eNickname->isEmpty()){
        QMessageBox::warning(this, tr("Apelido inválido"), tr("Por favor, digite um apelido para o produto."), tr("Ok"));

        eNickname->setFocus();

        return false;
    }

    if (listSizes.isEmpty()){
        QMessageBox::warning(this, tr("O produto precisa ter pelo menos um valor cadastrado"), tr("Por favor, escolha pelo menos um valor para o produto."), tr("Ok"));

        eSize->setFocus();

        return false;
    }

    if (eCategory->isEmpty()){
        QMessageBox::warning(this, tr("Categoria inválida"), tr("Por favor, digite um categoria para o produto.\nCaso a categoria ainda não exista, ela sera criada juntamente com o produto."), tr("Ok"));

        eCategory->setFocus();

        return false;
    }

    return true;

    // não verifiquei seção pq não sei se ela deve ser obrigatória
}

void ProductInterface::updateCountProducts(QString qnt)
{
    QFont f;

#if defined(Q_WS_X11)
    f = QFont("Sans Serif",9);
#endif

#if defined(Q_WS_WIN)
    f = QFont("MS Shell Dlq 2", 11);
#endif

    f.setBold(true);

    textCountProduct->setText(qnt);
    textCountProduct->setFont(f);
    textCountProduct->setAlignment(Qt::AlignHCenter);

    adjustLayout();
}

void ProductInterface::updateCbCategory()
{
    cbCategory->clear();

    cbCategory->addItem(tr("Categorias"));

    foreach(QString nameCategory, product->getAllCategory())
        cbCategory->addItem(nameCategory);
}

void ProductInterface::updateCbSubCategory(QString nameCategory)
{
    if (cbCategory->currentIndex() > 0){
        // primeiro ele ja gera o filtro
        product->bd.search("select idcategory from category where name = '" + nameCategory + "'");

        QString sql = "select p.idproduct, p.name as \"Nome\", p.nickname as \"apelido\", c.name as \"Categoria\", sc.name as \"Seção\", p.note as \"Anotação\" from product p, category c, subcategory sc where c.idcategory = p.idcategory and sc.idsubcategory = p.idsubcategory and sc.idcategory = c.idcategory ";
        sql += "and p.idCategory = " + product->bd.loadSimple(0);
        sql += " order by p.idproduct";

        product->bd.updateModelQuery("product", sql);

        updateCountProducts(product->countProducts(product->bd.loadSimple(0)));

        // agora ele da update nas subcategorias

        QList<QString> sections = product->getAllSubCategory(nameCategory);

        if (sections.length() > 0){
            cbSection->clear();
            cbSection->addItem("Seção");
            foreach(QString nameSection, sections)
                cbSection->addItem(nameSection);

            cbSection->setVisible(true);
        }else
            cbSection->setVisible(false);
    }else{
        QString sql = "select p.idproduct, p.name as \"Nome\", p.nickname as \"apelido\", c.name as \"Categoria\", sc.name as \"Seção\", p.note as \"Anotação\" from product p, category c, subcategory sc where c.idcategory = p.idcategory and sc.idsubcategory = p.idsubcategory and sc.idcategory = c.idcategory ";
        sql += " order by p.idproduct";

        product->bd.updateModelQuery("product", sql);

        updateCountProducts(product->countProducts());

        cbSection->setVisible(false);
    }
}

void ProductInterface::filterSubCategory(QString nameSection){
    product->bd.search("select idcategory from category where name = '" + cbCategory->currentText() + "'");
    product->bd.search("select idsubcategory from subcategory where idcategory = '" + product->bd.loadSimple(0) + "' and name = '" + nameSection + "'", 1);
    if (cbSection->currentIndex() > 0){

        QString sql = "select p.idproduct, p.name as \"Nome\", p.nickname as \"apelido\", c.name as \"Categoria\", sc.name as \"Seção\", p.note as \"Anotação\" from product p, category c, subcategory sc where c.idcategory = p.idcategory and sc.idsubcategory = p.idsubcategory and sc.idcategory = c.idcategory ";
        sql += "and p.idCategory = " + product->bd.loadSimple(0) + " and p.idsubcategory = " + product->bd.loadSimple(0, 1);
        sql += " order by p.idproduct";

        product->bd.updateModelQuery("product", sql);

        updateCountProducts(product->countProducts(product->bd.loadSimple(0), product->bd.loadSimple(0, 1)));
    }else{
        QString sql = "select p.idproduct, p.name as \"Nome\", p.nickname as \"apelido\", c.name as \"Categoria\", sc.name as \"Seção\", p.note as \"Anotação\" from product p, category c, subcategory sc where c.idcategory = p.idcategory and sc.idsubcategory = p.idsubcategory and sc.idcategory = c.idcategory ";
        sql += "and p.idCategory = " + product->bd.loadSimple(0);
        sql += " order by p.idproduct";

        updateCountProducts(product->countProducts(product->bd.loadSimple(0)));

        product->bd.updateModelQuery("product", sql);
    }

}

void ProductInterface::getSearchProduct(QString text)
{
    if (text.length() > 0){
        // primeiro seta os filtros
        cbCategory->setCurrentIndex(0);

        QString sql = "select p.idproduct, p.name as \"Nome\", p.nickname as \"apelido\", c.name as \"Categoria\", sc.name as \"Seção\", p.note as \"Anotação\" from product p, category c, subcategory sc where c.idcategory = p.idcategory and sc.idsubcategory = p.idsubcategory and sc.idcategory = c.idcategory ";

        if (typeS == NAME)
            sql += "and lower(p.name) like lower('" + text + "%')";
        else
            sql += "and lower(p.nickname) like lower('" + text + "%')";

        sql += " order by p.idproduct";

        product->bd.updateModelQuery("product", sql);
    }else{
        QString sql = "select p.idproduct, p.name as \"Nome\", p.nickname as \"apelido\", c.name as \"Categoria\", sc.name as \"Seção\", p.note as \"Anotação\" from product p, category c, subcategory sc where c.idcategory = p.idcategory and sc.idsubcategory = p.idsubcategory and sc.idcategory = c.idcategory ";
        sql += " order by p.idproduct";

        product->bd.updateModelQuery("product", sql);
    }
}

void ProductInterface::addDataValue()
{
    QList<QString> data;

    dataModel->clear();
    for (int i = 0; i < listSizes.length(); i++){
        data << listSizes[i] << listValues[i];

        dataModel->add(data);

        data.clear();
    }

    if (listSizes.length() > 0){
        eSize->setText(listSizes[0]);

        DSBValue->setValue(listValues[0].toDouble());
    }
}

void ProductInterface::onTriggered_actionName()
{
    actionName->setChecked(true);
    actionName->setEnabled(false);

    actionNickName->setChecked(false);
    actionNickName->setEnabled(true);

    typeS = NAME;
}

void ProductInterface::onTriggered_actionNickName()
{
    actionName->setChecked(false);
    actionName->setEnabled(true);

    actionNickName->setChecked(true);
    actionNickName->setEnabled(false);

    typeS = NICKNAME;
}
int ProductInterface::textCountProductSize()
{
    short int pointSize;

#if defined(Q_WS_X11)
    pointSize = 9;
#endif

#if defined(Q_WS_WIN)
    pointSize = 11;
#endif

    int value;

    if(textCountProduct->toPlainText().length() <= 1)
        value = 30;
    else
        value = 12 + textCountProduct->toPlainText().length() * pointSize;

    return value;
}

void ProductInterface::logout()
{
    onClick_pbLeave();
}
