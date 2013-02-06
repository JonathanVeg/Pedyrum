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


 #include "pizzamixed.h"
#include <QDebug>
#include <QMessageBox>

const char BACKGROUND[50] = ":/pizzaMixed/background";

PizzaMixed::PizzaMixed(QWidget *parent) :
    QWidget(parent)
{
    createComponent();
    statics();
    connections();

//    input();
}

void PizzaMixed::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_F2){
        onClick_pbConfirm();
    }

    //dei uma distancia para evitar apertos acidentais
    if (event->key() == Qt::Key_F5){
        onClick_pbLeave();
    }
}

void PizzaMixed::createComponent()
{
    pizza = new Pizza();

    lBackground = new QLabel(this);

    swPanel = new QStackedWidget(this);

    wGraphicView = new QWidget(this);

    graphics = new GraphicsReport(wGraphicView);

    search = new Search(this,tr("Pesquisar"));

    //    SBPart = new SpinBox(this);
    SBPart = new SpinBox(this);

    pbAddItem = new QPushButton(this);

    pbRemoveItem = new QPushButton(this);

    tablePizza = new TableView(this);

    cbSize = new ComboBox(this);

    DSBValue = new DoubleSpinBox(this);

    SBAmount = new SpinBox(this);

    pbConfirm = new QPushButton(this);

    pbLeave = new QPushButton(this);

    wNotes = new QWidget(this);

    lTextNote = new QLabel(wNotes);

    teNotePizza = new QTextEdit(wNotes);

    lNote = new Label(this,true,100);

    QList<QString> headers;
    headers << tr("ID") << tr("Sabor") << tr("Partes");

    model = new DataModel(this, headers);

    actionName = new QAction(tr("Pesquisar por nome"),this);
    actionNickName = new QAction(tr("Pesquisar por Apelido"),this);
}

void PizzaMixed::statics()
{
    lBackground->setGeometry(0,0,this->width(),this->height());

    search->setGeometry(10, 10, 400, 28);

    SBPart->setGeometry(415, 10, 105, 28);

    swPanel->setGeometry(10, search->y() + 38,330,280);

    wGraphicView->setGeometry(0,0,swPanel->width(), swPanel->height());

    wNotes->setGeometry(0,0,swPanel->width(), swPanel->height());

    pbAddItem->setGeometry(SBPart->x() + SBPart->width() + 5, SBPart->y(),65,28);

    tablePizza->setGeometry(350,pbAddItem->y() + 40,240,170);

    pbRemoveItem->setGeometry(420,tablePizza->y() + 180,100,25);

    cbSize->setGeometry(350, pbRemoveItem->y() + pbRemoveItem->height() + 10,240,28);

    DSBValue->setGeometry(350, cbSize->y() + cbSize->height() + 10,240,28);

    SBAmount->setGeometry(350, DSBValue->y() + DSBValue->height() + 10,240,28);

    pbLeave->setGeometry(50, swPanel->y() + swPanel->height() + 10 ,100,28);

    pbConfirm->setGeometry(pbLeave->x() + pbLeave->width() + 50, pbLeave->y() ,100,28);

    graphics->setGeometry(0, 0, wGraphicView->width(), wGraphicView->height());

    lTextNote->setGeometry(0,5,340,20);

    lNote->setGeometry(310,50,30,30);

    teNotePizza->setGeometry(10, 40,wNotes->width() - 20,wNotes->height() - 50);



}

void PizzaMixed::input()
{
    this->setWindowTitle(tr("Montagem de pizza mista"));
    wGraphicView->setStyleSheet("background-color: rgba(255,255,255,25);");
    this->setMaximumSize(600,380);
    this->setMinimumSize(600,380);

    lBackground->setPixmap(QPixmap(BACKGROUND));
    lBackground->setScaledContents(true);

    swPanel->addWidget(wGraphicView);
    swPanel->addWidget(wNotes);

    pbAddItem->setText(tr("Adicionar"));
    pbAddItem->setFont(this->font());
    pbAddItem->setToolTip(tr("Adicionar o sabor a pizza mista"));

    pbRemoveItem->setText(tr("Remover pizza"));
    pbRemoveItem->setFont(this->font());
    pbRemoveItem->setToolTip(tr("Remover o sabor selecionado da pizza mista"));

    tablePizza->setObjectName("tablePizza");
    tablePizza->setStyleSheet(this->styleSheet());
    tablePizza->setFont(this->font());

    cbSize->setFont(this->font());
    cbSize->setToolTip(tr("Selecione o tamanho da pizza"));

    DSBValue->setFont(this->font());
    DSBValue->setPrefix(tr("Preço sugerido R$ "));
    DSBValue->setToolTip(tr("Sugerimos o valor da pizza mais cara"));

    SBAmount->setPrefix(tr("Quantidade: "));
    SBAmount->setFont(this->font());
    SBAmount->setValue(1);
    SBAmount->setMaximum(9999999);

    pbConfirm->setText(tr("Concluído"));
    pbConfirm->setFont(this->font());

    pbLeave->setText(tr("Limpar"));
    pbLeave->setFont(this->font());

    tablePizza->setStyleSheet("background-color: rgba(255,255,255,25);"
                              "color: rgba(240,240,240,240);");
    tablePizza->setFont(this->font());
    tablePizza->setModel(model);
    tablePizza->setColumnHidden(0, true);

    SBPart->setPrefix(tr("Partes: "));
    SBPart->setMinimum(1);
    SBPart->setMaximum(6);
    SBPart->setToolTip(tr("Selecione a quantidade de partes.\nMáximo de seis partes (até seis sabores diferentes)"));

    graphics->setBackgroundColor(0, 0, 0, 0);
    graphics->useLegend(false);
    graphics->setType(GraphicsReport::Sectors);
    graphics->setInvalidDataText("Selecione uma pizza");

    lTextNote->setText(tr("Anotações"));
    lTextNote->setAlignment(Qt::AlignCenter);
    lTextNote->setFont(this->font());
    lTextNote->setStyleSheet("color: rgba(254, 255, 180, 255);");

    lNote->setImage(":/treatment/icoNote");
    lNote->setHover(":/treatment/icoNote-hover");
    lNote->setAlternateImage(":/treatment/icoNote-alternate");

    QList<QColor> colors;
    colors.append(QColor(240, 0, 0, 255));
    colors.append(QColor(0, 220, 0, 255));
    colors.append(QColor(31, 61, 250, 255));
    colors.append(QColor(0 ,199, 199, 255));
    colors.append(QColor(220, 0, 99, 255));
    colors.append(QColor(240, 130, 41, 255));

    graphics->setColors(colors);

    search->setCompleter(pizza->bd.getCompleter("product"));

    search->eSearch->setNextComponent(SBPart);

    teNotePizza->setStyleSheet("background-color: qlineargradient"
                               "(spread:pad, x1:0, y1:1, x2:0.165045,"
                               "y2:0, stop:0 rgba(254, 255, 180, 255),"
                               "stop:0.721591 rgba(255, 250, 205, 255));"
                               "border-radius: 10px;border: 1px solid #C0C0C0;");

    tablePizza->setColumnWidth(1, (tablePizza->width() * 0.6) - 1);
    tablePizza->setColumnWidth(2, (tablePizza->width() * 0.4) - 1);

    actionName->setCheckable(true);
    actionName->setChecked(true);
    actionName->setEnabled(false);

    actionNickName->setCheckable(true);
    actionNickName->setChecked(false);
    actionNickName->setEnabled(true);

    search->setAction(actionName);
    search->setAction(actionNickName);

}

void PizzaMixed::connections()
{
    connect(search, SIGNAL(textSearch(QString)), SLOT(getSearchProduct(QString)));

    connect(pbAddItem, SIGNAL(clicked()), SLOT(onClick_pbAddItem()));

    connect(SBPart, SIGNAL(valueChanged(int)), SLOT(valueChanged_SBPart(int)));

    connect(cbSize, SIGNAL(currentIndexChanged(QString)), SLOT(suggestValues(QString)));

    connect(pbRemoveItem, SIGNAL(clicked()), SLOT(onClick_pbRemoveItem()));

    connect(pbLeave, SIGNAL(clicked()), SLOT(onClick_pbLeave()));

    connect(pbConfirm, SIGNAL(clicked()), SLOT(onClick_pbConfirm()));

    connect(SBPart, SIGNAL(returnPressed()), SLOT(returnPressed_sbPart()));

    connect(lNote,SIGNAL(onClick()),this,SLOT(onClick_lNote()));

    connect(actionName,SIGNAL(triggered()),SLOT(onTriggered_actionName()));

    connect(actionNickName,SIGNAL(triggered()),SLOT(onTriggered_actionNickName()));
}

void PizzaMixed::onClick_pbRemoveItem(){
    if (tablePizza->currentIndex().row() >= 0){
        int op = QMessageBox::question(this, tr("Remover item"),tr("Deseja remover o produto da pizza mista?"), tr("Sim"), tr("Não"));

        int i = tablePizza->currentIndex().row();

        if (op == 0){
            SBPart->setMaximum(SBPart->maximum() + listOrderPartProduct[i]);

            listOrderPartProduct.removeAt(i);
            listOrderIdProduct.removeAt(i);
            listOrderNameProduct.removeAt(i);
            listOrderSizesProduct.removeAt(i);
            listOrderValuesProduct.removeAt(i);

            model->removeRow(i);

            graphics->setData(listOrderPartProduct);
        }
    }
}

void PizzaMixed::onClick_pbLeave(){
    if (pbLeave->text() == tr("Cancelar")){
        pbLeave->setText("Limpar");

        pbConfirm->setText("Concluído");

        clearAll();

        this->close();

        return;
    }

    int op = QMessageBox::question(this, tr("Remover todos os itens"),tr("Tem certeza que deseja todos os itens da pizza mista?"), tr("Sim"), tr("Não"));

    if (op == 0){
        clearAll();
    }
}

void PizzaMixed::clearAll(){
    model->clear();

    listOrderPartProduct.clear();
    listOrderIdProduct.clear();
    listOrderNameProduct.clear();
    listOrderValuesProduct.clear();
    listOrderSizesProduct.clear();

    listSizes.clear();

    graphics->clearData();

    SBPart->setMaximum(6);

    cbSize->clear();

    SBAmount->clear();

    DSBValue->clear();

    search->eSearch->setFocus();
}

void PizzaMixed::onClick_pbConfirm(){
    if (listOrderPartProduct.length() < 2){

        QMessageBox::warning(this, "Pizza mista inválida", "Pizza mista precisa ter pelo menos dois sabores", QMessageBox::Ok);

        search->eSearch->setFocus();

        return;
    }

    if (pbConfirm->text() == tr("Concluído")){

        emit emitPizzaMista(listOrderPartProduct, listOrderIdProduct, listOrderNameProduct, teNotePizza->toPlainText(), SBAmount->value(), DSBValue->value(), cbSize->currentText(), true, -1); // i de insert

    }else{

        emit emitPizzaMista(listOrderPartProduct, listOrderIdProduct, listOrderNameProduct, teNotePizza->toPlainText(), SBAmount->value(), DSBValue->value(), cbSize->currentText(), false, indexUpdate); // u de update

        pbConfirm->setText("Concluído");

        pbLeave->setText("Limpar");
    }

    clearAll();

    close();
}

void PizzaMixed::onClick_lNote()
{
    if(lNote->isAlternate)
    {
        lNote->setAlternate(false);
        swPanel->setCurrentIndex(0);
    }
    else
    {
        lNote->setAlternate(true);
        swPanel->setCurrentIndex(1);
    }
}

void PizzaMixed::valueChanged_SBPart(int value){
    //   graphics->setData(graphics->getDataSimple() << float(value));
}

void PizzaMixed::onClick_pbAddItem(){
    // verifica se existe algum produto selecionado
    if (dataOrderIdProduct.isEmpty()){
        QMessageBox::warning(this,tr("Erro"),
                             tr("Por favor, selecione um sabor válido."),
                             QMessageBox::Ok);

        search->eSearch->setFocus();

        return;
    }

    // para evitar 2 sabores diferentes, se aquele id já existir, ele vai add mais partes a ele
    if (listOrderIdProduct.indexOf(dataOrderIdProduct) >= 0){
        int i = listOrderIdProduct.indexOf(dataOrderIdProduct);

        SBPart->setMaximum(SBPart->maximum() + listOrderPartProduct[i]);
        SBPart->setValue(SBPart->value() + listOrderPartProduct[i]);

        listOrderPartProduct.removeAt(i);
        listOrderIdProduct.removeAt(i);
        listOrderNameProduct.removeAt(i);
        listOrderSizesProduct.removeAt(i);
        listOrderValuesProduct.removeAt(i);

        /// aqui ele tem que mudar no model se o sabor ja existir e add as novas partes
        model->removeRow(i);
    }

    // caso o produto não esteja na lista

    dataOrderPartProduct = SBPart->value();

    listOrderIdProduct << dataOrderIdProduct;
    listOrderNameProduct << dataOrderNameProduct;
    listOrderValuesProduct << dataOrderValuesProduct;
    listOrderSizesProduct << dataOrderSizesProduct;
    listOrderPartProduct << dataOrderPartProduct;

    QList<QString> values;

    values << dataOrderIdProduct << dataOrderNameProduct << QString::number(dataOrderPartProduct);

    model->add(values);

    if (listSizes.isEmpty())
        listSizes = dataOrderSizesProduct;
    else
        updateCbSizes();

    cbSize->clear();
    cbSize->addItems(listSizes);

    int totalPartsMissing = 6; // partes que fatam
    foreach(int p, listOrderPartProduct)
        totalPartsMissing -= p;

    if(totalPartsMissing == 0)
        cbSize->setFocus();
    else{
        dataOrderPartProduct = 1;
        dataOrderIdProduct.clear();
        dataOrderNameProduct.clear();
        dataOrderSizesProduct.clear();
        dataOrderValuesProduct.clear();

        search->eSearch->Clear();

        SBPart->setValue(1);

        search->eSearch->setFocus();
    }

    SBPart->setMaximum(totalPartsMissing);

    suggestValues(cbSize->currentText());

    graphics->setData(listOrderPartProduct);
}

void PizzaMixed::getSearchProduct(QString text){
    if (text.length() > 0){
        QString sql = "select * from product where lower(name) like lower('" + text + "%') order by idproduct";

        pizza->bd.updateModelQuery("product", sql);

        sql = "select idproduct from product where lower(name) = lower('" + text + "')";

        if (pizza->bd.search(sql)){

            //SBPart->setFocus();

            dataOrderIdProduct = pizza->bd.loadSimple(0);
            dataOrderNameProduct = text;

            sql = "select s.name, pv.valueproduct from size s, product p, productvalue pv where p.idproduct = pv.idproduct and pv.idsize = s.idsize and p.idproduct = " + pizza->bd.loadSimple(0);

            if (pizza->bd.search(sql, 1)){
                dataOrderSizesProduct = pizza->bd.loadColumn(0, 1);
                dataOrderValuesProduct = pizza->bd.loadColumn(1, 1);
            }

            dataOrderPartProduct = SBPart->value();
        }else{
            dataOrderPartProduct = 1;
            dataOrderIdProduct.clear();
            dataOrderNameProduct.clear();
            dataOrderSizesProduct.clear();
            dataOrderValuesProduct.clear();
        }

    }else{
        QString sql = "select * from product order by idproduct";

        pizza->bd.updateModelQuery("product", sql);
    }
}

void PizzaMixed::suggestValues(QString size){
    double maxValue = 0;

    if (! size.isEmpty()){
        for (int i = 0; i < listOrderSizesProduct.length(); i++){
            if (listOrderSizesProduct[i].indexOf(size) >= 0)
                if (listOrderValuesProduct[i][listOrderSizesProduct[i].indexOf(size)].toDouble() > maxValue)
                    maxValue = listOrderValuesProduct[i][listOrderSizesProduct[i].indexOf(size)].toDouble();
        }
    }

    DSBValue->setValue(maxValue);
}

void PizzaMixed::updateCbSizes()
{
    // dataOrderSizesProduct listSizes
    foreach(QString size, dataOrderSizesProduct)
        if (listSizes.indexOf(size) < 0)
            listSizes << size;
}

void PizzaMixed::editPizzaMixed(QList<int> listOrderPartProduct, QList<QString> listOrderIdProduct, QList<QString> listOrderNameProduct, QString note, int quantity, double value, QString size, int row){

    Q_UNUSED(listOrderIdProduct);

    indexUpdate = row;
    for (int i = 0; i < listOrderNameProduct.length(); i++){
        search->eSearch->setText(listOrderNameProduct[i]);

        SBPart->setValue(listOrderPartProduct[i]);

        onClick_pbAddItem();
    }

    SBAmount->setValue(quantity);

    int index = note.indexOf("\n");

    if (index == -1)
        note.clear();
    else
        note = note.mid(index + 1);

    teNotePizza->setText(note);

    for (int i = 0; i < cbSize->count(); i++){
        cbSize->setCurrentIndex(i);

        if (cbSize->currentText() == size)
            break;
    }

    DSBValue->setValue(value);

    pbConfirm->setText(tr("Editar item"));

    pbLeave->setText(tr("Cancelar"));
}

void PizzaMixed::returnPressed_sbPart(){
    onClick_pbAddItem();

    search->eSearch->setFocus();
}

void PizzaMixed::onTriggered_actionName()
{
    actionName->setChecked(true);
    actionName->setEnabled(false);

    actionNickName->setChecked(false);
    actionNickName->setEnabled(true);
}

void PizzaMixed::onTriggered_actionNickName()
{
    actionName->setChecked(false);
    actionName->setEnabled(true);

    actionNickName->setChecked(true);
    actionNickName->setEnabled(false);
}
