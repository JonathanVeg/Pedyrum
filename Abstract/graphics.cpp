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


 #include <Abstract/graphics.h>
#include <QPainter>
#include <QDebug>

Graphics::Graphics(QWidget *parent) :
    QWidget(parent)
{

    QList<QColor> colors;

    QColor color;

    color.setRgb(0,180,0);
    colors << color;
    color.setRgb(0,0,180);
    colors << color;
    color.setRgb(100,100,100);
    colors << color;
    color.setRgb(0,255,0);
    colors << color;
    color.setRgb(255,255,0);
    colors << color;
    color.setRgb(255,0,255);
    colors << color;
    color.setRgb(255,150,0);
    colors << color;

    setColors(colors);

    setMargins(50,150,30,60);

    QList<float> data;

    data << 100 << 300 << 200 << 300 << 900;

    setData(data);

    QList<QString> legenda;

    legenda << "Cor 1" << "Cor 2" << "Cor 3" << "Cor 4" << "Cor 5";

    setLegend(legenda);

    setType(Graphics::Sectors);
    maxLine = 0;
}

void Graphics::paintEvent(QPaintEvent *)
{
    drawBackground();

    if (data.length() > 0){
        if (type == Graphics::Lines){
            drawLines();
            drawMask();
        }else
            if (type == Graphics::Sectors)
                drawSectors();
        drawLegend();
    }
}

void Graphics::drawLegend()
{
    QPainter p(this);

    QPen pen;

    pen.setColor(Qt::black);

    int y = top;

    int x = width() - right + 20;

    QFont font;
    font.setBold(true);
    font.setPointSize(10);
    p.setFont(font);

    QString text = "Legenda";

    p.drawText(x, y, text);

    font.setBold(false);

    p.setFont(font);

    y += 20;

    if (type == Graphics::Sectors)
        for (int i = 0; i < legend.length(); i++){
            pen.setColor(colors[i]);
            pen.setWidth(2);

            p.setPen(pen);

            p.drawLine(x,y,x+10,y);

            x+=10;

            p.drawText(x,y, " " + legend[i]);

            x-=10;

            y+=20;
        }
    else{
        for (int i = 0; i < data.length(); i++){
            pen.setColor(colors[i]);
            pen.setWidth(2);

            p.setPen(pen);

            p.drawLine(x,y,x+10,y);

            x+=10;

            p.drawText(x,y, " " + legend[i]);

            x-=10;

            y+=20;
        }
    }
}

void Graphics::drawSectors()
{
    QPainter p(this);

    float total = 0;

    // calculas a soma de todos os valores a serem representados
    QList < float > angles;
    for (int i = 0; i < data[0].length(); i++){
        total += data[0][i];
    }

    int space;

    space = 7 * 7; // espaço ocupado pela legenda
    for (int i = 0; i < legend.length(); i ++){
        if (legend[i].length() * 7 > space){
            space = legend[i].length() * 7;
        }
    }

    space += 10;

    // calcula os angulos a serem usados
    for (int i = 0; i < data[0].length(); i++){
        angles.append(data[0][i] / total * 360);
    }

    float beforeAngle = 0;
    float angle = 0;

    int radius;

    if ((width() - space) <= height()){
        radius = width() - space - 30;
    }else{
        radius = height() - 30;
    }
    right = space + 30;

    for (int i = 0; i < angles.length(); i++){
        p.setRenderHint(QPainter::Antialiasing, true);
        //p.setPen(QPen(Qt::black));
        p.setPen(QPen(Qt::NoPen)); // não mostrará linhas demarcadoras
        p.setBrush(QBrush::QBrush(colors[i]));

        QRectF rectangle(15,15, radius, radius);
        angle += angles[i];
        int startAngle = beforeAngle * 16;
        int spanAngle = angles[i] * 16;
        //int spanAngle = angle * 16;
        beforeAngle = angle;

        p.drawPie(rectangle, startAngle, spanAngle);
    }
}

void Graphics::setData(QList<QList<float> > data)
{
    this->data = data;
}

void Graphics::setData(QList<float> data){
    QList<QList<float> > data_;
    data_ << data;

    setData(data_);
}

void Graphics::setColors(QList<QColor> colors)
{
    this->colors = colors;
}

bool Graphics::drawLines()
{
    QPainter p(this);

    p.setRenderHint(QPainter::Antialiasing);

    if (data.length() == 0)
        return false;

    QList < QList < float > > data = adjustValues();

    int pX = 0;
    int pY = 0;

    int colunas = data[0].length();
    int linhas = data.length();

    int distance = (int) (width() - left - right * 2) / (colunas - 1);

    for (int i = 0; i < linhas; i++){
        QPen pen;

        pX = left + 5;
        pY = button;

        for (int j = 0; j < colunas - 1; j++){
            pen.setColor(colors[i]);
            pen.setWidth(2);
            p.setPen(pen);

            if (py(pY + data[i][j]) > maxLine)
                maxLine = py(pY + data[i][j]);

            if (py(pY + data[i][j+1]) > maxLine)
                maxLine = py(pY + data[i][j+1]);

            p.drawLine(pX, py(pY + data[i][j]), pX+distance, py(pY + data[i][j+1]));

            pen.setColor(colors[i]);
            pen.setWidth(5);
            p.setPen(pen);

            QPoint point;
            point.setX(pX);
            point.setY(py(pY + data[i][j]));
            p.drawPoint(point);
            point.setX(pX + distance);
            point.setY(py(pY + data[i][j+1]));
            p.drawPoint(point);

            pen.setColor(Qt::black);
            pen.setWidth(3);

            p.setPen(pen);

            p.drawLine(pX, py(button - 2), pX, py(button - 7)); // linhas demarcadoras

            pX += distance;
        }
        pen.setColor(Qt::black);
        pen.setWidth(3);

        p.setPen(pen);

        p.drawLine(pX, py(button - 2), pX, py(button - 7)); // linhas demarcadoras
    }

    return true;
}

void Graphics::drawMask()
{
    QPainter p(this);
    QPen pen;
    pen.setWidth(3);
    pen.setColor(Qt::black);

    p.setPen(pen);

    p.drawLine(left, py(button), left, py(height() - top)); // linha vertical
    p.drawLine(left, py(button), width() - right, py(button)); // linha horizontal

    float maxValueReal = getMax(data);
    float maxValueAdjust = getMax(adjustValues());
    int tamVerticalLine = py(button) - py(height() - top);

    int i = 1;
    while (tamVerticalLine % 10 != 0){
        tamVerticalLine = py(button) - (py(height() - top)-i);
        i++;
    }
    p.drawLine(left, py(button), left, (py(height() - top)-i-5)); // linha vertical
    tamVerticalLine += 5;

    int distancia = (tamVerticalLine - 5) / 10;

    float jump = tamVerticalLine/maxValueAdjust;

    maxValueReal *= jump;

    jump = maxValueReal / 10;

    QString valueStr;
    float valueInt = 0;

    pen.setWidth(1);
    p.setPen(pen);
    p.drawText(left - 20, py(button) + 5,"0");
    for (int i = 1; i <= 10; i++){
        p.drawLine(left, py(button) - i*distancia, width() - right, py(button) - i * distancia);
        valueInt += jump;
        valueStr.setNum(valueInt);
        int index = 0;
        int pos = 0;
        foreach (QString v, valueStr){
            if (v == ".")
                pos = index;
            index++;
        }
        valueStr = valueStr.mid(0,pos);
        p.drawText(left - 20, py(button) - i*distancia + 5, valueStr);
    }
}

QList <QList <float> > Graphics::adjustValues(float percentage){
    QList <QList <float> > retorno;

    retorno = data;

    for (int i = 0; i < data.length(); i++){
        for (int j = 0; j < data[0].length(); j++){
            retorno[i][j] = retorno[i][j] * percentage / 100;
        }
    }

    return retorno;
}

QList<QList<float> > Graphics::adjustValues()
{
    QList < QList < float > > retorno;

    retorno = data;

    for (int i = 0; i < data.length(); i++){
        for (int j = 0; j < data[0].length(); j++){
            retorno[i][j] = data[i][j] * getPercentage() / 100;
        }
    }

    return retorno;
}

float Graphics::getPercentage()
{
    float max = getMax(data);

    return (100 * (this->height() -  top - button - 20)) / max;
}

float Graphics::getMax(QList<QList<float> > data)
{
    float max = -20000;

    for (int i = 0; i < data.length(); i++){
        for (int j = 0; j < data[0].length(); j++){
            if (data[i][j] > max){
                max = data[i][j];
            }
        }
    }

    return max;
}

void Graphics::setBackgroundColor(int r, int g, int b)
{
    backgroundColor.r = r;
    backgroundColor.g = g;
    backgroundColor.b = b;
}

void Graphics::setBackgroundColor(QColor color)
{
    backgroundColor.r = color.red();
    backgroundColor.g = color.green();
    backgroundColor.b = color.blue();
}

void Graphics::setMargins(int left, int right, int top, int button)
{
    this->left = left;
    this->right = right;
    this->top = top;
    this->button = button;
}

int Graphics::py(int pY)
{
    return height() - pY;
}

void Graphics::drawBackground()
{
    QPainter p(this);

    QBrush b;

    b.setColor(Qt::white);

    b.setStyle(Qt::SolidPattern);

    p.setBrush(b);

    p.setPen(Qt::NoPen);

    p.drawRect(0,0,width(),height()-5);
}

void Graphics::setLegend(QList<QString> legend)
{
    this->legend = legend;
}

void Graphics::setType(Graphics::types)
{
    this->type = type;
}

QList<QList<float> > Graphics::getData()
{
    return data;
}
