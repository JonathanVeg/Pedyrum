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


 #include "encryption.h"
#include <QDebug>

Encryption::Encryption(QObject *parent) :
    QObject(parent)
{
}

QString Encryption::encrypt(QString text){
    QString textReturn = "";
    foreach(QString letter, text){
        textReturn += QString::number(QString::number(letterToAscii(letter)).length());
        textReturn += QString::number(letterToAscii(letter));
    }

    return textReturn;
}

QString Encryption::decrypt(QString text){
    QString textReturn = "";
    int i = 0;

    QString ascii;
    int qnt;
    while (i < text.length()){
        qnt = text.mid(i, 1).toInt();
        ascii = text.mid(i+1, qnt);
        textReturn += asciiToLetter(ascii.toInt());
        i += qnt + 1;
    }

    return textReturn;
}

QString Encryption::encryptWithoutBack(QString text){
    QString textReturn = "";
    int count = 0;
    foreach(QString letter, text){
        if (count % 2 == 0)
            for (int i = 0; i < QString::number(letterToAscii(letter)).length(); i++)
                textReturn += QString::number(letterToAscii(letter))[QString::number(letterToAscii(letter)).length() - 1 - i];
        else
            textReturn += QString::number(letterToAscii(letter));
    }

    return textReturn;
}

int Encryption::letterToAscii(QString letter){
    QChar c = letter.at(0).toAscii();
    return (int)c.toAscii();
}

QString Encryption::asciiToLetter(int ascii){
    char asciiChar = (char)ascii;
    return QString::fromAscii(&asciiChar).mid(0,1);
}
