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


 #ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QWidget>

class Graphics : public QWidget
{
    Q_OBJECT
public:
    explicit Graphics(QWidget *parent = 0);

private:
    enum types{
        Sectors,
        Bars,
        Lines
    };

public: // funções
    void setData(QList<float> data);
    void setData(QList < QList< float > > data);
    void setColors(QList < QColor > colors);
    void setBackgroundColor(int r, int g, int b);
    void setBackgroundColor(QColor color);
    void setMargins(int left, int right, int top, int button);
    void setLegend(QList<QString> legend);
    void setType(Graphics::types type); // pode ser "lines" ou "sectors" ou, futuramente, "bars"

    QList<QList<float > > getData();


protected: // funções
    void paintEvent(QPaintEvent *); // função padrão sempre chamada
    void drawSectors(); // desenha o grafico de setores
    void drawLegend(); // pinta a legenda no gráfico
    void drawMask(); // desenha as mascaras do grafico
    void drawBackground(); // pinta o fundo do widget


    bool drawLines(); // o de linhas

    int py(int pY); // função para inverter os valores

    float getPercentage();
    float getMax(QList<QList<float> > data);

    QList <QList <float> > adjustValues(); // ajusta os valores para desenhar o grafico
    QList <QList <float> > adjustValues(float porcentagem);

protected: // variaveis
    QList < QList< float > > data;
    QList < QColor > colors;

    //variaveis de margem
    int left;
    int right;
    int top;
    int button;

    // variaveis de desenho do fundo do gráfico
    bool verticalLines;
    bool horizontalLines;

    // tipo do gráfico a ser exibido
    Graphics::types type;

    // variavel que terá o valor da linha mais alta
    int maxLine;

    // variaveis da cor do fundo do widget
    struct{
        int r,g,b;
    }backgroundColor;

    // variavel de legenda do gráfico
    QList<QString> legend;


signals:

public slots:

};

#endif // GRAPHICS_H
