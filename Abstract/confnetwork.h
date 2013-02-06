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


 #ifndef CONFNETWORK_H
#define CONFNETWORK_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <Component/edit.h>
#include <Component/combobox.h>
#include <QComboBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QCheckBox>
#include <Abstract/encryption.h>
#include <QToolButton>
#include <QAction>
#include <QStackedWidget>
#include <Object/createdatabasethread.h>

class ConfNetwork : public QWidget
{
    Q_OBJECT
public: // funções
    explicit ConfNetwork(QWidget *parent = 0);

    void testConnectionFile();

    void testConnectionInput();

    void resizeEvent(QResizeEvent *); // ajusta os componentes no resize da tela

    bool fieldVerification();

    void createNewTables();


private: // funções

    void createComponent(); // cria os componentes

    void input(); // seta as configurações iniciais do componente

    void connections(); // cria as conexões

    void statics(); // seta os compoentes não alterados no resize.

    QString getIP(); // pega o ip do terminal em uso

    bool isValid(QString text); // verifica se só tem numeros

    void saveData(QString username, QString password, QString ip, int port);

    void createDatabase();

    bool configPostgres(QString filename);    

private: // variáveis
    QSqlDatabase sdb; // para testes de conexão

    Encryption e;

signals:

    void gotoNext(int index);        
    void connected();
    void goToSplash();
    void noConnected();

private slots:

    void adjustLayout();

    void onClick_pbConnect();

    void onTextChanged_ePassword();

    void onTextChanged_ePort();

    void onCurrentIndexChanged_cbTypeConnection(int index);

    void onTriggered_aShowPassword();

    void onClick_lDownloadManual();

    void hideSplash();

    void showSplash();

    void databaseC(bool b, QString s);

    void tablesC(bool b, QString s);

    void proceduresC(bool b, QString s);

    void allC();

public:

    QString dbName;

    QWidget *wGroup;
    QLabel *lBackground;

    QLabel *lLogo;

    QLabel *lIpDisplay; // exibir o ipLocal
    Edit *eUsername; // username do banco de dados
    Edit *ePassword;
    QToolButton *tbViewPassword;
    QAction *aShowPassword;
    ComboBox *cbTypeConection; // pega o tipo de conexão, como cliente ou como servidor
    Edit *eIP;
    Edit *ePort;
    Edit *eDir;    
    Label *lDownloadManual;

    QPushButton *pbConnect; // conectar

    QStackedWidget *swPanels;

    QWidget *wSplash;
    QLabel *lSplash;

    CreateDatabaseThread *cdThread;

};

#endif // CONFNETWORK_H
