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


 #include "Interface/counterinterface.h"
#include <QMessageBox>

CounterInterface::CounterInterface(QWidget *parent) :
    QWidget(parent)
{
    createComponent();
    statics();
    connections();

    totalValueOrder = 0;

    isAdd = true;
}

void CounterInterface::createComponent()
{
    wProduct = new QWidget(this);
    lProduct = new QLabel(wProduct);

    searchProduct = new Search(wProduct,tr("Pesquisar Produtos"));
    productPreview = new QTextBrowser(wProduct);
    cbSize = new ComboBox(wProduct);
    DSBValueProduct = new DoubleSpinBox(wProduct);
    sbAmount = new SpinBox(wProduct);
    lPizza = new Label(wProduct,true,100);

    pbConfirmProduct = new QPushButton(wProduct);
    pbLeaveProduct = new QPushButton(wProduct);

    wProductNotes = new QWidget(this);
    lProductNotes = new QLabel(wProductNotes);
    lTextNotesProduct = new QLabel(wProductNotes);
    teProductNotes = new QTextEdit(wProductNotes);

    wPaymentView = new PaymentView(this);

    wItens = new QWidget(this);
    lItens = new QLabel(wItens);

    pbSaveOrdered = new QPushButton(wItens);
    pbRemoveItem = new QPushButton(wItens);
    pbClearOrdered = new QPushButton(wItens);
    pbCancelOrdered = new QPushButton(wItens);

    tableItem = new TableView(wItens);

    order = new Order("connectionCounter");

    QList<QString> headers;

    headers << tr("Nome") << tr("Tamanho") << tr("Quantidade") << tr("Valor") << tr("Anotação");
    dataModelItems = new DataModel(this, headers);

    wPizzaMixed = new PizzaMixed();

    actionName = new QAction(tr("Pesquisar por nome"),this);
    actionNickName = new QAction(tr("Pesquisar por Apelido"),this);
}

void CounterInterface::input()
{
    lItens->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.498,"
                          "y1:0, x2:0.494636, y2:1, stop:0 rgba(255, 255, 255, 210),"
                          "stop:0.982955 rgba(255, 255, 255, 170));"
                          "border-radius: 5px;border: 1px solid rgba(255,255,255,255);");

    pbSaveOrdered->setObjectName("pbSaveOrdered");
    pbSaveOrdered->setStyleSheet(this->styleSheet());
    pbSaveOrdered->setText(tr("Salvar"));
    pbSaveOrdered->setFont(this->font());
    pbSaveOrdered->setCursor(Qt::PointingHandCursor);

    pbRemoveItem->setObjectName("pbRemoveItem");
    pbRemoveItem->setStyleSheet(this->styleSheet());
    pbRemoveItem->setText(tr("Remover"));
    pbRemoveItem->setFont(this->font());
    pbRemoveItem->setCursor(Qt::PointingHandCursor);

    pbClearOrdered->setObjectName("pbClearOrdered");
    pbClearOrdered->setStyleSheet(this->styleSheet());
    pbClearOrdered->setText(tr("Limpar"));
    pbClearOrdered->setFont(this->font());
    pbClearOrdered->setCursor(Qt::PointingHandCursor);

    pbCancelOrdered->setObjectName("pbCancelOrdered");
    pbCancelOrdered->setStyleSheet(this->styleSheet());
    pbCancelOrdered->setText(tr("Cancelar"));
    pbCancelOrdered->setFont(this->font());
    pbCancelOrdered->setCursor(Qt::PointingHandCursor);

    pbConfirmProduct->setObjectName("pbConfirmProduct");
    pbConfirmProduct->setStyleSheet(this->styleSheet());
    pbConfirmProduct->setFont(this->font());
    pbConfirmProduct->setText(tr("Confirmar"));
    pbConfirmProduct->setCursor(Qt::PointingHandCursor);

    pbLeaveProduct->setObjectName("pbLeaveProduct");
    pbLeaveProduct->setStyleSheet(this->styleSheet());
    pbLeaveProduct->setFont(this->font());
    pbLeaveProduct->setText(tr("Limpar"));
    pbLeaveProduct->setCursor(Qt::PointingHandCursor);

    productPreview->setStyleSheet("background-color: rgba(0,0,0,0);"
                                  "color: rgba(255,255,255,240);");

    lPizza->setImage(":/treatment/icopizza");
    lPizza->setHover(":/treatment/icopizza-hover");

    sbAmount->setPrefix(tr("Quant "));
    sbAmount->setFont(this->font());
    sbAmount->setMinimum(1);
    sbAmount->setValue(1);
    sbAmount->setMaximum(99999);

    DSBValueProduct->setPrefix(tr("R$ "));
    DSBValueProduct->setMaximum(9999999);

    lProductNotes->setPixmap(QPixmap(":/treatment/field-notes-Background"));
    lTextNotesProduct->setText(tr("Anotações"));
    lTextNotesProduct->setFont(this->font());
    lTextNotesProduct->setAlignment(Qt::AlignHCenter);
    lTextNotesProduct->setStyleSheet("color: rgba(254, 255, 180, 255);");

    teProductNotes->setStyleSheet("background-color: qlineargradient"
                                  "(spread:pad, x1:0, y1:1, x2:0.165045,"
                                  "y2:0, stop:0 rgba(254, 255, 180, 255),"
                                  "stop:0.721591 rgba(255, 250, 205, 255));"
                                  "border-radius: 5px;border: 1px solid #C0C0C0;");

    teProductNotes->setFont(this->font());

    lProduct->setPixmap(QPixmap(":/treatment/fieldBackground"));
    lProduct->setScaledContents(true);

    QFont f;

#if defined(Q_WS_X11)
    f.setPointSize(9);
#endif

#if defined(Q_WS_WIN)
    f.setPointSize(12);
#endif

    productPreview->setFont(f);
    setPreview();

    tableItem->setModel(dataModelItems);

    wPizzaMixed->setFont(this->font());
    wPizzaMixed->setStyleSheet(this->styleSheet());

    searchProduct->setCompleter(order->bd.getCompleter("product"));

    cbSize->add("Tamanhos");

    actionName->setCheckable(true);
    actionName->setChecked(true);
    actionName->setEnabled(false);

    actionNickName->setCheckable(true);
    actionNickName->setChecked(false);
    actionNickName->setEnabled(true);

    searchProduct->setAction(actionName);
    searchProduct->setAction(actionNickName);

    searchProduct->eSearch->setNextComponent(cbSize);
    cbSize->setNextComponent(DSBValueProduct);
    DSBValueProduct->setNextComponent(sbAmount);

    emit setInput();
}

void CounterInterface::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Delete)
        onClick_pbRemoveItem();

    switch (event->modifiers()){

    case Qt::ControlModifier:{
        if (event->key() == Qt::Key_S)
            onClick_pbSaveOrdered();

        if (event->key() == Qt::Key_L)
            onClick_pbClearOrdered();

        if (event->key() == Qt::Key_C)
            onClick_pbCancelOrdered();

        if (event->key() == Qt::Key_M)
            onClick_lPizza();

        break;
    }
    }

    if (event->key() == Qt::Key_F11)
        emit fullScreen();
}

void CounterInterface::adjustLayout()
{
    wPaymentView->setGeometry(this->width() - 280,0,280,this->height());

    wProduct->setGeometry((this->width() - 280)/2 - 355, (this->height() * 0.55)/2 - 155,350,310);
    wProductNotes->setGeometry(wProduct->x() + wProduct->width() + 10, wProduct->y(), 350,310);

    wItens->setGeometry(0,this->height() * 0.55,this->width() - wPaymentView->width(),this->height() * 0.45);
    lItens->setGeometry(5,0,wItens->width() - 10,wItens->height() - 5);

    pbSaveOrdered->setGeometry(wItens->width()/2 - 195,5,90,28);
    pbRemoveItem->setGeometry(pbSaveOrdered->x() + pbSaveOrdered->width() + 10,5,90,28);
    pbClearOrdered->setGeometry(pbRemoveItem->x() + pbRemoveItem->width() + 10,5,90,28);
    pbCancelOrdered->setGeometry(pbClearOrdered->x() + pbClearOrdered->width() + 10,5,90,28);

    tableItem->setGeometry(lItens->x() + 5,38,lItens->width() - 10,wItens->height() - 50);

    tableItem->setColumnWidth(0,tableItem->width() * 0.18);
    tableItem->setColumnWidth(1,tableItem->width() * 0.14);
    tableItem->setColumnWidth(2,tableItem->width() * 0.14);
    tableItem->setColumnWidth(3,tableItem->width() * 0.1);
    tableItem->setColumnWidth(4,tableItem->width() * 0.44 - 1);
}

void CounterInterface::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void CounterInterface::statics()
{
    lProduct->setGeometry(0,0,350,310);

    searchProduct->setGeometry(25,15,300,28);

    productPreview->setGeometry(10,searchProduct->height() + 50,210,170);
    cbSize->setGeometry(productPreview->width() + 15,70,110,26);
    DSBValueProduct->setGeometry(cbSize->x(),cbSize->y() + 40, cbSize->width(), 26);
    sbAmount->setGeometry(DSBValueProduct->x(),DSBValueProduct->y() + 40, DSBValueProduct->width(), 26);
    lPizza->setGeometry(sbAmount->x() + 35,190,40,40);

    pbLeaveProduct->setGeometry(57,275,90,28);
    pbConfirmProduct->setGeometry(203,275,90,28);

    lProductNotes->setGeometry(0,0,350,310);
    lTextNotesProduct->setGeometry(0,10,350,25);
    teProductNotes->setGeometry(15,35,320,260);
}

void CounterInterface::connections()
{
    connect(DSBValueProduct, SIGNAL(valueChanged(double)), SLOT(valueChanged_DSBValueProduct(double)));

    connect(this, SIGNAL(setInput()), wPizzaMixed, SLOT(input()));

    connect(this, SIGNAL(setInput()), wPaymentView, SLOT(input()));

    connect(lPizza, SIGNAL(onClick()), SLOT(onClick_lPizza()));

    connect(wPaymentView, SIGNAL(save()), SLOT(onClick_pbSaveOrdered()));

    connect(wPaymentView, SIGNAL(clear()), SLOT(onClick_pbClearOrdered()));

    connect(wPaymentView, SIGNAL(cancel()), SLOT(onClick_pbCancelOrdered()));

    connect(wPaymentView, SIGNAL(pizza()), SLOT(onClick_lPizza()));

    connect(wPizzaMixed, SIGNAL(emitPizzaMista(QList<int>,QList<QString>,QList<QString>,QString,int,double,QString,bool,int)), this, SLOT(slotPizzaMista(QList<int>,QList<QString>,QList<QString>,QString,int,double,QString,bool,int)));

    connect(searchProduct, SIGNAL(textSearch(QString)), SLOT(getSearchProduct(QString)));

    connect(pbConfirmProduct, SIGNAL(clicked()), SLOT(onClick_pbConfirmProduct()));

    connect(pbLeaveProduct, SIGNAL(clicked()), SLOT(onClick_pbLeaveProduct()));

    connect(tableItem, SIGNAL(doubleClicked(QModelIndex)), SLOT(doubleClicked_tableItem(QModelIndex)));

    connect(this, SIGNAL(editPizzaMixed(QList<int>,QList<QString>,QList<QString>,QString,int,double,QString,int)), wPizzaMixed, SLOT(editPizzaMixed(QList<int>,QList<QString>,QList<QString>,QString,int,double,QString,int)));

    connect(sbAmount, SIGNAL(valueChanged(int)), SLOT(valueChanged_sbAmount(int)));

    connect(cbSize, SIGNAL(currentIndexChanged(int)), SLOT(currentIndexChanged_cbSize(int)));

    connect(pbRemoveItem, SIGNAL(clicked()), SLOT(onClick_pbRemoveItem()));

    connect(pbClearOrdered, SIGNAL(clicked()), SLOT(onClick_pbClearOrdered()));

    connect(pbCancelOrdered, SIGNAL(clicked()), SLOT(onClick_pbCancelOrdered()));

    connect(pbSaveOrdered, SIGNAL(clicked()), SLOT(onClick_pbSaveOrdered()));

    connect(actionName,SIGNAL(triggered()),SLOT(onTriggered_actionName()));

    connect(actionNickName,SIGNAL(triggered()),SLOT(onTriggered_actionNickName()));

    connect(wPaymentView->order, SIGNAL(loadComplete()), SLOT(loadDataOrder()));

    connect(sbAmount, SIGNAL(returnPressed()), SLOT(onClick_pbConfirmProduct()));
}

void CounterInterface::setPreview()
{
    QString html;

    html = "<html>"
            "<p align=\"center\"> Nome do produto</p>"
            "<p align=\"center\"> Valor em reais</p>"
            "<p align=\"center\"> Unidade(s)</p>"
            "<p align=\"center\"> Valor total </p>"
            "</html>";

    productPreview->setHtml(html);
}

void CounterInterface::setPreview(QString nome, float value, int quant)
{
    QString html;
    QString quantText, valueText, totalText;

    valueText.setNum(value);
    quantText.setNum(quant);
    totalText.setNum(value * quant);

    html = "<html>"
            "<p align=\"center\">" + nome + "</p>"
            "<p align=\"center\">" + valueText + tr(" Reais") + "</p>"
            "<p align=\"center\">" + quantText + tr(" Unidade(s)") + "</p>"
            "<p align=\"center\">" + totalText +  tr(" Reais") + "</p>"
            "</html>";

    productPreview->setHtml(html);
}

void CounterInterface::onClick_lPizza(){
    wPizzaMixed->show();
}

void CounterInterface::onClick_pbConfirmProduct(){
    if (pbConfirmProduct->text() == "Editar item"){
        dataModelItems->removeRow(indexSelectedItem);

        totalValueOrderProducts -= orderValues[indexSelectedItem] * orderQuantity[indexSelectedItem];

        orderQuantity.removeAt(indexSelectedItem);
        orderValues.removeAt(indexSelectedItem);
        orderSizes.removeAt(indexSelectedItem);
        orderPartProduct.removeAt(indexSelectedItem);
        orderIdProduct.removeAt(indexSelectedItem);
        orderNameProduct.removeAt(indexSelectedItem);
        orderNoteProduct.removeAt(indexSelectedItem);

        updateValues();
    }

    if (dataOrderIdProduct.isEmpty()){
        QMessageBox::warning(this, tr("Selecione um sabor"), tr("Por favor, selecione um produto válido."), QMessageBox::Ok);

        searchProduct->eSearch->Clear();

        searchProduct->eSearch->setFocus();

        return;
    }

    dataOrderQntProduct = sbAmount->value();
    dataOrderSizeProduct = cbSize->currentText();

    orderQuantity << dataOrderQntProduct;
    //    orderValues << dataOrderValueProduct;
    orderValues << DSBValueProduct->value();
    orderSizes << dataOrderSizeProduct;
    orderNoteProduct << teProductNotes->toPlainText();

    QList<QString> id, name;

    id << dataOrderIdProduct;
    name << dataOrderNameProduct;

    orderIdProduct << id;
    orderNameProduct << name;

    QList<int> part;

    part << 1;

    orderPartProduct << part;

    totalValueOrderProducts += DSBValueProduct->value() * dataOrderQntProduct;
    //    totalValueOrderProducts += dataOrderValueProduct * dataOrderQntProduct;

    QList<QString> dataForModel;

    //    dataForModel << dataOrderNameProduct << cbSize->currentText() << QString::number(dataOrderQntProduct) << QString::number(dataOrderValueProduct) << teProductNotes->toPlainText();
    dataForModel << dataOrderNameProduct << cbSize->currentText() << QString::number(dataOrderQntProduct) << QString::number(DSBValueProduct->value()) << teProductNotes->toPlainText();

    dataModelItems->add(dataForModel);

    updateValues();

    onClick_pbLeaveProduct();
}

void CounterInterface::onClick_pbLeaveProduct(){
    setPreview();
    listSizes.clear();
    listValues.clear();
    sbAmount->setValue(1);
    cbSize->setCurrentIndex(0);
    teProductNotes->clear();
    searchProduct->eSearch->Clear();
    DSBValueProduct->setValue(0);

    productName.clear();

    dataOrderIdProduct.clear();

    pbLeaveProduct->setText("Limpar");
    pbConfirmProduct->setText("Confirmar");
}

void CounterInterface::slotPizzaMista(QList<int> listOrderPartProduct, QList<QString> listOrderIdProduct, QList<QString> listOrderNameProduct, QString note, int quantity, double value, QString size, bool insert, int row){
    if (! insert){

        dataModelItems->removeRow(row);

        totalValueOrderProducts -= orderValues[row] * orderQuantity[row];

        orderQuantity.removeAt(row);
        orderValues.removeAt(row);
        orderSizes.removeAt(row);
        orderPartProduct.removeAt(row);
        orderIdProduct.removeAt(row);
        orderNameProduct.removeAt(row);
        orderNoteProduct.removeAt(row);

        updateValues();
    }

    orderQuantity << quantity;
    orderValues << value;
    orderPartProduct << listOrderPartProduct;
    orderIdProduct << listOrderIdProduct;
    orderNameProduct << listOrderNameProduct;
    orderSizes << size;

    QString noteN = "Sabores: ";

    int totalParts = 0;

    foreach(int part, listOrderPartProduct)
        totalParts += part;

    for (int i = 0; i < listOrderNameProduct.length(); i++){
        noteN += QString::number(listOrderPartProduct[i]) + "/" + QString::number(totalParts) + " de " + listOrderNameProduct[i];
        if (i == listOrderNameProduct.length() - 2)
            noteN += " e ";
        else
            if (i < listOrderNameProduct.length() - 1)
                noteN += ", ";
    }

    if (! note.isEmpty())
        noteN += "\n" + note;

    orderNoteProduct << noteN;

    QList<QString> dataForModel;

    dataForModel << "Pizza mista" << size << QString::number(quantity) << QString::number(value) << noteN;

    dataModelItems->add(dataForModel);

    totalValueOrderProducts += value * quantity;

    updateValues();
}

void CounterInterface::updateValues()
{
    wPaymentView->updateValuesTotal(totalValueOrderProducts);
}

void CounterInterface::getSearchProduct(QString text){
    if (text.length() > 0){
        QString sql = "select * from product where lower(name) like lower('" + text + "%') order by idproduct";

        order->bd.updateModelQuery("product", sql);

        sql = "select idproduct, note from product where lower(name) = lower('" + text + "')";

        if (order->bd.search(sql)){

            dataOrderIdProduct = order->bd.loadSimple(0);
            dataOrderNoteProduct = order->bd.loadSimple(1);
            dataOrderNameProduct = text;

            teProductNotes->setText(order->bd.loadSimple(1));

            sql = "select s.name, pv.valueproduct from size s, product p, productvalue pv where p.idproduct = pv.idproduct and pv.idsize = s.idsize and p.idproduct = " + order->bd.loadSimple(0);

            if (order->bd.search(sql, 1)){
                cbSize->clear();
                listSizes = order->bd.loadColumn(0, 1);
                listValues = order->bd.loadColumn(1, 1);
                cbSize->addItems(listSizes);

                float value = listValues[0].toDouble();

                dataOrderValueProduct = listValues[0].toDouble();

                productName = text;

                setPreview(productName, value, sbAmount->value());

                DSBValueProduct->setValue(value);
            }

        }else{
            listSizes.clear();
            listValues.clear();
            cbSize->clear();
            cbSize->addItem("Tamanho");
            DSBValueProduct->setValue(0.00);
            sbAmount->setValue(1);
            productName.clear();
            setPreview();
        }

    }else{
        QString sql = "select * from product order by idproduct";

        order->bd.updateModelQuery("product", sql);

        setPreview();
    }
}

void CounterInterface::doubleClicked_tableItem(QModelIndex index){
    int row = index.row();
    if (orderIdProduct[row].length() == 1){
        // produto normal
        searchProduct->eSearch->setText(orderNameProduct[row][0]);

        for (int j = 0; j < cbSize->count(); j++){
            cbSize->setCurrentIndex(j);
            if (orderSizes[row] == cbSize->currentText())
                break;
        }

        sbAmount->setValue(orderQuantity[row]);

        indexSelectedItem = row;

        teProductNotes->setText(orderNoteProduct[row]);

        pbConfirmProduct->setText("Editar item");
        pbLeaveProduct->setText("Cancelar");
    }else{
        emit editPizzaMixed(orderPartProduct[row], orderIdProduct[row], orderNameProduct[row], orderNoteProduct[row], orderQuantity[row], orderValues[row], orderSizes[row], row);

        wPizzaMixed->show();
    }
}

void CounterInterface::valueChanged_sbAmount(int value){
    Q_UNUSED(value);

    if (listSizes.length() > 0){
        //        float value = listValues[cbSize->currentIndex()].toDouble();

        float value = DSBValueProduct->value();

        dataOrderValueProduct = listValues[cbSize->currentIndex()].toDouble();

        dataOrderQntProduct = sbAmount->value();

        setPreview(productName, value, sbAmount->value());
    }
}

void CounterInterface::currentIndexChanged_cbSize(int index){
    Q_UNUSED(index);

    if (listSizes.length() > 0 && cbSize->count() > 0){
        float value = listValues[cbSize->currentIndex()].toDouble();

        DSBValueProduct->setValue(value);

        dataOrderValueProduct = value;

        setPreview(productName, value, sbAmount->value());
    }
}

void CounterInterface::onClick_pbRemoveItem(){
    int row = tableItem->currentIndex().row();

    if (row >= 0){

        int op = QMessageBox::question(this, tr("Remover item"),tr("Deseja remover o produto do pedido?"), tr("Sim"), tr("Não"));

        if (op == 0){
            dataModelItems->removeRow(row);

            totalValueOrderProducts -= orderValues[row] * orderQuantity[row];

            orderQuantity.removeAt(row);
            orderValues.removeAt(row);
            orderSizes.removeAt(row);
            orderPartProduct.removeAt(row);
            orderIdProduct.removeAt(row);
            orderNameProduct.removeAt(row);
            orderNoteProduct.removeAt(row);

            updateValues();
        }
    }
}

void CounterInterface::onClick_pbClearOrdered(){
    if (tableItem->model()->rowCount() == 0)
        return;

    int op = QMessageBox::question(this, tr("Deleter todos os itens."),tr("Tem certeza que deseja apagar todos os itens deste pedido?"), tr("Sim"), tr("Não"));

    if (op == 0){
        dataModelItems->clear();

        orderQuantity.clear();
        orderValues.clear();
        orderSizes.clear();
        orderPartProduct.clear();
        orderIdProduct.clear();
        orderNameProduct.clear();
        orderNoteProduct.clear();

        totalValueOrderProducts = 0;

        updateValues();

        isAdd = true;

        pbSaveOrdered->setText(tr("Salvar"));
    }
}

void CounterInterface::onClick_pbCancelOrdered(){
    int op = QMessageBox::question(this, tr("Deleter todos os itens."),tr("Tem certeza que deseja cancelar este pedido?"), tr("Sim"), tr("Não"));

    if (op == 0){
        clearOrder();
    }
}

void CounterInterface::onClick_pbSaveOrdered()
{
    if (orderIdProduct.length() == 0 ){

        QMessageBox::warning(this, tr("Selecione um produto"), tr("O pedido deve ter pelo menos um produto"), tr("Voltar"));

        searchProduct->eSearch->Clear();

        searchProduct->eSearch->setFocus();

        return;
    }

    if (wPaymentView->totalValueOrder > wPaymentView->totalPaid){

        int op = QMessageBox::question(this, tr("Preço pago insuficiente"), tr("O valor pago é insuficiente para pagar o pedido.\nDeseja salvar o pedido dessa forma?"), tr("Sim"), tr("Não"));

        if (op == 1)
            return;
    }

    bool client = true;

    if (wPaymentView->idClient == 0 && wPaymentView->ePhone->isEmpty() && wPaymentView->ePhoneBusiness->isEmpty()){
        client = false;
    }

    // se o cliente tiver sido selecionado, ele vai verificar se o cliente se foi alterado
    if (! wPaymentView->idClient == 0){
        wPaymentView->dataClientF.clear();

        if (wPaymentView->typeClient == wPaymentView->NORMAL){

            wPaymentView->dataClientF << QString::number(wPaymentView->idClient) << wPaymentView->eName->text() << wPaymentView->ePhone->text() << wPaymentView->eStreet->text() << wPaymentView->eNumber->text() << wPaymentView->eComplement->text() << wPaymentView->eDistrict->text() << wPaymentView->eCep->text() << QString::number(wPaymentView->DSBShipping->value()) << wPaymentView->teNotes->toPlainText();

        }else{

            wPaymentView->dataClientF << QString::number(wPaymentView->idClient) << wPaymentView->eNameContactBusiness->text() << wPaymentView->eNameBusiness->text() << wPaymentView->eFantasyNameBusiness->text() << wPaymentView->eCNPJBusiness->text() << wPaymentView->ePhoneBusiness->text() << wPaymentView->eStreetBusiness->text() << wPaymentView->eNumberBusiness->text() << wPaymentView->eComplementBusiness->text() << wPaymentView->eDistrictBusiness->text() << wPaymentView->eCepBusiness->text() << QString::number(wPaymentView->DSBShippingBusiness->value()) << wPaymentView->teNotesBusiness->toPlainText();

        }

        bool equal = true;

        for (int i = 0; i < wPaymentView->dataClientI.length(); i++){
            if (wPaymentView->dataClientI[i] != wPaymentView->dataClientF[i]){
                equal = false;
                break;
            }
        }

        if (! equal){
            int op = QMessageBox::question(this, tr("Alteração do cliente."), tr("Tiveram alterações no cliente.\nDeseja salvá-las?"), "Sim", "Não");

            if (op == 0)
                if (! wPaymentView->order->updateClient(wPaymentView->dataClientF))
                    qDebug() << "[CounterInterface] [Botão Salvar Pedido]";
        }
    }

    if ((!(wPaymentView->ePhone->isEmpty() && wPaymentView->ePhoneBusiness->isEmpty())) && wPaymentView->idClient == 0){
        if (wPaymentView->typeClient == wPaymentView->NORMAL){
            if (isAvailable()){
                //seta os valores

                wPaymentView->idClient = order->nextIdClient();

                wPaymentView->order->phone = wPaymentView->ePhone->text();
                wPaymentView->order->name = wPaymentView->eName->text();
                wPaymentView->order->cep = wPaymentView->eCep->text();
                wPaymentView->order->street = wPaymentView->eStreet->text();
                wPaymentView->order->number = wPaymentView->eNumber->text();
                wPaymentView->order->complement = wPaymentView->eComplement->text();
                wPaymentView->order->district = wPaymentView->eDistrict->text();
                wPaymentView->order->shipping = wPaymentView->DSBShipping->value();
                wPaymentView->order->note = wPaymentView->teNotes->toPlainText();

                wPaymentView->order->insertClient();
            }
        }else{
            if (isAvailableBusiness()){
                //seta os valores

                wPaymentView->idClient = wPaymentView->order->nextIdClient();

                wPaymentView->order->phone = wPaymentView->ePhoneBusiness->text();
                wPaymentView->order->nameRespondible = wPaymentView->eNameContactBusiness->text();
                wPaymentView->order->cep = wPaymentView->eCepBusiness->text();
                wPaymentView->order->street = wPaymentView->eStreetBusiness->text();
                wPaymentView->order->number = wPaymentView->eNumberBusiness->text();
                wPaymentView->order->complement = wPaymentView->eComplementBusiness->text();
                wPaymentView->order->district = wPaymentView->eDistrictBusiness->text();
                wPaymentView->order->shipping = wPaymentView->DSBShipping->value();
                wPaymentView->order->note = wPaymentView->teNotesBusiness->toPlainText();
                wPaymentView->order->corporateName = wPaymentView->eNameBusiness->text();
                wPaymentView->order->CNPJ = wPaymentView->eCNPJBusiness->text();
                wPaymentView->order->fantasyName = wPaymentView->eFantasyNameBusiness->text();

                wPaymentView->order->insertClient();
            }
        }
    }

    wPaymentView->order->isAdd = isAdd;
    wPaymentView->order->typeClient = wPaymentView->typeClient;
    wPaymentView->order->idClient = wPaymentView->idClient;
    wPaymentView->order->totalValueOrderProducts = wPaymentView->totalValueOrder;
    wPaymentView->order->valuePaid = wPaymentView->totalPaid;
    wPaymentView->order->valueShipping = wPaymentView->DSBShipping->value();
    wPaymentView->order->orderQuantity = orderQuantity;
    wPaymentView->order->orderValues = orderValues;
    wPaymentView->order->orderSizes = orderSizes;
    wPaymentView->order->orderPartProduct = orderPartProduct;
    wPaymentView->order->orderIdProduct = orderIdProduct;
    wPaymentView->order->orderNameProduct = orderNameProduct;
    wPaymentView->order->orderNoteProduct = orderNoteProduct;

    QList<QString> orderPaymentForm;
    QList<QString> orderPaymentFormValue;

    if (wPaymentView->tablePaymentForm->model()->rowCount() > 0){
        for (int i = 0; i < wPaymentView->tablePaymentForm->model()->rowCount(); i++){
            orderPaymentForm << wPaymentView->tablePaymentForm->model()->index(i, 0).data().toString();
            orderPaymentFormValue << wPaymentView->tablePaymentForm->model()->index(i, 1).data().toString();
        }
    }

    wPaymentView->order->orderPaymentForm = orderPaymentForm;
    wPaymentView->order->orderPaymentFormValue = orderPaymentFormValue;

    wPaymentView->order->typeOrder = Order::COUNTER;

    if (wPaymentView->order->insertOrder())
    {
        int op = QMessageBox::question(this, tr("Salvo com sucesso"), tr("Pedido salvo com sucesso.\nDeseja imprimir o pedido agora?"), tr("Sim"), tr("Não"));

        if (op == 0)
            html.print(html.createHTMLOrder(order->idOrder.toInt()));
    }

    emit newOrder();

    clearOrder();
}

void CounterInterface::clearOrder()
{
    dataModelItems->clear();

    orderQuantity.clear();
    orderValues.clear();
    orderSizes.clear();
    orderPartProduct.clear();
    orderIdProduct.clear();
    orderNameProduct.clear();
    orderNoteProduct.clear();

    totalValueOrderProducts = 0;

    searchProduct->eSearch->Clear();

    wPaymentView->dataModelPayment->clear();

    wPaymentView->ePhone->Clear();

    wPaymentView->ePhoneBusiness->Clear();

    wPaymentView->lPaid->setText(tr("R$ 0,00"));

    wPaymentView->totalPaid = 0;

    updateValues();

    searchProduct->eSearch->Clear();

    searchProduct->eSearch->setFocus();
}

bool CounterInterface::isAvailable()
{
    if (wPaymentView->ePhone->isEmpty()){
        QMessageBox::warning(this, tr("Telefone inválido"), tr("Por favor, digite um número de telefone."), tr("Ok"));

        wPaymentView->ePhone->setFocus();

        return false;
    }

    if (wPaymentView->eName->isEmpty()){
        QMessageBox::warning(this, tr("Nome inválido"), tr("Por favor, digite um nome para o cliente."), tr("Ok"));

        wPaymentView->eName->setFocus();

        return false;

    }

    if (wPaymentView->eStreet->isEmpty()){
        QMessageBox::warning(this, tr("Rua inválido"), tr("Por favor, digite uma rua para o cliente."), tr("Ok"));

        wPaymentView->eStreet->setFocus();

        return false;

    }

    if (wPaymentView->eNumber->isEmpty()){
        QMessageBox::warning(this, tr("Número inválido"), tr("Por favor, digite um número."), tr("Ok"));

        wPaymentView->eNumber->setFocus();

        return false;
    }

    return true;
}

bool CounterInterface::isAvailableBusiness()
{
    if (wPaymentView->ePhoneBusiness->isEmpty()){
        QMessageBox::warning(this, tr("Telefone inválido"), tr("Por favor, digite um número de telefone."), tr("Ok"));

        wPaymentView->ePhoneBusiness->setFocus();

        return false;
    }

    if (wPaymentView->eNameContactBusiness->isEmpty()){
        QMessageBox::warning(this, tr("Nome inválido"), tr("Por favor, digite um nome para o cliente."), tr("Ok"));

        wPaymentView->eNameContactBusiness->setFocus();

        return false;

    }

    if (wPaymentView->eNameBusiness->isEmpty()){
        QMessageBox::warning(this, tr("Razão social inválida"), tr("Por favor, digite a razão social."), tr("Ok"));

        wPaymentView->eNameBusiness->setFocus();

        return false;
    }

    if (wPaymentView->eFantasyNameBusiness->isEmpty()){
        QMessageBox::warning(this, tr("Nome fantasia inválido"), tr("Por favor, digite no,e fantasia."), tr("Ok"));

        wPaymentView->eFantasyNameBusiness->setFocus();

        return false;
    }

    if (wPaymentView->eCNPJBusiness->isEmpty()){
        QMessageBox::warning(this, tr("CNPJ inválido"), tr("Por favor, digite um CNPJ válido."), tr("Ok"));

        wPaymentView->eCNPJBusiness->setFocus();

        return false;
    }

    if (wPaymentView->eStreetBusiness->isEmpty()){
        QMessageBox::warning(this, tr("Rua inválido"), tr("Por favor, digite uma rua para o cliente."), tr("Ok"));

        wPaymentView->eStreetBusiness->setFocus();

        return false;

    }

    if (wPaymentView->eNumberBusiness->isEmpty()){
        QMessageBox::warning(this, tr("Número inválido"), tr("Por favor, digite um número."), tr("Ok"));

        wPaymentView->eNumberBusiness->setFocus();

        return false;
    }


    return true;
}

void CounterInterface::onTriggered_actionName()
{
    actionName->setChecked(true);
    actionName->setEnabled(false);

    actionNickName->setChecked(false);
    actionNickName->setEnabled(true);
}

void CounterInterface::onTriggered_actionNickName()
{
    actionName->setChecked(false);
    actionName->setEnabled(true);

    actionNickName->setChecked(true);
    actionNickName->setEnabled(false);
}

void CounterInterface::editOrder(int idOrder)
{
    wPaymentView->order->loadDataOrder(idOrder);
}

void CounterInterface::loadDataOrder()
{
    qDebug() << Q_FUNC_INFO << "loadDataOrder";

    pbSaveOrdered->setText(tr("Editar")); // muda o texto

    isAdd = false;

    if (wPaymentView->order->typeClient == 1){
        wPaymentView->typeClient = wPaymentView->NORMAL;

        wPaymentView->onClick_pbSelectPeople();

        wPaymentView->ePhone->setText(wPaymentView->order->phoneClient);
    }else if (wPaymentView->typeClient == 2){

        wPaymentView->typeClient = wPaymentView->BUSINESS;

        wPaymentView->onClick_pbSelectBusiness();

        wPaymentView->ePhoneBusiness->setText(wPaymentView->order->phoneClient);
    }

    wPaymentView->idClient = wPaymentView->order->idClient;

    totalValueOrderProducts = wPaymentView->order->totalValueOrderProducts;

    orderQuantity = wPaymentView->order->orderQuantity;
    orderValues = wPaymentView->order->orderValues;
    orderSizes = wPaymentView->order->orderSizes;
    orderPartProduct = wPaymentView->order->orderPartProduct;
    orderIdProduct = wPaymentView->order->orderIdProduct;
    orderNameProduct = wPaymentView->order->orderNameProduct;
    orderNoteProduct = wPaymentView->order->orderNoteProduct;

    wPaymentView->dataModelPayment->clear();

    dataModelItems->clear();

    QList<QString> dataForModel;
    for (int i = 0; i < wPaymentView->order->orderPaymentForm.length(); i++){

        dataForModel.clear();

        dataForModel << wPaymentView->order->orderPaymentForm[i] << wPaymentView->order->orderPaymentFormValue[i];

        wPaymentView->dataModelPayment->add(dataForModel);
    }


    for (int i = 0; i < wPaymentView->order->orderIdProduct.length(); i++){
        dataForModel.clear();

        if (orderIdProduct[i].length() == 1)
        {

            dataForModel << wPaymentView->order->orderNameProduct[i] << wPaymentView->order->orderSizes[i] << QString::number(wPaymentView->order->orderQuantity[i]) << QString::number(wPaymentView->order->orderValues[i]) << wPaymentView->order->orderNoteProduct[i];

        }else{

            dataForModel << "Pizza mista" << wPaymentView->order->orderSizes[i] << QString::number(wPaymentView->order->orderQuantity[i]) << QString::number(wPaymentView->order->orderValues[i]) << wPaymentView->order->orderNoteProduct[i];

        }

        dataModelItems->add(dataForModel);
    }

    double totalPaid = 0;

    for(int i = 0; i < wPaymentView->tablePaymentForm->model()->rowCount(); i++){
        totalPaid += wPaymentView->tablePaymentForm->model()->index(i, 1).data().toDouble();
    }

    wPaymentView->lPaid->setText(wPaymentView->tools.convertMoney(QString::number(totalPaid)));

    wPaymentView->totalPaid = totalPaid;

    if (totalPaid - totalValueOrderProducts >= 0)
        wPaymentView->lTransshipment->setText(wPaymentView->tools.convertMoney(QString::number(totalPaid - totalValueOrderProducts)));
    else
        wPaymentView->lTransshipment->setText("R$ 0,00");


    // da update nos valores

    updateValues();
}

void CounterInterface::valueChanged_DSBValueProduct(double value)
{
    if (! productName.isEmpty())
        setPreview(productName, value, sbAmount->value());
    else
        DSBValueProduct->setValue(0);
}
