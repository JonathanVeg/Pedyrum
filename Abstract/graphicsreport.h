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


 #ifndef GRAPHICSREPORT_H
#define GRAPHICSREPORT_H

#include <QWidget>

const int WidthLegend = 100;

class GraphicsReport : public QWidget
{
    Q_OBJECT
public:
    explicit GraphicsReport(QWidget *parent = 0);

public:
    enum types{
        Sectors,
        Bars,
        Lines
    };


public: // funções
    void setData(QList<float> data); // para setores
    void setData(QList<int> data); // para setores
    void setData(QList<QList<float> > data); // para barras e linhas

    QList<QList<float> > getData();
    QList<float> getDataSimple(); // retorna apenas a posição 0 (grafico de setores)

    void setColors(QList<QColor> colors);

    void setBackgroundColor(QColor backgroundColor);
    void setBackgroundColor(int r, int g, int b, int a = 0); // alfa verificarei

    void setLegend(QList<QString> legend);

    void setType(GraphicsReport::types type);

    void setMargins(int marginVertical, int marginHorizontal);

    void generateColors(); // usar depois do setData, pq ela gerara as cores necessarias para ele

    QColor generateColor(int increment, QColor color = Qt::color0); // gera uma cor a partir
    // do background e incrementa do valor passado como parametro

    void useLegend(bool use);

    void setInvalidDataText(QString invalidDataText);

    void clearData();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

    // desenha os graficos
    void drawSectors();
    void drawLines();
    void drawBars();

    void drawLegend(); // pinta a legenda

    // escreve que os dados são isuficientes para desenhar o grafico
    void drawInvalidData();

    void drawBackground();

    QList<QList<float> > adjustValues(); // ajusta os valores para caberem no espaço

    float calcPercentage(); // calcula a porcentagem para ajuste do grafico

    int invertY(int py);


protected: // variaveis
    QList<QList<float> > data;

    QList<QColor> colors;

    QColor backgroundColor;

    // tipo de grafico a ser mostrado
    GraphicsReport::types type;

    // legenda
    QList<QString> legend;

    // margens
    int marginVertical;
    int marginHorizontal;

    bool isLegend;

    QString invalidDataText;

signals:

public slots:

};

#endif // GRAPHICSREPORT_H
