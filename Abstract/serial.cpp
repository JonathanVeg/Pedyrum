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


 #include "serial.h"
#include <QDebug>
#include <QTime>

Serial::Serial(QObject *parent) :
    QObject(parent)
{
}

QString Serial::generateRandomCode(int lengthCode)
{
    QTime midnight(0, 0, 0);
    int seed = midnight.secsTo(QTime::currentTime());

    QString randomCode;

    QString random;

    double randNumber;

    srand(seed);
    while (randomCode.length() < lengthCode){

        randNumber = rand();

        random.setNum(randNumber);

        random = random.mid(random.length() - 1, 1);

        if (random != "0")
            randomCode += random;
    }

    return randomCode;
}

QString Serial::generateSerial(QString randomCode){
    QString rc = randomCode;

    if (! rc.length() == 15)
        return "Random code invalid";

    int b1 = rc.mid(0, 5).toInt();
    int b2 = rc.mid(5, 5).toInt();
    int b3 = rc.mid(10, 5).toInt();

    QString b1s;
    QString b2s;
    QString b3s;

    // essa parte é para descobrir o "Fator multiplicador" aplicando-os nos blocos

    if (rc.mid(0,1).toInt() > rc.mid(13,1).toInt()){

        b1 *= rc.mid(0,1).toInt() - rc.mid(13,1).toInt();
        b2 *= rc.mid(0,1).toInt() - rc.mid(13,1).toInt();
        b3 *= rc.mid(0,1).toInt() - rc.mid(13,1).toInt();

    }else if (rc.mid(13,1).toInt() > rc.mid(0,1).toInt()){

        b1 *= rc.mid(13,1).toInt() - rc.mid(0,1).toInt();
        b2 *= rc.mid(13,1).toInt() - rc.mid(0,1).toInt();
        b3 *= rc.mid(13,1).toInt() - rc.mid(0,1).toInt();

    }else{

        b1 *= 2;
        b2 *= 2;
        b3 *= 2;

    }

    // aqui ele cria a versão strign dos blocos e, se tiver mais de 5 caracteres, ele os corta, deixando apenas os 5 primeiros

    b1s = QString::number(b1);
    b2s = QString::number(b2);
    b3s = QString::number(b3);

    b1s = (b1s.length() > 5)? b1s.mid(0,5): b1s;
    b2s = (b2s.length() > 5)? b2s.mid(0,5): b2s;
    b3s = (b3s.length() > 5)? b3s.mid(0,5): b3s;

    // agora ele volta-os para numero para ordenação
    b1 = b1s.toInt();
    b2 = b2s.toInt();
    b3 = b3s.toInt();

    // ordenando os numeros
    int b4; // variavel auxiliar.

    if (b1 > b2 && b3 > b2){
        b4 = b1;
        b1 = b2;
        b2 = b4;

    }else if (b1 > b3 && b2 > b3){
        b4 = b1;
        b1 = b3;
        b3 = b4;

    }

    if (b3 < b2){
        b4 = b2;
        b2 = b3;
        b3 = b4;
    }

    // devolve para numero e cria o serial intercalando os numeros, em ordem descrescente

    b1s = QString::number(b1);
    b2s = QString::number(b2);
    b3s = QString::number(b3);

    QString serial = "";

    for (int i = 0; i <= 5; i++){
        serial += b3s.mid(i, 1);
        serial += b2s.mid(i, 1);
        serial += b1s.mid(i, 1);
    }

    return serial;
}

QString Serial::generateSerialDate(QString date, QString randomCode)
{
    QString pos = ""; // posições alteradas

    int qnt = 0;

    QString d = ""; // data auxiliar

    int loop = date.length();

    for (int i = 0; i < loop; i++){
        if (date.mid(i, 1) == "0"){
            qnt ++;

            pos += QString::number(i);

            if (i == 0){
                d = QString::number(qnt);

                d += date.mid(1);
            }else if (i == date.length() - 1){
                d = date.mid(0, date.length() - 1);
                d += QString::number(qnt);
            }else{
                d = date.mid(0, i);
                d += QString::number(qnt);
                d += date.mid((i+1));
            }

            date = d;
        }
    }

    QString serial = "";

    for (int i = 0; i < date.length(); i++){
        serial += randomCode.mid(i,1);
        serial += date.mid(i,1);
    }

    // aqui os codigos ja estão misturados

    QString s = ""; // serial alterado

    int j = 0;

    for (int i = 0; i < pos.length(); i ++){
        j = i * 2;

        if (i == 0){
            s = pos.mid(0, 1);

            s += serial.mid(1);
        }else{
            s = serial.mid(0, j);
            s += pos.mid(i, 1);
            s += serial.mid((j+1));
        }


        serial = s;
    }

    s = serial.mid(0, serial.length() - 2);
    s += QString::number(pos.length());
    s += serial.mid(serial.length() - 1);

    serial = s;

    return serial;
}

QString Serial::decryptSerialDate(QString serial)
{
    //  primeiro passo, separar a data do codigo

    QString date = "";
    QString code = "";

    for (int i = 0; i < serial.length(); i++){
        if (i % 2 == 0)
            code += serial.mid(i, 1);
        else
            date += serial.mid(i, 1);
    }

    int qnt = code.mid(code.length() - 1, 1).toInt();

    for (int i = 0; i < qnt; i++){

        int pos = code.mid(i, 1).toInt(); // posição que tem que ser alterada

        QString d; // data auxiliar

        if (pos == 0){
            d = QString::number(0);

            d += date.mid(1);
        }else if (i == date.length() - 1){
            d = date.mid(0, date.length() - 1);
            d += QString::number(0);
        }else{
            d = date.mid(0, pos);
            d += QString::number(0);
            d += date.mid((pos+1));
        }

        date = d;
    }

    return date;
}
