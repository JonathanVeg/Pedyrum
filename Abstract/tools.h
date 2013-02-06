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


 #ifndef TOOLS_H
#define TOOLS_H

#include <QObject>
#include <QDebug>

class Tools : public QObject
{
    Q_OBJECT
public:
    explicit Tools(QObject *parent = 0);

    QString currentDay();
    QString currentMonth();
    QString currentMonth2();
    QString currentYear();
    QString currentTime();
    QString secondsToTime(int segunds, bool isLong = false);
    QString upFirstLetter(QString text);

    bool createTextFile(QString fileName);

    QString readFile(QString fileName);

    bool writeFile(QString fileName, QString text);

    /// pensar em um nome melhor
    QDateTime subDate(QString date, QString hour, int minutes); // pega a data e hora e diminui o número de dias.

    QString month(int month);

    QString getDate(Qt::DateFormat format = Qt::SystemLocaleShortDate);

    int dayOfWeek(QString dateS); // pega o dia da semana de determinada data

    QString getDayBeginWeek(QString dateS); // pega o primeiro dia da semana da data especificada

    QString getDayEndWeek(QString dateS); // pega o último dia da semana da data especificada

    QString daysInMonth(QString dateS);
    QString daysInYear(QString dateS);

    bool betweenDates(QString dateI, QString dateF, QString dateC); // ve se a date se esta entre as outras datas, inclusive

    QString getTime(QString format);

    QString convertDate(QString date, QString separator = "/"); // converte de ddmmyyyy para yyyymmdd
    QString convertDate2(QString date, QString separator = "/"); // reverte a conversão acima - tenho que ver um nome melhor para isso

    QChar compareDate(QString date0, QString date1); // retorna o numero da data maior

    QChar compareHour(QString hour0, QString hour1); // retorna o numero da data maior

    QString getIndexState(int index);

    // funções para trabalhar com horas
    bool betweenHours(QString hourI, QString hourF, QString hourC);
    unsigned int hourNumber(QString hour);

    unsigned int dateNumber(QString date);

    // recebe um valor e converte em formato monetário
    QString convertMoney(QString value, QString separator = ",");

    // função que matemática que pode ser útil em futuras criptografias
    int modulo(int a, int e, int n); // retorna (a^e)%n

    bool isValidDate(QString dateS);

    bool isValidHour(QString hourS);

    // validações
    bool validateCNPJ(QString cnpj);
    bool validateCPF(QString cpf);

signals:

public slots:

};

#endif // TOOLS_H
