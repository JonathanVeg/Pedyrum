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


 #ifndef PRODUCTINTERFACE_H
#define PRODUCTINTERFACE_H

#include <QWidget>
#include <Component/label.h>
#include <Component/edit.h>
#include <Component/tableview.h>
#include <QTextBrowser>
#include <QStackedWidget>
#include <Component/doublespinbox.h>
#include <QTextEdit>
#include <QPushButton>
#include <QSplitter>
#include <Abstract/graphiceffects.h>
#include <Abstract/datamodel.h>
#include <Component/search.h>
#include <Component/combobox.h>
#include <QPropertyAnimation>
#include <Object/product.h>
#include <QAction>
#include <QKeyEvent>

class ProductInterface : public QWidget
{
    Q_OBJECT
public:
    explicit ProductInterface(QWidget *parent = 0);

    void createComponent(); // Cria os componentes na tela.

    void connections();     // cria as coneções.
    void hidePanel(bool b);
    void statics();         // alinha o layout estático.

    QString createPreview();

    void updateCbCategory();

    int textCountProductSize();

private:
    void resizeEvent(QResizeEvent *);
    bool isSelectEmpty();
    bool isAvailable();

    // add os novos preços a tabela de valores
    void addDataValue();

signals:
    void goToPanel(int);

    void fullScreen();

private slots:
    void onClick_lAdd();
    void onClick_lRemove();
    void onClick_lProfile();
    void onClick_pbConfirm();
    void onClick_pbLeave();
    void onClick_addValue();
    void onClick_removeValue();
    void getSearchProduct(QString text);
    void textChanged_eCategory(QString text);
    void textChanged_eSize(QString text);

    void onDoubleClick_tbProduct(QModelIndex index);
    void onClick_tbProduct(QModelIndex index);
    void rowChanged_tbProduct(int row);

    void adjustSplitter();

    void updateCountProducts(QString qnt);
    void updateCbSubCategory(QString nameCategory);
    void filterSubCategory(QString nameSection);

public slots:
    void logout();

    void onClick_tbValue(QModelIndex);
    void adjustLayout();
    void gotoPanel(int index);
    void nextPanel();
    void input();

    void keyPressEvent(QKeyEvent *event);

    void onTriggered_actionName();

    void onTriggered_actionNickName();


public:
    enum typeSearch{
        NAME = 0,
        NICKNAME = 1
    };

    typeSearch typeS;

    int idProductUpdate;

    short int nextPanelIndex;

    QLabel *lTop;

    Label *lAdd;
    Label *lRemove;
    Label *lProfile;
    QTextBrowser *textCountProduct;
    ComboBox * cbCategory;
    ComboBox * cbSection;
    Search *searchProduct;

    QWidget *wSpace;
    QLabel *lBackgroundSpace;

    QStackedWidget *swPanels;

    TableView *tableProduct;
    QSplitter *splitter;

    QWidget *wGroupPlus;
    QLabel *lBackgroundPlus;

    Edit *eName;
    Edit *eNickname;
    Edit *eCategory;
    Edit *eSection;
    Edit *eSize;

    DoubleSpinBox *DSBValue;
    QPushButton *pbAddValue;
    QPushButton *pbRemoveValue;
    TableView *tableValues;

    QLabel *lTextNotes;
    QTextEdit  *teNotes;

    QPushButton *pbConfirm;
    QPushButton *pbLeave;

    QWidget *wGroupProfile;
    QLabel *lBackgroundProfile;
    QTextBrowser *textPreview;
    
    GraphicEffects *panelEffects;

    QPropertyAnimation  *  animaUp;
    QPropertyAnimation  *  animaDown;

    Product *product;

    QLabel *lEmpty;

    DataModel *dataModel;

    QAction *actionName;
    QAction *actionNickName;

private:
    bool isAdd;
    bool isSelect;

    QList<QString> listSizes;
    QList<QString> listValues;
};

#endif // PRODUCTINTERFACE_H
