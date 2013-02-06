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


 #include "graphicsreport.h"
#include <QPainter>
#include <QDebug>

GraphicsReport::GraphicsReport(QWidget *parent) :
    QWidget(parent)
{
    useLegend(true);

    setType(Sectors);

    setMargins(10, 10);

    invalidDataText = "Dados insuficientes para construção do gráfico";
}

void GraphicsReport::setData(QList<float> data){
    QList<QList<float> > data_;
    data_ << data;
    setData(data_);
}

void GraphicsReport::setData(QList<int> data){
    QList<QList<float> > data_;

    QList<float> dataFloat;

    foreach(int d, data)
        dataFloat << (float) d;

    data_ << dataFloat;

    setData(data_);
}

void GraphicsReport::setData(QList<QList<float> > data){
    this->data = data;
    this->repaint();
}

void GraphicsReport::setColors(QList<QColor> colors){
    this->colors = colors;
}

void GraphicsReport::setBackgroundColor(int r, int g, int b, int a){
    backgroundColor.setRed(r);
    backgroundColor.setBlue(b);
    backgroundColor.setGreen(g);
    backgroundColor.setAlpha(a);
}

void GraphicsReport::setBackgroundColor(QColor backgroundColor){
    backgroundColor = backgroundColor;
}

void GraphicsReport::setLegend(QList<QString> legend){
    this->legend = legend;
}

void GraphicsReport::setType(GraphicsReport::types type){
    this->type = type;
}

void GraphicsReport::setMargins(int marginVertical, int marginHorizontal){
    this->marginHorizontal = marginHorizontal;
    this->marginVertical = marginVertical;
}

void GraphicsReport::generateColors(){
    if (data.length() == 0)
        return;

    QList<QColor> colors;
    QColor color;

    int variation; // variação do tom das cores

    int qnt;
    if (type == Sectors)
        qnt = data[0].length();
    else
        if (type == Lines)
            qnt = data[0].length();

    variation = (qnt > 6)? 51: 80;
    color = backgroundColor;
    color.setAlpha(255);
    for (int i = 0; i < qnt; i++){
        if (i % 3 == 0){
            color.setRed(generateColor(variation, color).red());
        }else{
            if (i % 3 == 1){
                color.setGreen(generateColor(variation, color).green());
            }else{
                color.setBlue(generateColor(variation, color).blue());
            }
        }
        colors << color;
    }
    setColors(colors);
}

QColor GraphicsReport::generateColor(int increment, QColor color){

    if (color == Qt::color0)
        color = backgroundColor;

    //color.setAlpha(color.alpha());
    color.setAlpha(255);

    if (color.red() + increment <= 255)
        color.setRed(color.red() + increment);
    else
        color.setRed((color.red() + increment) - 255);

    if (color.green() + increment <= 255)
        color.setGreen(color.green() + increment);
    else
        color.setGreen((color.green() + increment) - 255);

    if (color.blue() + increment <= 255)
        color.setBlue(color.blue() + increment);
    else
        color.setBlue((color.blue() + increment) - 255);

    return color;
}

void GraphicsReport::paintEvent(QPaintEvent *){
    drawBackground();
    generateColors();

    switch(type){
    case Bars:
        drawBars();
        break;
    case Lines:
        drawLines();
        break;
    case Sectors:
        drawSectors();
        break;
    }
}

void GraphicsReport::drawSectors(){
    if (data.length() == 0){
        drawInvalidData();
        return;
    }

    if(data[0].length() == 0){
        drawInvalidData();
        return;
    }

    if (isLegend)
        drawLegend();

    QPainter p(this);

    // calcula a soma dos valores para calculo dos angulos
    float sum = 0;

    foreach (float i, data[0])
        sum += i;

    QList<float> angles; // angulonos dos setores do grafico

    foreach (float i, data[0])
        angles << i/sum * 360;

    float beforeAngle = 0;

    float angle = 0;

    int radius;
    if (isLegend)
        radius = (width() - WidthLegend > height())? height()- 2*marginVertical: width()- 2*marginHorizontal - WidthLegend; // tira vinte para repor depois, dando 10px de margem
    else
        radius = (width() > height())? height()- 2*marginVertical: width()- 2*marginHorizontal; // tira vinte para repor depois, dando 10px de margem

    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);

    int index = 0; // para percorrer cores e afins

    // variaveis usadas no foreach
    int startAngle;
    int spanAngle;

    foreach (float a, angles){
        p.setBrush(QBrush::QBrush(colors[index]));

        QRectF rect(marginHorizontal, marginVertical, radius, radius);

        angle += a;

        startAngle = beforeAngle * 16;

        spanAngle = a * 16;

        beforeAngle = angle;

        p.drawPie(rect, startAngle, spanAngle);

        index ++;
    }
}

void GraphicsReport::drawLines(){
    if (data.length() == 0){
        drawInvalidData();
        return;
    }

    drawLegend();

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QList<QList<float > > data = adjustValues();

    int distance = (int) (width() - WidthLegend - 20) / (data[0].length() - 1); // -20 (10 px de cada lado de margem)
    int px = 10; // posição x do desenho do grafico (10 px de margem)

    QPen pen;

    for (int i = 0; i < data.length(); i++){
        pen.setColor(colors[i]);
        px = 10; // zera a margem
        for (int j = 0; j < data[i].length() - 1; j++){
            pen.setWidth(2);

            p.setPen(pen);
            p.drawLine(px, invertY(marginVertical + data[i][j]), px + distance, invertY(marginVertical + data[i][j+1]));

            //desenhar o ponto para marcar os pontos do grafico
            pen.setWidth(5);
            p.setPen(pen);

            QPoint point;
            point.setX(px);
            point.setY(invertY(marginVertical + data[i][j]));
            p.drawPoint(point);

            point.setX(px + distance);
            point.setY(invertY(marginVertical + data[i][j+1]));
            p.drawPoint(point);

            px += distance;
        }
    }
}

void GraphicsReport::drawBars(){

}

void GraphicsReport::drawLegend(){
    QPainter p(this);

    QPen pen;

    pen.setColor(generateColor(150));

    p.setPen(pen);

    int y = marginVertical + 10; // a margem mais 10 de distancia (altura da linha)
    int x = width() - WidthLegend;

    QFont font;
    font.setBold(true);
    font.setPointSize(10);
    p.setFont(font);

    p.drawText(x, y, "Legenda");

    font.setBold(false);

    p.setFont(font);

    y += 20; // pula vinte pixels entre cada "linha"

    if (type == GraphicsReport::Sectors)
        for (int i = 0; i < legend.length(); i++){
            pen.setColor(colors[i]);
            pen.setWidth(2);

            p.setPen(pen);
            p.drawLine(x,y,x+10,y);
            p.drawText(x + 10, y, " " + legend[i]);

            y+=20;
        }
    else{
        for (int i = 0; i < data.length(); i++){
            pen.setColor(colors[i]);
            pen.setWidth(2);

            p.setPen(pen);
            p.drawLine(x,y,x+10,y);
            p.drawText(x + 10, y, " " + legend[i]);

            y+=20;
        }
    }
}

void GraphicsReport::drawInvalidData(){
    QPainter p(this);

    QPen pen;

    QColor color;

    pen.setColor(generateColor(150));

    p.setPen(pen);
    p.drawText(10, 10, width(), height() ,0, invalidDataText);
}

void GraphicsReport::drawBackground(){
    QPainter p(this);

    QBrush b;

    b.setColor(backgroundColor);
    b.setStyle(Qt::SolidPattern);
    p.setBrush(b);
    p.setPen(Qt::NoPen);

    p.drawRect(QRect(0, 0, width(), height()));
}

float GraphicsReport::calcPercentage(){
    float max = data[0][0];

    for (int i = 0; i < data.length(); i++){
        foreach (float value, data[i])
            max = (value > max)? value: max;
    }

    return (100 * (this->height() - 2*marginVertical)) / max; // gera a porcentagem e a retorna
    // duas vezes a margem para contar cima e baixo
}

QList<QList<float> > GraphicsReport::adjustValues(){
    QList<QList<float> > data;

    data = this->data;

    float percentage = calcPercentage();

    for (int i = 0; i < data.length(); i++)
        for (int j = 0; j < data[i].length(); j++)
            data[i][j] *= percentage / 100;

    return data;
}

int GraphicsReport::invertY(int py){
    return height() - py;
}

void GraphicsReport::mousePressEvent(QMouseEvent *)
{
//    type = (type == Lines)? Sectors: Lines;
    //    repaint();
}

void GraphicsReport::useLegend(bool use)
{
    this->isLegend = use;
}

void GraphicsReport::setInvalidDataText(QString invalidDataText)
{
    this->invalidDataText = invalidDataText;
}

QList<QList<float> > GraphicsReport::getData()
{
    return this->data;
}

QList<float> GraphicsReport::getDataSimple()
{
    return this->data[0];
}

void GraphicsReport::clearData()
{
    this->data.clear();
}
