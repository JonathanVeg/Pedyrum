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


 #ifndef TOPMENU_H
#define TOPMENU_H

#include <QWidget>
#include <Component/label.h>
#include <QResizeEvent>
#include <QPropertyAnimation>

const char url_MenuSuperior[50] = ":/topMenu/background";
const int const_Height = 28;

class topMenu : public QWidget
{
    Q_OBJECT
public:
    explicit topMenu(QWidget *parent = 0);

    void createComponent();  // Cria os componentes.

    void setComponent();     // Configura os componentes.

    void resizeEvent(QResizeEvent *); // evento ativado quando a mudanças
                                      // no tamanho do Widget.

    QString firstName(QString name); // ajusta o apelido para ficar apresentável
                                     // no menu superior.

    int sizeNickName(QString name); // retorna o tamanho do nickname

    void setNickName(QString text);

    void ajustLayout(); // ajusta a interface

signals:
   void sideVisible(bool b);
   void FullScreen(bool b);
   void userClick();


public slots:

    // Ajusta a largura da barra superior
    void goResize(int width_, int height_);    
    void onFocus_menuButton(bool b);    
    void onClick_fullScreen();

    void onClick_nickName();


public:

    QLabel *background;
    Label *fullScreen;
    Label *menuButton;
    Label *trayIco;
    Label *nickName;

    bool isOnMenu;

    bool visible;
    bool activateFocus;
    QString nickNameText;

};

#endif // TOPMENU_H
