/*
Copyright (c) 2013, Luiz C�ndido da Silva J�nior, heron@jaguati.com.br

Copyright (c) 2013, Jonathan,  jonathan@jaguati.com.br
 All rights reserved. 

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 

 Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 

 Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. 

 Neither the name of Pedyrum may be used to endorse or promote products derived from this software without specific prior written permission. 

 THIS SOFTWARE IS PROVIDED BY {{THE COPYRIGHT HOLDERS AND CONTRIBUTORS}} "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL {{THE COPYRIGHT HOLDER OR CONTRIBUTORS}} BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


 #ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <QObject>

class Encryption : public QObject
{
    Q_OBJECT
public:
    explicit Encryption(QObject *parent = 0);

    QString encrypt(QString text);

    QString decrypt(QString text);

    QString encryptWithoutBack(QString text); // criptografia para coisas como senha, para evitar que tenha volta e peguem a senha

    int letterToAscii(QString letter);

    QString asciiToLetter(int ascii);

signals:

public slots:

};

#endif // ENCRYPTION_H