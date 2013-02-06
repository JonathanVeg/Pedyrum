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


 #include "config_manageuser.h"
#include <QList>
#include <QDebug>
#include <QMessageBox>

Config_ManageUser::Config_ManageUser(QWidget *parent) :
    QWidget(parent)
{
    createComponent();    
    connections();
    statics();
    input();
}

void Config_ManageUser::createComponent()
{
    config = new Config();

    lTabs = new QLabel(this);
    {
        wTabs = new QTabWidget(this);

        wManageUser = new QWidget(this);
        {
            tableAccounts = new TableView(wManageUser);
            pbRemoveUser = new QPushButton(tr("Remover"),wManageUser);
            eUsername = new Edit(wManageUser,true, tr("Nome"), 30);
            ePassword = new Edit(wManageUser,true, tr("Senha"), 20);
            eConfirmPassword = new Edit(wManageUser,true, tr("Confirmação de senha"), 20);
            eSecretQuestion = new Edit(wManageUser,true, tr("Pergunta secreta"), 150);
            eSecretAnswer = new Edit(wManageUser,true, tr("Resposta secreta"), 150);
            pbConfirm = new QPushButton(tr("Adicionar"),wManageUser);
            pbLeave = new QPushButton(tr("Limpar"),wManageUser);

            QList<QString> list;
            list << tr("ID") << tr("Usuários");
            modelUserStandard = new DataModel(wManageUser,list);
        }

        wLogoutTime = new QWidget(this);
        {
            cbUseTimeout = new ComboBox(wLogoutTime);
            UseText = new QTextBrowser(wLogoutTime);
            SBTime = new SpinBox(wLogoutTime);
            pbSaveLogout = new QPushButton(tr("Salvar"),wLogoutTime);
        }

        wManageAccess = new QWidget(this);
        {
            tableAccountsAccess = new TableView(wManageAccess);
            textaccess = new QLabel(wManageAccess);
            checkTreatment = new QCheckBox(tr("Atendimento"),wManageAccess);
            checkClient = new QCheckBox(tr("Cliente"),wManageAccess);
            checkProduct = new QCheckBox(tr("Produto"),wManageAccess);
            checkOrdered = new QCheckBox(tr("Pedido"),wManageAccess);
            checkEmployee = new QCheckBox(tr("Funcionário"),wManageAccess);
            checkAlert = new QCheckBox(tr("Alerta"),wManageAccess);
            checkBill = new QCheckBox(tr("Contas"),wManageAccess);
            checkCash = new QCheckBox(tr("Caixa"),wManageAccess);

            textConfigure = new QLabel(wManageAccess);
            checkCreateUse = new QCheckBox(tr("Criar usuários"),wManageAccess);
            checkAlteraccess = new QCheckBox(tr("Mudar acesso dos usuários"),wManageAccess);
            pbConfirmaccess = new QPushButton(tr("Salvar"),wManageAccess);
        }
    }
}

void Config_ManageUser::input()
{
    isAdd = true;

    wTabs->addTab(wManageUser,tr(" Gerenciar usuários "));
    wTabs->addTab(wManageAccess,tr(" Nível de acesso "));
    //wTabs->addTab(wLogoutTime,tr(" Logout Automático "));

    wLogoutTime->hide();

    lTabs->setStyleSheet("background-color: rgba(0,0,0,180);"
                         "border: 1px solid rgba(255, 255, 255, 10);"
                         "border-radius: 15px; border: 1px solid rgba(255,255,255,255);");

    tableAccounts->setModel(modelUserStandard);
    tableAccounts->setColumnHidden(0,true);
    tableAccounts->setColumnWidth(1,240);

    tableAccountsAccess->setModel(modelUserStandard);
    tableAccountsAccess->setColumnHidden(0,true);
    tableAccountsAccess->setColumnWidth(1,240);

    cbUseTimeout->add(tr("Usar logout automático."));
    cbUseTimeout->add(tr("Não usar logout"));

    QFont f;

    f.setPointSize(9);

    UseText->setText(tr("Cada vez que o usuário ficar"
                        " o tempo que ele especificou sem mover"
                        " o mouse, automaticamente ele se desloga do sistema"
                        " e volta para a tela de login."));
    UseText->setFont(f);

    UseText->setStyleSheet("background-color: rgba(0,0,0,0);"
                           "color: rgba(255,255,255,230);");

    SBTime->setSuffix(tr(" minuto(s)"));
    SBTime->setMinimum(1);

    eUsername->setNextComponent(ePassword);
    ePassword->setNextComponent(eConfirmPassword);
    eConfirmPassword->setNextComponent(eSecretQuestion);
    eSecretQuestion->setNextComponent(eSecretAnswer);

    ePassword->setEchoMode(Edit::Password);
    onTextChanged_ePassword();

    eConfirmPassword->setEchoMode(Edit::Password);
    onTextChanged_eConfirmPassword();

    f.setPointSize(f.pointSize() + 8);

    textaccess->setText(tr("Nível de acesso dos usuários"));
    textaccess->setStyleSheet("color: rgba(255,255,255,190);");
    textaccess->setAlignment(Qt::AlignCenter);
    textaccess->setFont(f);

    checkTreatment->setStyleSheet(textaccess->styleSheet());
    checkClient->setStyleSheet(textaccess->styleSheet());
    checkProduct->setStyleSheet(textaccess->styleSheet());
    checkOrdered->setStyleSheet(textaccess->styleSheet());
    checkEmployee->setStyleSheet(textaccess->styleSheet());
    checkAlert->setStyleSheet(textaccess->styleSheet());
    checkBill->setStyleSheet(textaccess->styleSheet());
    checkCash->setStyleSheet(textaccess->styleSheet());

    f.setPointSize(f.pointSize() - 6);

    textConfigure->setText(tr("Gerenciar usuários"));
    textConfigure->setStyleSheet("color: rgba(255,255,255,200);");
    textConfigure->setAlignment(Qt::AlignCenter);
    textConfigure->setFont(f);

    checkCreateUse->setStyleSheet(textaccess->styleSheet());
    checkAlteraccess->setStyleSheet(textaccess->styleSheet());
}

void Config_ManageUser::adjustLayout()
{
    lTabs->setGeometry(0,0,this->width(),this->height());

    wTabs->setGeometry(20,20,this->width() - 40,this->height() - 40);

    wManageUser->setGeometry(0,0,wTabs->width(),wTabs->height());

    wManageAccess->setGeometry(0,0,wTabs->width(),wTabs->height());

    wLogoutTime->setGeometry(0,0,wTabs->width(),wTabs->height());

    if(cbUseTimeout->currentIndex() == 1)
        UseText->setGeometry(UseText->x(),UseText->y(), UseText->width(),0);
    else
        UseText->setGeometry(UseText->x(),UseText->y(), UseText->width(),100);

    SBTime->setGeometry(cbUseTimeout->x(),UseText->y() + UseText->height() + 10,200, 28);
    pbSaveLogout->setGeometry(SBTime->x() + 55,SBTime->y() + SBTime->height() + 10,90,30);


}

void Config_ManageUser::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void Config_ManageUser::connections()
{
    connect(cbUseTimeout,SIGNAL(currentIndexChanged(int)),SLOT(onCurrentIndexChanged_cbUseTimeout(int)));

    connect(eSecretAnswer, SIGNAL(returnPressed()), SLOT(onClick_pbConfirm()));

    connect(pbConfirm, SIGNAL(clicked()), SLOT(onClick_pbConfirm()));

    connect(pbLeave, SIGNAL(clicked()), SLOT(onClick_pbLeave()));

    connect(pbRemoveUser, SIGNAL(clicked()), SLOT(onClick_pbRemoveUser()));

    connect(tableAccounts, SIGNAL(doubleClicked(QModelIndex)), SLOT(doubleClicked_tableUserStandard(QModelIndex)));

    connect(tableAccountsAccess, SIGNAL(doubleClicked(QModelIndex)), SLOT(doubleClicked_tableUserStandard(QModelIndex)));

    connect(ePassword,SIGNAL(textChanged(QString)),SLOT(onTextChanged_ePassword()));

    connect(eConfirmPassword,SIGNAL(textChanged(QString)),SLOT(onTextChanged_eConfirmPassword()));

    connect(wTabs, SIGNAL(currentChanged(int)), SLOT(currentChanged_wTabs(int)));

    connect(pbConfirmaccess, SIGNAL(clicked()), SLOT(onClick_pbConfirmAccess()));

    connect(pbSaveLogout,SIGNAL(clicked()),SLOT(onClick_pbSaveLogout()));
}

void Config_ManageUser::statics()
{
    tableAccounts->setGeometry(5,70,240, 290);
    pbRemoveUser->setGeometry(5,tableAccounts->height() + tableAccounts->y(),tableAccounts->width(),30);

    eUsername->setGeometry(tableAccounts->x() + tableAccounts->width() + 70,120, 200,28);
    ePassword->setGeometry(eUsername->x(),eUsername->y() + eUsername->height() + 15,eUsername->width(),28);
    eConfirmPassword->setGeometry(ePassword->x(),ePassword->y() + ePassword->height() + 15,ePassword->width(),28);
    eSecretQuestion->setGeometry(eConfirmPassword->x(),eConfirmPassword->y() + eConfirmPassword->height() + 15,eConfirmPassword->width(),28);
    eSecretAnswer->setGeometry(eSecretQuestion->x(),eSecretQuestion->y() + eSecretQuestion->height() + 15,eSecretQuestion->width(),28);

    pbConfirm->setGeometry(eUsername->x(), eSecretAnswer->y() + eSecretAnswer->height() + 15,90,30);
    pbLeave->setGeometry(pbConfirm->x() + pbConfirm->width() + 20, eSecretAnswer->y() + eSecretAnswer->height() + 15,90,30);

    cbUseTimeout->setGeometry(180, 80,200,28);
    UseText->setGeometry(cbUseTimeout->x(), cbUseTimeout->y() + cbUseTimeout->height() + 10, 200, 100);
    SBTime->setGeometry(cbUseTimeout->x(),UseText->y() + UseText->height() + 10,200, 28);
    pbSaveLogout->setGeometry(SBTime->x() + 55,SBTime->y() + SBTime->height() + 10,90,30);

    tableAccountsAccess->setGeometry(tableAccounts->geometry());

    textaccess->setGeometry(100,30,400,20);

    checkTreatment->setGeometry(tableAccountsAccess->x() + tableAccountsAccess->width() + 40, tableAccountsAccess->y() + 30, 110,20);
    checkClient->setGeometry(tableAccountsAccess->x() + tableAccountsAccess->width() + 40, checkTreatment->y() + checkTreatment->height() + 20, 110,20);
    checkProduct->setGeometry(tableAccountsAccess->x() + tableAccountsAccess->width() + 40, checkClient->y() + checkClient->height() + 20, 110,20);
    checkOrdered->setGeometry(tableAccountsAccess->x() + tableAccountsAccess->width() + 40, checkProduct->y() + checkProduct->height() + 20, 110,20);

    checkEmployee->setGeometry(checkTreatment->x() + checkTreatment->width() + 40, checkTreatment->y(), 110,20);
    checkAlert->setGeometry(checkClient->x() + checkClient->width() + 40, checkClient->y(), 110,20);
    checkBill->setGeometry(checkProduct->x() + checkProduct->width() + 40, checkProduct->y(), 110,20);
    checkCash->setGeometry(checkOrdered->x() + checkOrdered->width() + 40, checkOrdered->y(), 110,20);

    textConfigure->setGeometry(300,checkCash->y() + checkCash->height() + 22,200,20);

    checkCreateUse->setGeometry(tableAccountsAccess->x() + tableAccountsAccess->width() + 40, textConfigure->y() + textConfigure->height() + 14, 220,20);
    checkAlteraccess->setGeometry(tableAccountsAccess->x() + tableAccountsAccess->width() + 40, checkCreateUse->y() + checkCreateUse->height() + 14, 220,20);
    pbConfirmaccess->setGeometry(255, tableAccountsAccess->y() + tableAccountsAccess->height() + 15,90,28);
}

void Config_ManageUser::setIdRoot(QString idRoot){
    this->idRoot = idRoot;
}

void Config_ManageUser::configureAccount(QString name)
{
    // carregar dados;
}

void Config_ManageUser::onCurrentIndexChanged_cbUseTimeout(int index)
{
    if(index == 0)
        SBTime->setEnabled(true);
    else
        SBTime->setEnabled(false);

    adjustLayout();
}

void Config_ManageUser::onClick_pbConfirm()
{
    if (! config->canEditAccount(config->CurrentIdSystemUser)){
        QMessageBox::warning(this, tr("Conta não adicionada"), tr("Você não possui permissão para adicionar novas contas."), QMessageBox::Ok);

        return;
    }

    if (eUsername->isEmpty()){
        QMessageBox::warning(this, tr("Conta não adicionada"), tr("Todos os campos devem ser preenchidos."), QMessageBox::Ok);

        eUsername->setFocus();

        return;
    }

    if (ePassword->isEmpty()){
        QMessageBox::warning(this, tr("Conta não adicionada"), tr("Todos os campos devem ser preenchidos."), QMessageBox::Ok);

        ePassword->setFocus();

        return;
    }

    if (eConfirmPassword->isEmpty()){
        QMessageBox::warning(this, tr("Conta não adicionada"), tr("Todos os campos devem ser preenchidos."), QMessageBox::Ok);

        eConfirmPassword->setFocus();

        return;
    }

    if (eSecretQuestion->isEmpty()){
        QMessageBox::warning(this, tr("Conta não adicionada"), tr("Todos os campos devem ser preenchidos."), QMessageBox::Ok);

        eSecretQuestion->setFocus();

        return;
    }

    if (eSecretAnswer->isEmpty()){
        QMessageBox::warning(this, tr("Conta não adicionada"), tr("Todos os campos devem ser preenchidos."), QMessageBox::Ok);

        eSecretAnswer->setFocus();

        return;
    }

    if (ePassword->value() != eConfirmPassword->value()){
        QMessageBox::warning(this, tr("Conta não adicionada"), tr("Senha e confirmação de senha devem ser iguais."), QMessageBox::Ok);

        ePassword->Clear();

        eConfirmPassword->Clear();

        ePassword->setFocus();

        return;
    }

    if (isAdd){

        config->usernameSystemUser = eUsername->value();

        if (config->canAddAccount()){
            config->passwordSystemUser = ePassword->value();

            config->secretQuestionSystemUser = eSecretQuestion->value();

            config->secretAnswerSystemUser = eSecretAnswer->value();

            if (config->insertUserAccount()){
                QMessageBox::information(this, tr("Adicionada com sucesso"), tr("Conta de usuário adicionada com sucesso."), QMessageBox::Ok);

                getAllUsers();

                clearForm();

            }else{

                QString errorMessage = QString(tr("Conta de usuário não foi adicionada.\nErro para informar ao suporte:\n'%1'")).arg(config->bd.lastError);

                QMessageBox::warning(this, tr("Conta não adicionada"), errorMessage, QMessageBox::Ok);

            }

            return;
        }

        QMessageBox::warning(this, tr("Conta não adicionada"), tr("Esse nome de usuário já está cadastrado."), QMessageBox::Ok);

        eUsername->setFocus();
    }else{

        /// edição

        config->usernameSystemUser = eUsername->value();

        config->passwordSystemUser = ePassword->value();

        config->secretQuestionSystemUser = eSecretQuestion->value();

        config->secretAnswerSystemUser = eSecretAnswer->value();

        if (config->updateUserAccount(idForUpdate)){
            QMessageBox::information(this, tr("Operação realizada com sucesso"), tr("Usuário alterado com sucesso."), QMessageBox::Ok);

            onClick_pbLeave();

        }else{
            QString errorMessage = QString("O usuário não pode ser alterado.\n\nErro para informar ao suporte:\n%1").arg(config->bd.lastError);

            QMessageBox::warning(this, tr("Operação não realizada"), errorMessage, QMessageBox::Ok);
        }
    }
}

void Config_ManageUser::onClick_pbRemoveUser(){

    if (config->CurrentIdSystemUser != config->idRoot()){
        QMessageBox::warning(this, tr("Operação bloqueada."), tr("Somente o administrador pode excluir usuários."), QMessageBox::Ok);

        return;
    }

    if (tableAccounts->currentIndex().row() < 0){
        QMessageBox::warning(this, tr("Selecione um usuário"), tr("Selecione um usuário para excluir."), QMessageBox::Ok);

        return;
    }

    if (modelUserStandard->index(tableAccounts->currentIndex().row(), 0).data().toString() == config->idRoot()){

        QMessageBox::warning(this, tr("Operação bloqueada."), tr("A conta do administrador não pode ser excluída, somente alterada."), QMessageBox::Ok);

        return;
    }

    int op = QMessageBox::question(this, tr("Deletar usuário"), tr("Tem certeza que deseja deletar o usuário selecionado?"), tr("Sim"), tr("Não"));

    if (op == 0){
        if (config->removeUser(modelUserStandard->index(tableAccounts->currentIndex().row(), 0).data().toString())){

            QMessageBox::information(this, tr("Removido com sucesso"), tr("Usuário removido com sucesso."), QMessageBox::Ok);

            getAllUsers();
        }
        else{

            QMessageBox::warning(this, tr("Usuário não removido"), tr("Usuário não pode ser removido."), QMessageBox::Ok);

        }
    }
}

void Config_ManageUser::clearForm()
{
    eUsername->Clear();

    ePassword->Clear();

    eConfirmPassword->Clear();

    eSecretQuestion->Clear();

    eSecretAnswer->Clear();

    eUsername->setFocus();
}

void Config_ManageUser::getAllUsers()
{
    QList<QList<QString> > allUsers = config->getAllUsers();

    QList<QString> user;

    modelUserStandard->clear();

    for (int i = 0; i < allUsers[0].length(); i++){
        user.clear();

        user << allUsers[0][i] << allUsers[1][i];

        modelUserStandard->add(user);
    }
}

void Config_ManageUser::doubleClicked_tableUserStandard(QModelIndex index)
{
    //verifica se ele é o root do sistema ou o cara logado na hora.

    idRoot = config->idRoot();

    // primeiro ve se é o root.
    QString currentSelectedId = tableAccounts->model()->index(index.row(), 0).data().toString();

    bool canEdit = config->canEditAccount(currentSelectedId);

    bool canAlterAccess = config->canAlterAccess(currentSelectedId);

    if ((currentSelectedId == idRoot) && (idRoot != config->CurrentIdSystemUser)){
        QString errorMessage;

        errorMessage = QString(tr("Você não pode alterar essa conta de usuário."));

        QMessageBox::warning(this, tr("Operação inválida"), errorMessage, QMessageBox::Ok);

        return;
    }


    if (config->CurrentIdSystemUser != idRoot){

        // se não for o root, ele ve se é o próprio usuário

        if (currentSelectedId != config->CurrentIdSystemUser && (!canEdit) && (!canAlterAccess)){
            QString errorMessage;

            errorMessage = QString(tr("Você não pode alterar essa conta de usuário."));

            QMessageBox::warning(this, tr("Operação inválida"), errorMessage, QMessageBox::Ok);

            return;
        }

        if ((!canEdit) && (canAlterAccess)){
            if (wTabs->currentIndex() == 0){
                QString errorMessage;

                errorMessage = QString(tr("Você não pode alterar os dados dessa conta de usuário, somente suas permissões de acesso."));

                QMessageBox::warning(this, tr("Operação restrita"), errorMessage, QMessageBox::Ok);

                wTabs->setCurrentIndex(1);
            }
        }else if(canEdit && (! canAlterAccess)){
            if (wTabs->currentIndex() == 1){
                QString errorMessage;

                errorMessage = QString(tr("Você não pode alterar as permissões de acesso dessa conta de usuário, somente seus dados."));

                QMessageBox::warning(this, tr("Operação restrita"), errorMessage, QMessageBox::Ok);

                wTabs->setCurrentIndex(0);
            }
        }
    }

    if (config->loadDataUser(currentSelectedId)){

        eUsername->setText(config->usernameSystemUser);
        ePassword->setText(config->passwordSystemUser);
        eConfirmPassword->setText(config->passwordSystemUser);
        eSecretQuestion->setText(config->secretQuestionSystemUser);
        eSecretAnswer->setText(config->secretAnswerSystemUser);

        isAdd = false;

        idForUpdate = currentSelectedId;

        pbConfirm->setText(tr("Alterar"));
        pbLeave->setText(tr("Cancelar"));

        checkTreatment->setChecked(config->cantreatment);
        checkClient->setChecked(config->canclient);
        checkProduct->setChecked(config->canproduct);
        checkOrdered->setChecked(config->canordered);
        checkEmployee->setChecked(config->canemployee);
        checkAlert->setChecked(config->canalert);
        checkBill->setChecked(config->canbill);
        checkCash->setChecked(config->cancash);
        checkCreateUse->setChecked(config->cancreateuser);
        checkAlteraccess->setChecked(config->canalteraccess);

        if (config->CurrentIdSystemUser == idRoot){
            if (currentSelectedId == idRoot){
                setStateCheckbox(false);
            }else{
                setStateCheckbox(true);
            }

        }else{
            setStateCheckbox(canAlterAccess);
        }

    }
}

void Config_ManageUser::setStateCheckbox(bool state){
    checkTreatment->setEnabled(state);
    checkClient->setEnabled(state);
    checkProduct->setEnabled(state);
    checkOrdered->setEnabled(state);
    checkEmployee->setEnabled(state);
    checkAlert->setEnabled(state);
    checkBill->setEnabled(state);
    checkCash->setEnabled(state);
    checkCreateUse->setEnabled(state);
    checkAlteraccess->setEnabled(state);
}

void Config_ManageUser::onClick_pbLeave()
{
    isAdd = true;

    pbConfirm->setText("Adicionar");

    pbLeave->setText("Limpar");

    eUsername->Clear();

    ePassword->Clear();

    eConfirmPassword->Clear();

    eSecretQuestion->Clear();

    eSecretAnswer->Clear();
}

void Config_ManageUser::onTextChanged_eConfirmPassword()
{
    if(eConfirmPassword->text() == eConfirmPassword->valueStandart)
        eConfirmPassword->setEchoMode(Edit::Normal);
    else
        eConfirmPassword->setEchoMode(Edit::Password);
}

void Config_ManageUser::onTextChanged_ePassword()
{
    if(ePassword->text() == ePassword->valueStandart)
        ePassword->setEchoMode(Edit::Normal);
    else
        ePassword->setEchoMode(Edit::Password);
}

void Config_ManageUser::currentChanged_wTabs(int index){
}

void Config_ManageUser::onClick_pbConfirmAccess()
{
    config->canalert = checkAlert->isChecked();
    config->canalteraccess = checkAlteraccess->isChecked();
    config->canbill = checkBill->isChecked();
    config->canclient = checkClient->isChecked();
    config->cancash = checkCash->isChecked();
    config->cancreateuser = checkCreateUse->isChecked();
    config->canemployee = checkEmployee->isChecked();
    config->canordered = checkOrdered->isChecked();
    config->canproduct = checkProduct->isChecked();
    config->cantreatment = checkTreatment->isChecked();

    config->insertUserAccessLevel(idForUpdate);
}

void Config_ManageUser::onClick_pbSaveLogout()
{

}

QString Config_ManageUser::getIdRoot()
{
    return this->idRoot;
}
