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


 #ifndef CONFIG_MANAGEUSER_H
#define CONFIG_MANAGEUSER_H

#include <QWidget>
#include <Component/tableview.h>
#include <Abstract/datamodel.h>
#include <Component/edit.h>
#include <QPushButton>
#include <Component/combobox.h>
#include <Component/spinbox.h>
#include <QTextBrowser>
#include <Object/config.h>
#include <QCheckBox>

class Config_ManageUser : public QWidget
{
    Q_OBJECT
public:
    explicit Config_ManageUser(QWidget *parent = 0);

    void createComponent();

    void input();

    void resizeEvent(QResizeEvent *);

    void connections();

    void statics();

    void clearForm();

    void getAllUsers();

    void setIdRoot(QString idRoot);

    QString getIdRoot();

    void setStateCheckbox(bool state);

signals:
    void updateUser();

public slots:

    void adjustLayout();

    void configureAccount(QString name);

    void onCurrentIndexChanged_cbUseTimeout(int index);

    void doubleClicked_tableUserStandard(QModelIndex index);

    void onClick_pbConfirm();

    void onClick_pbConfirmAccess();

    void onClick_pbLeave();

    void onClick_pbRemoveUser();

    void onTextChanged_ePassword();

    void onTextChanged_eConfirmPassword();

    void currentChanged_wTabs(int index);

    void onClick_pbSaveLogout();

public:
    Config *config;

    QTabWidget *wTabs;
    DataModel *modelUserStandard;

private:

    QString idRoot;

    bool isAdd;

    QString idForUpdate;

    QLabel *lTabs;

    QWidget *wManageUser;
    TableView *tableAccounts;
    QPushButton *pbRemoveUser;
    Edit *eUsername;
    Edit *ePassword;
    Edit *eConfirmPassword;
    Edit *eSecretQuestion;
    Edit *eSecretAnswer;
    QPushButton *pbConfirm;
    QPushButton *pbLeave;

    QWidget *wManageAccess;    
    TableView *tableAccountsAccess;
    QLabel *textaccess;
    QCheckBox *checkTreatment;
    QCheckBox *checkClient;
    QCheckBox *checkProduct;
    QCheckBox *checkOrdered;
    QCheckBox *checkEmployee;
    QCheckBox *checkAlert;
    QCheckBox *checkBill;
    QCheckBox *checkCash;
    QLabel *textConfigure;
    QCheckBox *checkCreateUse;
    QCheckBox *checkAlteraccess;
    QPushButton *pbConfirmaccess;

    QWidget *wLogoutTime;
    ComboBox *cbUseTimeout;

    QTextBrowser *UseText;
    SpinBox *SBTime;
    QPushButton *pbSaveLogout;

};

#endif // CONFIG_MANAGEUSER_H
