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


#include "makehtml.h"
#include <QTextEdit>
#include <QPrinter>
#include <QPrintDialog>

MakeHTML::MakeHTML(QObject *parent) :
    QObject(parent)
{
    setSizeFont(9);

    numberOfCopy = 0;

    order = new Order("connectionOrderHTML");

    config = new Config(this);

    printer = new TextPrinter(this);
}

void MakeHTML::start(QSqlDatabase *sdb)
{
    order->start(sdb);
    config->start(sdb);

    reload();
}

QString MakeHTML::createHTMLOrder(int idOrder)
{

    order->loadDataOrder(idOrder);

    html.clear();

    if(idOrder == 0)
        return html;

    /// data e hora do Pedido

    QString dateOrder;
    QString hourOrder;

    /// Mesa

    bool _isTransfer = false;
    bool _isMedia = false;

    QString tableSubsidized;
    QString tableBuyer;
    QString numberPeople;
    QString valueMedia;
    QString numberTable;

    if (order->bd.search("select dateorder, timeorder from ordered where idorder = " + QString::number(idOrder))){
        dateOrder = tools.convertDate2(order->bd.loadSimple(0));
        hourOrder = order->bd.loadSimple(1);
    }

    {
        if (order->bd.search("select * from treatmenttable where idorder = " + QString::number(idOrder))){
            QList<QString> data = order->bd.loadLine(0);

            typeOrder = 2;

            // Data:
            // 0: id;
            // 1: ismedia;
            // 2: iscompount;
            // 3: ischildren;
            // 4: isfather;
            // 5: idbartender;
            // 6: linkedtables;
            // 7: countpeople;
            // 8: currenttable

            qDebug() << Q_FUNC_INFO << "DATA: " << data;

            if (data[1] == "true"){
                _isMedia = true;
                numberPeople = data[7];
                valueMedia = QString::number(order->totalValueOrderProducts / numberPeople.toInt());
            }

            if (data[3] == "true"){
                _isTransfer = true;
                tableSubsidized = data[8];
                tableBuyer = data[6];

                if (tableBuyer.length() == 1)
                    tableBuyer = "0" + tableBuyer;
            }

            if (data[4] == "true"){
                _isTransfer = true;
                tableSubsidized = data[6];
                tableBuyer = data[8];

                if (tableBuyer.length() == 1)
                    tableBuyer = "0" + tableBuyer;
            }

            numberTable = data[8];
        }
    }

    /// Cliente

    QString businessHTML;
    QString clientHTML;
    QString clientBusinessHTML;

    /// Mexa nas variaveis daqui pra baixo.

    QString nameBusiness;
    QString phoneBusiness;
    QString addressBusiness;
    QString districtBusiness;
    QString cityBusiness;
    QString ufBusiness;
    QString dateTime;

    if (order->bd.search("select * from business")){
        nameBusiness = order->bd.loadSimple(0);
        phoneBusiness = order->bd.loadSimple(1);
        addressBusiness = order->bd.loadSimple(2);
        districtBusiness = order->bd.loadSimple(3);
        cityBusiness = order->bd.loadSimple(4);
        ufBusiness = order->bd.loadSimple(5);
    }

    QString numberOrder = order->number;

    if (order->bd.search("select typeorder from ordered where idorder = " + QString::number(idOrder))){

        qDebug() << "typeOrder" << order->bd.loadLine(0);

        if (order->bd.loadSimple(0) == "Balcão")
            typeOrder = 1;

        if (order->bd.loadSimple(0) == "Mesa")
            typeOrder = 2;

        if (order->bd.loadSimple(0) == "Delivery")
            typeOrder = 3;
    }

    bool isPaid = order->isPaid; /// var usada para dizer se o pedido já foi pago ou não

    QString nameClient = order->client->getName();

    QString addressClient = order->client->getStreet() + ", " + order->client->getNumber() + " " + order->client->getComplement() + ", CEP: " + order->client->getCEP(); // rua, numero, complemento, cep

    QString noteClient = order->client->getNote();
    QString districtClient = order->client->getDistrict();
    QString phoneClient = order->client->getPhone();

    // só colocar os valores aqui

    QString nameClientBusiness = order->client->getFantasyName();

    QString addressClientBusiness = order->client->getStreet() + ", " + order->client->getNumber() + " " + order->client->getComplement() + ". " + order->client->getDistrict();

    QString districtClientBusiness = order->client->getDistrict();
    QString phoneClientBusiness = order->client->getPhone();
    QString noteClientBusiness = order->client->getNote();

    QString transshipment;
    QString total;
    QString deliveryRate;
    QString Subtotal;
    QString valuePaid;

    Subtotal = QString::number(order->totalValueOrderProducts);
    Subtotal = tools.convertMoney(Subtotal);

    if (order->valueShipping > 0){
        deliveryRate = tools.convertMoney(QString::number(order->valueShipping));

        total = tools.convertMoney(QString::number(order->valueShipping + order->totalValueOrderProducts));

    }else{
        total = Subtotal;
    }

    valuePaid = tools.convertMoney(QString::number(order->valuePaid));

    transshipment = tools.convertMoney(QString::number(order->valuePaid - (order->valueShipping + order->totalValueOrderProducts)));

    QString listItens;

    /// Dados da empresa

    dateTime = dateOrder + " - " + hourOrder;

    businessHTML = "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\"> p, li { white-space: pre-wrap; }"
            "</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:" + H5 + "; font-weight:400; font-style:normal;\">"
            "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px;"
            "margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:" + H5 + ";\">" + nameBusiness + "</span></p>"
            "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
            "text-indent:0px;\"><span style=\" font-size:" + H5 + ";\">" + addressBusiness + "</span></p>"
            "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
            "text-indent:0px;\"><span style=\" font-size:" + H5 + ";\">" + districtBusiness + ", " + cityBusiness + ", " + ufBusiness + "</span></p>"
            "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
            "text-indent:0px;\"><span style=\" font-size:" + H5 + ";\">" + phoneBusiness + "</span></p>"
            "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px;"
            "margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:" + H5 + ";\">" + dateTime + "</span></p>"
            "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px;"
            "margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:" + H5 + ";\"></p>";

    if(nameBusiness != "")
        html = businessHTML;
    else
        html = "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\"> p, li { white-space: pre-wrap; }"
                "</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:" + H5 + "; font-weight:400; font-style:normal;\">"
                "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px;";

    /// Indentificador de pedido

    QString nameTypeOrder;

    if(typeOrder == 1)
        nameTypeOrder = tr("Balcão");

    if(typeOrder == 2)
    {
        nameTypeOrder = tr("Mesa");
        nameTypeOrder += " " + numberTable;
    }

    if(typeOrder == 3)
        nameTypeOrder = tr("Entrega");


    html +=  "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
            "text-indent:0px;\"><span style=\" font-size:"+ H1 + "; color:#343434;\">" + tr("Pedido Nº ") + numberOrder + "</span></p>"
            "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
            "text-indent:0px;\"><span style=\" font-size:"+ H2 +"; color:#616161;\">" + nameTypeOrder + "</span></p>"
            "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px;"
            "margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:"+ H2 +"; color:#616161;\"></p>"
            "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;";

    /// Dados do cliente pessoa

    if(order->typeClient == 1)
    {

        clientHTML = "text-indent: 0px;\"><span style=\" font-size: " + H3 + "; font-weight: 600; color: #3d3d3d;\">" + tr("Nome") + "</span><span style=\""
                "font-size: " + H3 + "; font-weight: 600; color: #000000;\">: </span><span style=\" font-size: " + H3 + "; font-weight: 600; color: #616161;\">"
                "</span><span style=\" font-size: " + H3 + "; font-weight: 600; color: #757575;\">" + nameClient + "</span></p>"
                "<p style=\" margin-top: 0px; margin-bottom: 0px; margin-left: 0px; margin-right: 0px; -qt-block-indent: 0;"
                "text-indent: 0px;\"><span style=\" font-size: " + H3 + "; font-weight: 600; color: #3d3d3d;\">" + tr("Endereco") + "</span><span style=\" font-size: 10pt;"
                "font-weight: 600; color: #000000;\">: </span><span style=\" font-size: " + H3 + "; font-weight: 600; color: #616161;\">" + addressClient + "</span></p>";

        if(districtClient != "")
        {
            clientHTML += "<p style=\" margin-top: 0px; margin-bottom: 0px; margin-left: 0px; margin-right: 0px; -qt-block-indent: 0;"
                    "text-indent: 0px;\"><span style=\" font-size: " + H3 + "; font-weight: 600; color: #3d3d3d;\">" + tr("Bairro") + "</span><span style=\" font-size: " + H3 + ";"
                    "font-weight: 600; color: #000000;\">: </span><span style=\" font-size: " + H3 + "; font-weight: 600; color: #616161;\">" +  districtClient + "</span></p>";
        }

        clientHTML += "<p style=\" margin-top: 0px; margin-bottom: 0px; margin-left: 0px; margin-right: 0px; -qt-block-indent: 0;"
                "text-indent: 0px;\"><span style=\" font-size: " + H3 + "; font-weight: 600; color: #3d3d3d;\">" + tr("Telefone") + "</span><span style=\" font-size: " + H3 + ";"
                "font-weight: 600; color: #000000;\">: </span><span style=\" font-size: " + H3 + "; font-weight: 600; color: #616161;\">" +  phoneClient + "</span></p>";


        if(noteClient != "")
            clientHTML +="<p style=\" margin-top: 0px; margin-bottom: 0px; margin-left: 0px; margin-right: 0px; -qt-block-indent: 0;"
                    "text-indent: 0px;\"><span style=\" font-size: " + H3 + "; font-weight: 600; color: #3d3d3d;\">" + tr("Anotação") + "</span><span style=\" font-size: " + H3 + ";"
                    "font-weight: 600; color: #000000;\">: </span><span style=\" font-size: " + H3 + "; font-weight: 600; color: #616161;\">" +  noteClient + "</span></p>";

        clientHTML += "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px;"
                "margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:" + H5 + ";\">" + dateTime + "</span></p>"
                "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px;"
                "margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:" + H5 + ";\"></p>";

        if(nameClient != "")
            html += clientHTML;
    }
    else
    {
        clientBusinessHTML = "text-indent: 0px;\"><span style=\" font-size: " + H3 + "; font-weight: 600; color: #3d3d3d;\">" + tr("Nome") + "</span><span style=\""
                "font-size: " + H3 + "; font-weight: 600; color: #000000;\">: </span><span style=\" font-size: " + H3 + "; font-weight: 600; color: #616161;\">"
                "</span><span style=\" font-size: " + H3 + "; font-weight: 600; color: #757575;\">" + nameClientBusiness + "</span></p>"
                "<p style=\" margin-top: 0px; margin-bottom: 0px; margin-left: 0px; margin-right: 0px; -qt-block-indent: 0;"
                "text-indent: 0px;\"><span style=\" font-size: " + H3 + "; font-weight: 600; color: #3d3d3d;\">" + tr("Endereco") + "</span><span style=\" font-size: 10pt;"
                "font-weight: 600; color: #000000;\">: </span><span style=\" font-size: " + H3 + "; font-weight: 600; color: #616161;\">" + addressClientBusiness + "</span></p>";

        if(districtClientBusiness != "")
        {
            clientBusinessHTML +="<p style=\" margin-top: 0px; margin-bottom: 0px; margin-left: 0px; margin-right: 0px; -qt-block-indent: 0;"
                    "text-indent: 0px;\"><span style=\" font-size: " + H3 + "; font-weight: 600; color: #3d3d3d;\">" + tr("Bairro") + "</span><span style=\" font-size: " + H3 + ";"
                    "font-weight: 600; color: #000000;\">: </span><span style=\" font-size: " + H3 + "; font-weight: 600; color: #616161;\">" +  districtClientBusiness + "</span></p>";
        }

        clientBusinessHTML += "<p style=\" margin-top: 0px; margin-bottom: 0px; margin-left: 0px; margin-right: 0px; -qt-block-indent: 0;"
                "text-indent: 0px;\"><span style=\" font-size: " + H3 + "; font-weight: 600; color: #3d3d3d;\">" + tr("Telefone") + "</span><span style=\" font-size: " + H3 + ";"
                "font-weight: 600; color: #000000;\">: </span><span style=\" font-size: " + H3 + "; font-weight: 600; color: #616161;\">" +  phoneClientBusiness + "</span></p>"
                "<p style=\" margin-top: 0px; margin-bottom: 0px; margin-left: 0px; margin-right: 0px; -qt-block-indent: 0;";

        if(clientBusinessHTML != "")
            clientBusinessHTML += "text-indent: 0px;\"><span style=\" font-size: " + H3 + "; font-weight: 600; color: #3d3d3d;\">" + tr("Anotação") + "</span><span style=\" font-size: " + H3 + ";"
                    "font-weight: 600; color: #000000;\">: </span><span style=\" font-size: " + H3 + "; font-weight: 600; color: #616161;\">" +  noteClientBusiness + "</span></p>";

        if(nameClientBusiness != "")
            html += clientHTML;

    }

    /// Marcadores de uso

    html += "<p style=\"-qt-paragraph-type: empty; margin-top: 0px; margin-bottom: 0px; margin-left: 0px;"
            "margin-right: 0px; -qt-block-indent: 0; text-indent: 0px; font-size: " + H5 + "; font-weight: 600; color: #616161;\"></p>"
            "<p align=\"center\" style=\" margin-top: 0px; margin-bottom: 0px; margin-left: 0px; margin-right: 0px; -qt-block-indent: 0;"
            "text-indent: 0px;\"><span style=\" font-size: " + H5 + "; font-weight: 600; color: #6d6d6d;\">"
            "[" + tr("Qnt") + "] [" + tr("Produto") + "] [" + tr("Tamanho") + "] [" + tr("Valor") + "] [" + tr("Anotação") + "] </span></p>"
            "<p align=\"center\" style=\"-qt-paragraph-type: empty; margin-top: 0px; margin-bottom: 0px; margin-left: " + H5 + ";"
            "margin-right: 0px; -qt-block-indent: 0; text-indent: 0px; font-size: " + H5 + "; font-weight: 600; color: #6d6d6d;\"></p>"
            "<hr/>"
            "<p align=\"center\" style=\"-qt-paragraph-type: empty; margin-top: 0px; margin-bottom: 0px;"
            "margin-left: 0px; margin-right: 0px; -qt-block-indent: 0; text-indent: 0px; font-size: " + H5 + ";\"></p>";


    /// ------------- FOR DE ITENS

    listItens = "";
    for (int i = 0; i < order->orderQuantity.length(); i++){

        listItens += "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
                "text-indent:0px;\"><span style=\" font-size:" + H4 + "; font-weight:600; color:#6d6d6d;\">" + QString::number(order->orderQuantity.value(i)) + "-  </span>"
                "<span style=\" font-size:" + H4 + "; font-weight:600; color:#2f2f2f;\">";

        if (order->orderNameProduct.value(i).length() > 1)
            listItens += "Pizza mista";
        else
            listItens += order->orderNameProduct.value(i).value(0);

        listItens += " - " + order->orderSizes.value(i);

        listItens += "</span><span style=\" font-size:" + H4 + ";"
                "font-weight:600; color:#6d6d6d;\"> - " + QString::number(order->orderValues.value(i));

        if (! order->orderNoteProduct.value(i).isEmpty())
            listItens += "- </span><span style=\" font-size:" + H4 + ";"
                    "font-weight:600; color:#212121;\">" + order->orderNoteProduct.value(i) + "</span>";

        listItens += "</p>";
    }

    /// ------------- FIM DO FOR DE ITENS

    html += listItens;

    html += "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px;"
            "margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:" + H4 + "; font-weight:600; color:#6d6d6d;\"></p>"
            "<hr/>"
            "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px;"
            "margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:" + H4 + "; font-weight:600; color:#6d6d6d;\"></p>";

    /// Observações de Mesa



    if(_isTransfer)
    {
        html +="<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:"+H4+";\">Transferência de pagamento da mesa " + tableSubsidized + " para a mesa " + tableBuyer +"</span></p>"
                "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px;"
                "margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:" + H4 + "; font-weight:600; color:#6d6d6d;\"></p>";
    }

    if(_isMedia)
    {
        html += "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;"
                "-qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:"+ H4+ ";\">" + tr("Média da conta para ") + numberPeople + tr(" pessoas é de: ") + tools.convertMoney(valueMedia) + "</span></p>"
                "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px;"
                "margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:" + H4 + "; font-weight:600; color:#6d6d6d;\"></p>";
    }

    /// FOR Da Forma de pagamento

    bool hasPay = false;
    QString paymentForm = "";

    paymentForm += "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
            "text-indent:0px;\"><span style=\" font-size:" + H4 + "; font-weight:600; color:#737373;\">" + tr("Forma de pagamento") + "</span></p>";

    qDebug() << "Quantidade formas pagamento" << order->orderPaymentForm.length();
    for (int i = 0; i < order->orderPaymentForm.length(); i++){

        if (order->orderPaymentFormValue.value(i).toDouble() != 0){
            hasPay = true;
            paymentForm += "<p><span style=\" font-size:" + H4 + "; font-weight:600;\">  &#32; &#32; &#32; &#32;  " + order->orderPaymentForm.value(i) +    "</span><span style=\" font-size:" + H4 + ";\"> - " + tools.convertMoney(order->orderPaymentFormValue.value(i)) +"</span></p>";
            //                "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px;"
            //                "margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:" + H4 + ";\"></p>";
        }
    }

    /// FIM DO FOR Da Forma de pagamento


    if (hasPay)
        html += paymentForm;


    /// Agendamento

    bool isSchedule;

    if (order->dateSchedule == "")
        isSchedule = false;
    else
        isSchedule = true;

    if(isSchedule)
    {
        html += "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
                "text-indent:0px;\"><span style=\" font-size:"+ H3 + "; font-weight:600; color:#737373;\">" + tr("Agendado para: ") + "</span><span style=\" font-size:"+ H3 +";"
                "font-weight:600;\">" + order->dateSchedule + "</span><span style=\" font-size:"+ H3 +"; font-weight:600;"
                "color:#737373;\">" + tr(" às ") + "</span><span style=\" font-size:"+ H3 +"; font-weight:600;\">" + order->hourSchedule + "</span></p>";
    }

    /// Valores

    html += "<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
            "text-indent:0px;\"><span style=\" font-size:"+ H3 +"; font-weight:600; color:#848484;\">" + tr("Subtotal") + ".............................</span>"
            "<span style=\" font-size:"+ H3 +"; font-weight:600;\">" + Subtotal + "</span></p>";

    if(deliveryRate != "")
    {
        html += "<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
                "text-indent:0px;\"><span style=\" font-size:"+ H3 +"; font-weight:600; color:#787878;\">" + tr("Taxa de Entrega") + ".................</span>"
                "<span style=\" font-size:"+ H3 +"; font-weight:600;\">" + deliveryRate + "</span></p>";
    }

    html += "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
            "text-indent:0px;\"><span style=\" font-size:"+ H3 +"; font-weight:600; color:#838383;\">" + tr("Total") + "..................................</span>"
            "<span style=\" font-size:"+ H3 +"; font-weight:600;\">" + total + "</span></p>";

    if (hasPay)
        html += "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
                "text-indent:0px;\"><span style=\" font-size:"+ H3 +"; font-weight:600; color:#909090;\">" + tr("Pago") +" .................................. </span>"
                "<span style=\" font-size:"+ H3 +"; font-weight:600;\">" +  valuePaid + "</span></p>"
                "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
                "text-indent:0px;\"><span style=\" font-size:"+ H3 +"; font-weight:600; color:#8f8f8f;\">" + tr("Troco") +" ................................. </span>"
                "<span style=\" font-size:"+ H3 +"; font-weight:600;\">" + transshipment + "</span></p>"
                "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px;"
                "margin-right:0px; -qt-block-indent:0;\" text-indent:0px; font-size:"+ H3 +"; font-weight:600;\"></p>";

    if(isPaid)
    {
        html += "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
                "text-indent:0px;\"><span style=\" font-size:"+ H3 +"; font-weight:600; color:#8f8f8f;\">" + tr("Este pedido já foi pago") + "</span></p>";
    }

    html += "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0;"
            "text-indent:0px;\"><span style=\" font-size:"+ H3 +"; color:#1e1e1e;\">" + tr("*Este documento não possui nenhuma função fiscal") + "</span></p></body></html>"
            "<br>";

    return html;
}

void MakeHTML::setSizeFont(int size)
{
    QString temp;
    int s = 14;

    if(size == 3)
        s = 14;

    if(size == 2)
        s = 13;

    if(size == 1)
        s = 12;

    size = s;

    size -= 4;

    temp.setNum(size);

    H1 = temp + "pt";

    //    size -= 2;
    temp.setNum(size);

    H2 = temp + "pt";

    //    size -= 2;
    temp.setNum(size);

    H3 = temp + "pt";

    //    size -= 1;
    temp.setNum(size);

    H4 = temp + "pt";

    //    size -= 1;
    temp.setNum(size);

    H5 = temp + "pt";
}

void MakeHTML::print(QString html)
{
    loadNumberOfCopy();

    QTextEdit editor_;
    editor_.setHtml(html);

    for (int n = 0; n < numberOfCopy; ++n)
    {
        printer->print(editor_.document(),"Print");
    }
}

void MakeHTML::loadNumberOfCopy()
 {
    if (order->bd.search("select numberofcopies from configorder"))
    {
        if(order->bd.loadSimple(0).toInt() > 0)
            numberOfCopy = order->bd.loadSimple(0).toInt();
        else
            numberOfCopy = 1;
    }
    else
        numberOfCopy = 1;
}

void MakeHTML::createPDF(QString HTML)
{
    QTextEdit editor_;
    editor_.setHtml(html);
    printer->exportPdf(editor_.document(),"PDF");
}

void MakeHTML::reload()
{
    config->loadDataOrder();

    loadNumberOfCopy();

    setSizeFont(config->fontSize);

    createHTMLOrder(0);
}



