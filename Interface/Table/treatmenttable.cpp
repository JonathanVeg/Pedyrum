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


 #include "treatmenttable.h"
#include <QDebug>
#include <QMessageBox>

TreatmentTable::TreatmentTable(QWidget *parent) :
    QWidget(parent)
{
    createComponent();
    statics();
    connections();
}

void TreatmentTable::createComponent()
{
    QList<QString> headers;

    headers << tr("Nome") << tr("Tamanho") << tr("Quantidade") << tr("Valor") << tr("Anotação");

    dataModelItems = new DataModel(this, headers);

    order = new Order("connectionTable");

    for (int i = 0; i < 100; ++i) {
        _dataItem[i] = new DataItem(this);
    }

    wProduct = new QWidget(this);
    lProduct = new QLabel(wProduct);
    searchProduct = new Search(wProduct,tr("Pesquisar Produtos"));
    productPreview = new QTextBrowser(wProduct);
    DSBValueProduct = new DoubleSpinBox(wProduct);
    cbSize = new ComboBox(wProduct);
    sbAmount = new SpinBox(wProduct);
    lPizza = new Label(wProduct,true,100);
    lNotes = new Label(wProduct,true,100);

    pbConfirmProduct = new QPushButton(wProduct);
    pbLeaveProduct = new QPushButton(wProduct);

    wNotes = new QWidget(this);
    lBgNotes = new QLabel(wNotes);
    teProductNotes = new QTextEdit(wNotes);
    pbConfirmNotes = new QPushButton(tr("Concluido"),wNotes);

    cbBartender = new ComboBox(this);
    lTable = new QLabel(this);
    lTableText = new QLabel(this);
    lTableTitle = new QLabel(this);
    pbBack = new QPushButton(tr("Voltar"),this);

    wPizzaMixed = new PizzaMixed();
}

void TreatmentTable::input()
{
    isAdd = true;

    lProduct->setPixmap(QPixmap(":/treatment/fieldBackground"));
    lProduct->setScaledContents(true);

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

    DSBValueProduct->setPrefix(tr("R$ "));

    lPizza->setImage(":/treatment/icopizza");
    lPizza->setHover(":/treatment/icopizza-hover");

    lNotes->setImage(":/treatment/icoNote");
    lNotes->setImage(":/treatment/icoNote-hover");

    sbAmount->setPrefix(tr("Quant "));
    sbAmount->setFont(this->font());
    sbAmount->setMinimum(1);
    sbAmount->setValue(1);
    sbAmount->setMaximum(99999);

    lTable->setStyleSheet("background-color: green;"
                          "border-radius: 5px;border: 0.5px solid rgba(255,255,255,50);"
                          "color: rgba(220,220,220,220);");


    cbBartender->addItem(tr("Selecione o garçom"));
    qDebug() << wPaymentView->order->bartenders();
    bartenders = wPaymentView->order->bartenders();
    cbBartender->addItems(bartenders[1]);

    QFont f;
    f.setPointSize(26);
    lTableText->setStyleSheet("color: rgb(245,245,245);");
    lTableText->setAlignment(Qt::AlignCenter);
    lTableText->setFont(f);

    f.setPointSize(18);
    lTableTitle->setText(tr("Mesa atual"));
    lTableTitle->setStyleSheet("color: rgba(255,255,255,200);");
    lTableTitle->setAlignment(Qt::AlignCenter);
    lTableTitle->setFont(f);

    f.setPointSize(10);
    pbBack->setFont(f);

    lBgNotes->setStyleSheet("background-color: rgba(0,0,0,190);"
                            "border-radius: 5px;border: 0.5px solid rgba(255,255,255,50);");

    teProductNotes->setStyleSheet("background-color: qlineargradient"
                                  "(spread:pad, x1:0, y1:1, x2:0.165045,"
                                  "y2:0, stop:0 rgba(254, 255, 180, 255),"
                                  "stop:0.721591 rgba(255, 250, 205, 255));"
                                  "border-radius: 5px;border: 1px solid #C0C0C0;");

    wNotes->hide();
    wProduct->show();

    setPreview();

    // searchProduct->setCompleter();

    searchProduct->eSearch->setNextComponent(cbSize);

    cbSize->setNextComponent(DSBValueProduct);

    DSBValueProduct->setNextComponent(sbAmount);

}

void TreatmentTable::started(){
    try {
        searchProduct->setCompleter(order->bd.getCompleter("product"));
    }catch(...){
        qDebug() << "completer product não achado";
    }
}

void TreatmentTable::statics()
{
    searchProduct->setGeometry(25,15,300,28);

    productPreview->setGeometry(10,searchProduct->height() + 50,210,170);
    cbSize->setGeometry(productPreview->width() + 15,70,110,26);
    DSBValueProduct->setGeometry(cbSize->x(),cbSize->y() + 40, cbSize->width(), 26);
    sbAmount->setGeometry(DSBValueProduct->x(),DSBValueProduct->y() + 40, DSBValueProduct->width(), 26);
    lPizza->setGeometry(sbAmount->x() + 10,190,40,40);
    lNotes->setGeometry(lPizza->x() + lPizza->width() + 10,190,40,40);

    pbLeaveProduct->setGeometry(57,275,90,28);
    pbConfirmProduct->setGeometry(203,275,90,28);
}

void TreatmentTable::adjustLayout()
{
    wProduct->setGeometry(215,15,350,310);
    lProduct->setGeometry(0,0,wProduct->width(),wProduct->height());

    cbBartender->setGeometry(20,50,140,28);
    lTableTitle->setGeometry(30,85,120,80);
    lTable->setGeometry(30,150,120,80);
    lTableText->setGeometry(lTable->geometry());

    pbBack->setGeometry(40,240,100,32);

    wNotes->setGeometry(wProduct->geometry());
    lBgNotes->setGeometry(0,0,wNotes->width(),wNotes->height());
    teProductNotes->setGeometry(10,10,wNotes->width() - 20,wNotes->height() - 60);
    pbConfirmNotes->setGeometry(wNotes->width()/2 - 45,teProductNotes->y() + teProductNotes->height() + 10,90,28);
}

void TreatmentTable::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void TreatmentTable::onClick_pbBack()
{
    wNotes->hide();
    wProduct->show();

    emit back();

    orderQuantity.clear();
    orderValues.clear();
    orderSizes.clear();
    orderPartProduct.clear();
    orderIdProduct.clear();
    orderNameProduct.clear();
    orderNoteProduct.clear();

    listOrderIdProduct.clear();
    listOrderNameProduct.clear();
    listOrderSizeProduct.clear();
    listOrderValueProduct.clear();
    listOrderQntProduct.clear();
    listOrderNoteProduct.clear();
}

void TreatmentTable::connections()
{
    connect(wPizzaMixed, SIGNAL(emitPizzaMista(QList<int>,QList<QString>,QList<QString>,QString,int,double,QString,bool,int)), this, SLOT(slotPizzaMista(QList<int>,QList<QString>,QList<QString>,QString,int,double,QString,bool,int)));

    connect(this, SIGNAL(editPizzaMixed(QList<int>,QList<QString>,QList<QString>,QString,int,double,QString,int)), wPizzaMixed, SLOT(editPizzaMixed(QList<int>,QList<QString>,QList<QString>,QString,int,double,QString,int)));

    connect(searchProduct, SIGNAL(textSearch(QString)), SLOT(getSearchProduct(QString)));

    connect(sbAmount, SIGNAL(returnPressed()), SLOT(onClick_pbConfirmProduct()));

    connect(pbConfirmProduct, SIGNAL(clicked()), SLOT(onClick_pbConfirmProduct()));

    connect(lPizza, SIGNAL(onClick()), SLOT(onClick_lPizza()));

    connect(pbLeaveProduct, SIGNAL(clicked()), SLOT(onClick_pbLeaveProduct()));

    connect(sbAmount, SIGNAL(valueChanged(int)), SLOT(valueChanged_sbAmount(int)));

    connect(pbBack,SIGNAL(clicked()),SLOT(onClick_pbBack()));

    connect(pbConfirmNotes,SIGNAL(clicked()),SLOT(onClick_pbConfirmNotes()));

    connect(lNotes,SIGNAL(onClick()),SLOT(onClick_lNotes()));

    connect(DSBValueProduct, SIGNAL(valueChanged(double)), SLOT(valueChanged_DSBValueProduct(double)));

}

void TreatmentTable::load(int index)
{
    _selected = index;

    QString temp;

    temp.setNum(_selected);
    lTableText->setText(temp);
}

void TreatmentTable::onClick_lNotes()
{
    wProduct->hide();
    wNotes->show();
}

void TreatmentTable::onClick_pbConfirmNotes()
{
    wNotes->hide();
    wProduct->show();
}

void TreatmentTable::setPreview()
{
    QString html;

    html = "<html>"
            "<p align=\"center\"> Nome do produto</p>"
            "<p align=\"center\"> Valor em reais</p>"
            "<p align=\"center\"> Unidade(s)</p>"
            "<p align=\"center\"> Valor total </p>"
            "</html>";

    QFont f;
    f.setPointSize(12);

    productPreview->setFont(f);

    productPreview->setText(html);
}

void TreatmentTable::setPreview(QString nome, float value, int quant)
{
    QString html;
    QString quantText, valueText, totalText;

    valueText.setNum(value);
    quantText.setNum(quant);
    totalText.setNum(value * quant);

    QFont f;
    f.setPointSize(12);

    productPreview->setFont(f);

    html = "<html>"
            "<p align=\"center\">" + nome + "</p>"
            "<p align=\"center\">" + valueText + tr(" Reais") + "</p>"
            "<p align=\"center\">" + quantText + tr(" Unidade(s)") + "</p>"
            "<p align=\"center\">" + totalText +  tr(" Reais") + "</p>"
            "</html>";

    productPreview->setText(html);
}

void TreatmentTable::setCompleterProduct(QCompleter* completer)
{
    searchProduct->setCompleter(completer);
}

void TreatmentTable::valueChanged_DSBValueProduct(double value)
{
    if (! productName.isEmpty()){
        setPreview(productName, value, sbAmount->value());
    }
    else
        DSBValueProduct->setValue(0);
}

void TreatmentTable::valueChanged_sbAmount(int value)
{
    if (listSizes.length() > 0){
        //        float value = listValues[cbSize->currentIndex()].toDouble();
        float value = DSBValueProduct->value();

        dataOrderValueProduct = listValues[cbSize->currentIndex()].toDouble();

        dataOrderQntProduct = sbAmount->value();

        setPreview(productName, value, sbAmount->value());
    }
}

void TreatmentTable::onClick_pbConfirmProduct()
{
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

    sendModelItems(dataModelItems);

    _dataItem[_selected]->setFree(false);
    _dataItem[_selected]->setValueSimple(calculateValueSimple());
    _dataItem[_selected]->setValueComplex(calculateValueComplex());

    updateData(0, true);

    onClick_pbLeaveProduct();


    qDebug() << " --- DataOrder --- ";
    qDebug() << orderQuantity;
    qDebug() << orderValues;
    qDebug() << orderSizes;
    qDebug() << orderPartProduct;
    qDebug() << orderIdProduct;
    qDebug() << orderNameProduct;
    qDebug() << orderNoteProduct;
    qDebug() << " ----------------- ";
}

void TreatmentTable::onClick_pbLeaveProduct()
{
    listSizes.clear();
    listValues.clear();
    sbAmount->setValue(1);
    cbSize->setCurrentIndex(0);
    teProductNotes->clear();
    searchProduct->eSearch->Clear();
    DSBValueProduct->setValue(0);

    productName.clear();

    dataOrderIdProduct.clear();

    setPreview();

    pbLeaveProduct->setText("Limpar");
    pbConfirmProduct->setText("Confirmar");
}

void TreatmentTable::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
        onClick_pbBack();

    switch(event->modifiers()){

    case (Qt::ControlModifier):{
        saveOrder();
    }

    }
}

void TreatmentTable::onClick_lPizza()
{
    wPizzaMixed->show();
}

void TreatmentTable::getSearchProduct(QString text)
{
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

                DSBValueProduct->setValue(value);

                setPreview(productName, value, sbAmount->value());
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

void TreatmentTable::updateValues()
{

    //wPaymentView->updateValuesTotal(totalValueOrderProducts);

    //    lSubtotalOrdered->setText(tr("SubTotal R$: ") + QString::number(totalValueOrderProducts));

    //    totalValueOrder = totalValueOrderProducts + DSBShipping->value();

    //    lTotalOrdered->setText(tr("Total R$: ") + QString::number(totalValueOrder));

    //    if (DSBPaid->value() > totalValueOrder)
    //        lTransshipment->setText(tr("Troco R$: ") + QString::number(DSBPaid->value() - totalValueOrder));
    //    else
    //        lTransshipment->setText(tr("Troco R$: 0"));
}

void TreatmentTable::setDataModelItems(DataModel *dataModelItems)
{
    this->dataModelItems = dataModelItems;
}

void TreatmentTable::setGeneralData(GeneralData *generalData){
    _generalData = generalData;
}

double TreatmentTable::calculateValueSimple(int index){
    double total = 0;

    if (index == -1)
        index = _selected;

    DataModel * model = _generalData->getModelItems(index);

    for (int i = 0; i < model->rowCount(QModelIndex()); i++){
        total += model->index(i, 2).data().toDouble() * model->index(i, 3).data().toDouble();
    }

    return total;
}

double TreatmentTable::calculateValueComplex(){
    descriptionValueComplex = "";

    if (_dataItem[_selected]->isChildren()){
        double fatherValue = calculateValueSimple(_dataItem[_selected]->father());
        double yourValue = calculateValueSimple();

        double totalValue = fatherValue + yourValue;

        descriptionValueComplex += "Descrição do pedido (formado 'mesa: valor').\n";

        descriptionValueComplex += toStr(selected()) + ": " + tools.convertMoney(toStr(yourValue)) + ";\n";

        descriptionValueComplex += toStr(_dataItem[_selected]->children()) + ": " + tools.convertMoney(toStr(fatherValue)) + ".\n";

        descriptionValueComplex += "\nValor total: " + tools.convertMoney(toStr(totalValue));


        return totalValue;
    }

    if (_dataItem[_selected]->isFather()){
        double childrenValue = calculateValueSimple(_dataItem[_selected]->children());

        double yourValue = calculateValueSimple();

        double totalValue = childrenValue + yourValue;

        descriptionValueComplex += "Descrição do pedido (formado 'mesa: valor').\n";

        descriptionValueComplex += toStr(selected()) + ": " + tools.convertMoney(toStr(yourValue)) + ";\n";

        descriptionValueComplex += toStr(_dataItem[_selected]->children()) + ": " + tools.convertMoney(toStr(childrenValue)) + ".\n";

        descriptionValueComplex += "\nValor total: " + tools.convertMoney(toStr(totalValue));


        return totalValue;
    }

    if (_dataItem[_selected]->isCompound()){
        double totalValue = 0;

        double yourValue = calculateValueSimple();

        descriptionValueComplex += "Descrição do pedido (formado 'mesa: valor').\n";

        descriptionValueComplex += toStr(selected()) + ": " + tools.convertMoney(toStr(yourValue)) + ";\n";

        foreach (int i, _dataItem[_selected]->listCompound()){

            double compoundValue = calculateValueSimple(i);

            descriptionValueComplex += toStr(i) + ": " + tools.convertMoney(toStr(compoundValue)) + ".\n";

            totalValue += compoundValue;
        }

        totalValue += yourValue;

        descriptionValueComplex += "\nValor total: " + tools.convertMoney(toStr(totalValue));

        return totalValue;
    }

    // se não for nada, seu valor complexo é o próprio valor dela
    return calculateValueSimple();
}

void TreatmentTable::slotPizzaMista(QList<int> listOrderPartProduct, QList<QString> listOrderIdProduct, QList<QString> listOrderNameProduct, QString note, int quantity, double value, QString size, bool insert, int row){

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

    setDataModelItems(dataModelItems);

    sendModelItems(dataModelItems);

    totalValueOrderProducts += value * quantity;

    updateValues();

}

void TreatmentTable::doubleClicked_tableItem(QModelIndex index){
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

void TreatmentTable::setTableItem(TableView *tableItem){
    this->tableItem = tableItem;
}

void TreatmentTable::saveOrder(){
    // verificacoes

    individual = true;

    qDebug() << QString(Q_FUNC_INFO).split(" ").last();

    qDebug() << QString("RESUMO DA MESA %1 A SER FECHADA").arg(QString::number(selected()));
    qDebug() << "\tisFather?" << dataItem(selected())->isFather();
    if (dataItem(selected())->isFather())
        qDebug() << "\tyourChildren:" << dataItem(selected())->children();
    qDebug() << "\tisChildren?" << dataItem(selected())->isChildren();
    if (dataItem(selected())->isChildren())
        qDebug() << "\tyourFather:" << dataItem(selected())->father();
    qDebug() << "\tisCompound?" << dataItem(selected())->isCompound();
    if (dataItem(selected())->isFather())
        qDebug() << "\tyour brothers:" << dataItem(selected())->listCompound();

    if ((! _dataItem[selected()]->isChildren()) && (! _dataItem[selected()]->isCompound())){
        // if atoa, mas não deve ser apagado por hr (questao de teste)
        qDebug() << "[TreatmentTable] [saveOrder] mesa simples";
    }

    if (cbBartender->currentIndex() == 0){
        int op = QMessageBox::warning(this, "Garçom", "Você não selecionou nenhum garçom para essa mesa.\nDeseja salvar o pedido dessa forma?", "Sim", "Não");

        if (op == 1){
            cbBartender->setFocus();

            return;
        }
    }else{
        int index = cbBartender->currentIndex() - 1;

        qDebug() << "cbBartender" << bartenders << index;

        wPaymentView->order->setIdSelectedBartender(bartenders[index][0].toInt());
    }


    if (dataItem(selected())->isChildren()){
        qDebug() << "[TreatmentTable] [saveOrder] mesa que possui pai";

        wPaymentView->order->isChildren = true;

        QString father = QString::number(dataItem(selected())->father());

        father = father.length() > 1? father: "0" + father;


        int op = QMessageBox::question(this, tr("Fechamendo especial"), QString("A conta dessa mesa foi marcada para ser paga pela mesa %1. Deseja imprimir a conta somente dessa mesa ou fechar as duas contas (mesas %1, %2) juntas?").arg(father, QString::number(selected())), tr("Só ela"), tr("As duas"));

        wPaymentView->order->linkedTables = father;

        /// if (op == 0) fecha como mesa simples

        if (op == 1){
            // junta os dados dos pedidos aqui

            individual = false;

            DataOrder * dataOrderFather = _generalData->getDataOrder(father.toInt());

            orderQuantity << dataOrderFather->orderQuantity; // concatena as listas
            orderValues << dataOrderFather->orderValues;
            orderSizes << dataOrderFather->orderSizes;
            orderPartProduct << dataOrderFather->orderPartProduct;
            orderIdProduct << dataOrderFather->orderIdProduct;
            orderNameProduct << dataOrderFather->orderNameProduct;
            orderNoteProduct << dataOrderFather->orderNoteProduct;
        }
    }else
        wPaymentView->order->isChildren = false;

    if (dataItem(selected())->isFather()){
        qDebug() << "[TreatmentTable] [saveOrder] mesa que possui filho";

        wPaymentView->order->isFather = true;

        QString children = QString::number(dataItem(selected())->children());

        children = children.length() > 1? children : "0" + children;

        wPaymentView->order->linkedTables = children;

        int op = QMessageBox::question(this, tr("Fechamendo especial"), QString("Esta mesa também pagará a conta da mesa %1. Ao fechar a conta dessa mesa, automaticamente a conta da mesa %1 será fechada.\nDeseja fechar a conta agora?").arg(children, QString::number(selected())), tr("Sim"), tr("Não"));

        if (op == 0){

            individual = false;

            // junta os dados aqui
            DataOrder * dataOrderFather = _generalData->getDataOrder(children.toInt());

            orderQuantity << dataOrderFather->orderQuantity; // concatena as listas
            orderValues << dataOrderFather->orderValues;
            orderSizes << dataOrderFather->orderSizes;
            orderPartProduct << dataOrderFather->orderPartProduct;
            orderIdProduct << dataOrderFather->orderIdProduct;
            orderNameProduct << dataOrderFather->orderNameProduct;
            orderNoteProduct << dataOrderFather->orderNoteProduct;

            // qDebug("Dados do pai");
            // qDebug() << "\t" << dataOrderFather.orderValues;
            // qDebug() << "\t" << dataOrderFather.orderSizes;
            // qDebug() << "\t" << dataOrderFather.orderPartProduct;
            // qDebug() << "\t" << dataOrderFather.orderIdProduct;
            // qDebug() << "\t" << dataOrderFather.orderNameProduct;
            // qDebug() << "\t" << dataOrderFather.orderNoteProduct;
            // qDebug("Fim do dados do pai");
        }

        if (op == 1) return; // fecha nada
    }else
        wPaymentView->order->isFather = false;

    if (dataItem(selected())->isCompound()){
        qDebug() << "[TreatmentTable] [saveOrder] mesa junta";

        wPaymentView->order->isCompound = true;

        QList<QString> compounds;
        QString linkedTables = "";
        foreach (int compound, dataItem(selected())->listCompound()){
            QString compoundStr = QString::number(compound);
            compounds << compoundStr;
            linkedTables += compoundStr.length() > 1? compoundStr: "0" + compoundStr;
            linkedTables += ",";
        }

        linkedTables = linkedTables.mid(0, linkedTables.length() - 1);

        wPaymentView->order->linkedTables = linkedTables;

        QString question = "Esta mesa está junta com as mesas: ";

        foreach(QString c, compounds)
            question += c + ", ";

        question = question.mid(0, question.length() - 2) + ".\nDeseja fechar a conta de todas as mesas do grupo ou só dessa mesa?";

        int op = QMessageBox::question(this, tr("Fechamendo especial"), question, tr("Só ela"), tr("Todas"));

        /// if (op == 0) fecha simples, deixa passar


        if (op == 1){
            individual = false;

            foreach(QString compound, compounds){
                DataOrder * dataOrderFather = _generalData->getDataOrder(compound.toInt());

                orderQuantity << dataOrderFather->orderQuantity; // concatena as listas
                orderValues << dataOrderFather->orderValues;
                orderSizes << dataOrderFather->orderSizes;
                orderPartProduct << dataOrderFather->orderPartProduct;
                orderIdProduct << dataOrderFather->orderIdProduct;
                orderNameProduct << dataOrderFather->orderNameProduct;
                orderNoteProduct << dataOrderFather->orderNoteProduct;


                // for (int i = 0; i < _generalData->getModelPayment(compound.toInt())->rowCount(QModelIndex()); i++){

                //     dataModelPayment->add(_generalData->getModelPayment(compound.toInt())->row(i));

                // }
            }
        }else{
            individual = true;
        }
    }else
        wPaymentView->order->isCompound = false;

    wPaymentView->order->idBartender = 0;

    wPaymentView->order->countPeople = dataItem(_selected)->isMedia()? dataItem(_selected)->countPeople(): 1;

    linkedTablesStr = wPaymentView->order->linkedTables;

    linkedTables.clear();

    foreach (QString table, linkedTablesStr.split(","))
        linkedTables << table.toInt();

    linkedTables << selected();

    // recalcula os valores
    {
        double total = 0;

        for (int i = 0; i < orderQuantity.length(); i++){
            total += orderQuantity[i] * orderValues[i];
        }

        wPaymentView->totalValueOrder = total;
    }

    // qDebug() << " ----- dados do pedido ----- após juntar -----";
    // qDebug() << orderQuantity ; // dataOrderFather.orderQuantity; // concatena as listas
    // qDebug() << orderValues ; // dataOrderFather.orderValues;
    // qDebug() << orderSizes ; // dataOrderFather.orderSizes;
    // qDebug() << orderPartProduct ; // dataOrderFather.orderPartProduct;
    // qDebug() << orderIdProduct ; // dataOrderFather.orderIdProduct;
    // qDebug() << orderNameProduct ; // dataOrderFather.orderNameProduct;
    // qDebug() << orderNoteProduct ; // dataOrderFather.orderNoteProduct;
    // qDebug() << " ---------------------------------------------";

    int op = QMessageBox::question(this, "Média", "Deseja imprimir a média por pessoa no pedido?", "Sim", "Não");

    if (op == 1)
        saveOrderPt2();
    else{
        SBGroupMedia->setFocus();
        wGroupMedia->show();
    }

}
void TreatmentTable::saveOrderPt2(bool isMedia, int countPeople){
    // mesa simples

    if (isMedia && countPeople >= 2){
        wPaymentView->order->isMedia = true;
        wPaymentView->order->countPeople = countPeople;
    }
    else
        wPaymentView->order->isMedia = false;

    if (orderIdProduct.length() == 0 ){

        QMessageBox::warning(this, tr("Selecione um produto"), tr("O pedido deve ter pelo menos um produto"), tr("Voltar"));

        searchProduct->eSearch->Clear();

        searchProduct->eSearch->setFocus();

        return;
    }

    calculateValueComplex();

    if (wPaymentView->totalValueOrder > wPaymentView->totalPaid){

        QString message = "O valor pago é insuficiente para pagar o pedido.\nDeseja salvar o pedido dessa forma?";

        if (individual == false)
            if (descriptionValueComplex.length() > 0)
                message += "\n\n" + descriptionValueComplex;

        int op = QMessageBox::question(this, tr("Preço pago insuficiente"), message, tr("Sim"), tr("Não"));


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

    wPaymentView->order->typeOrder = Order::TABLE;

    wPaymentView->order->currentTable = selected();

    qDebug("Dados do pedido ANTES DE SALVAR");
    qDebug() << "\t" << wPaymentView->order->orderValues;
    qDebug() << "\t" << wPaymentView->order->orderSizes;
    qDebug() << "\t" << wPaymentView->order->orderPartProduct;
    qDebug() << "\t" << wPaymentView->order->orderIdProduct;
    qDebug() << "\t" << wPaymentView->order->orderNameProduct;
    qDebug() << "\t" << wPaymentView->order->orderNoteProduct;
    qDebug("Fim do dados do pedido");

    if (wPaymentView->order->insertOrder())
    {
        int op = QMessageBox::question(this, tr("Salvo com sucesso"), tr("Pedido salvo com sucesso.\nDeseja imprimir o pedido agora?"), tr("Sim"), tr("Não"));

        if (op == 0)
            html.print(html.createHTMLOrder(order->idOrder.toInt()));

        // aqui ele tem que mandar fechar tds as mesas
    }

    emit newOrder();

    clearOrder();

}

void TreatmentTable::setPaymentView(PaymentView *paymentView)
{
    wPaymentView = paymentView;
}

bool TreatmentTable::isAvailable(){
    qDebug() << "FAZER: isAvailable()";
    return true;
}

bool TreatmentTable::isAvailableBusiness(){
    qDebug() << "FAZER isAvailableBusiness()";
    return true;
}

void TreatmentTable::clearOrder(){
    linkedTables.clear();

    if (! individual){
        if (wPaymentView->order->linkedTables.length() > 0){
            foreach(QString table, wPaymentView->order->linkedTables.split(",")){
                linkedTables << table.toInt();
            }
        }
    }else{

    }

    linkedTables << selected();

    foreach(usint table, linkedTables){
        _generalData->clearDataOrder(table);

        dataItem(table)->setFree(true);

        dataItem(table)->resetValues();

        emit closeTable();// fechar a mesa visualmente aqui
    }
}

void TreatmentTable::removeAllProducts(){
    orderQuantity.clear();
    orderValues.clear();
    orderSizes.clear();
    orderPartProduct.clear();
    orderIdProduct.clear();
    orderNameProduct.clear();
    orderNoteProduct.clear();


    wPaymentView->totalValueOrder = 0;

    while (dataModelItems->rowCount(QModelIndex()) > 0)
        dataModelItems->removeRow(0);

    sendModelItems(dataModelItems);
}

void TreatmentTable::removeProduct(usint index){
    qDebug() << "[TreatmentTable] [removeProduct] index:" << index;

    orderQuantity.removeAt(index);
    orderValues.removeAt(index);
    orderSizes.removeAt(index);
    orderPartProduct.removeAt(index);
    orderIdProduct.removeAt(index);
    orderNameProduct.removeAt(index);
    orderNoteProduct.removeAt(index);

    // recalcula os valores
    {
        double total = 0;

        for (int i = 0; i < orderQuantity.length(); i++){
            total += orderQuantity[i] * orderValues[i];
        }

        wPaymentView->totalValueOrder = total;
    }

    dataModelItems->removeRow(index);

    sendModelItems(dataModelItems);
}

void TreatmentTable::clearAllOrders()
{
    wPaymentView->order->bd.exec("delete from productitem; delete from itemorder; delete from paymentorder; delete from ordered; delete from treatmenttable");
}

void TreatmentTable::setSelectedTable(int index)
{
    _selected = index;
}

void TreatmentTable::setWGroupMedia(QWidget *wGroupMedia, SpinBox *SBGroupMedia){
    this->wGroupMedia = wGroupMedia;

    this->SBGroupMedia = SBGroupMedia;
}

QString TreatmentTable::toStr(usint value)
{
    return QString::number(value);
}

QString TreatmentTable::toStr(double value)
{
    return QString::number(value);
}

QString TreatmentTable::toStr(int value)
{
    return QString::number(value);
}
