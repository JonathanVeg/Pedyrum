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


 #include "Interface/firstadmininterface.h"
#include <QMessageBox>

FirstAdminInterface::FirstAdminInterface(QWidget *parent) :
    QWidget(parent)
{
    createComponent();
    connections();
    input();

    bd.start("bdpedyrum", "connectionFirstAdmin");
}

void FirstAdminInterface::createComponent()
{
    lText = new QLabel(this);

    wGroup = new QWidget(this);

    lBackground = new QLabel(wGroup);

    eUsername = new Edit(wGroup,true,tr("Nome"));
    ePassword = new Edit(wGroup,true,tr("Senha"));
    ePasswordConfirm = new Edit(wGroup,true,tr("Confirmação de senha"));

    eSecretAsk = new Edit(wGroup,true,tr("Pergunta Secreta"));
    eAnswer = new Edit(wGroup,true,tr("Resposta"));

    pbCreate = new QPushButton(wGroup);
    pbLeave = new QPushButton(wGroup);
}

void FirstAdminInterface::input()
{
    QFont f;
    f.setPointSize(20);

    lText->setText(tr("Cadastro de Administrador"));
    lText->setStyleSheet("color: rgba(0,0,0,40);");
    lText->setFont(f);
    lText->setAlignment(Qt::AlignCenter);

    lBackground->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.498,"
                               "y1:0, x2:0.494636, y2:1, stop:0 rgba(255, 255, 255, 210),"
                               "stop:0.982955 rgba(255, 255, 255, 170));"
                               "border-radius: 15px;border: 1px solid rgba(0,0,0,25);");

    pbCreate->setText(tr("Criar Conta"));

    pbLeave->setText(tr("Limpar"));

    eUsername->setNextComponent(ePassword);
    ePassword->setNextComponent(ePasswordConfirm);
    ePasswordConfirm->setNextComponent(eSecretAsk);
    eSecretAsk->setNextComponent(eAnswer);
}

void FirstAdminInterface::adjustLayout()
{
    wGroup->setGeometry(this->width()/2 - 140,this->height()/2 - 175,280,350);

    lText->setGeometry(this->width()/2 - 300, wGroup->y() - 50, 600,30);

    lBackground->setGeometry(0,0,wGroup->width(),wGroup->height());

    eUsername->setGeometry(wGroup->width()/2 - 100,45,200,28);
    ePassword->setGeometry(wGroup->width()/2 - 100,eUsername->y() + 48,200,28);
    ePasswordConfirm->setGeometry(wGroup->width()/2 - 100,ePassword->y() + 48,200,28);

    eSecretAsk->setGeometry(wGroup->width()/2 - 100,ePasswordConfirm->y() + 48,200,28);
    eAnswer->setGeometry(wGroup->width()/2 - 100,eSecretAsk->y() + 48,200,28);

    pbLeave->setGeometry(eAnswer->x(),eAnswer->y() + 48,95,35);
    pbCreate->setGeometry(pbLeave->width() + pbLeave->x() + 10,pbLeave->y(),95,35);
}

void FirstAdminInterface::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void FirstAdminInterface::textChanged_ePassword()
{
    if(ePassword->text() == ePassword->valueStandart)
        ePassword->setEchoMode(Edit::Normal);
    else
        ePassword->setEchoMode(Edit::Password);
}

void FirstAdminInterface::textChanged_ePasswordConfirm()
{
    if(ePasswordConfirm->text() == ePasswordConfirm->valueStandart)
        ePasswordConfirm->setEchoMode(Edit::Normal);
    else
        ePasswordConfirm->setEchoMode(Edit::Password);
}

void FirstAdminInterface::onClick_pbCreate()
{
    emit gotoNext(2);

    bool canSave = true;

    if (eUsername->isEmpty()){
        eUsername->setFocus();

        canSave = false;
    }else if (ePassword->isEmpty()){
        ePassword->setFocus();

        canSave = false;
    }else if (ePasswordConfirm->isEmpty()){
        ePasswordConfirm->setFocus();

        canSave = false;
    }else if (eSecretAsk->isEmpty()){
        eSecretAsk->setFocus();

        canSave = false;
    }else if (eAnswer->isEmpty()){
        eAnswer->setFocus();

        canSave = false;
    }

    if (! canSave){
        QMessageBox::warning(this, tr("Cadastro incompleto"), tr("Todos os campos devem ser preenchidos."));

        return;
    }

    if (ePassword->text() != ePasswordConfirm->text()){
        QMessageBox::warning(this, tr("Senha inválida"), tr("Senha e confirmação de senha devem ser iguais."));

        ePassword->Clear();

        ePasswordConfirm->Clear();

        ePassword->setFocus();

        return;
    }

    // verifica se já tem usuário cadastrado

    if (bd.search("select * from usersystem where username = '" + e.encrypt(eUsername->text()) + "'")){
        QMessageBox::warning(this, tr("Erro"), tr("Já existe cadastro com esse nome de usuário."), QMessageBox::Ok);

        eUsername->setFocus();

        return;
    }

    QList<QString> values;

    values << QString::number(bd.generateID("usersystem", "iduser")) << e.encrypt(eUsername->text()) << e.encrypt(ePassword->text()) << e.encrypt(eSecretAsk->text()) << e.encrypt(eAnswer->text());

    if (bd.insert("usersystem", values)){

        QMessageBox::information(this, tr("Cadastro efeuado com sucesso"), tr("Usuário cadastrado com sucesso."), QMessageBox::Ok);

        onClick_pbLeave();

    }
    else
        QMessageBox::warning(this, tr("Usuário não cadastrado"), tr("Ocorreu um erro no cadastro do usuário."), QMessageBox::Ok);
}

void FirstAdminInterface::onClick_pbLeave()
{
    eUsername->Clear();
    ePassword->Clear();
    ePasswordConfirm->Clear();
    eSecretAsk->Clear();
    eAnswer->Clear();

    pbLeave->setFocus();
}

void FirstAdminInterface::connections()
{
    connect(ePassword,SIGNAL(textChanged(QString)),this,SLOT(textChanged_ePassword()));

    connect(ePasswordConfirm,SIGNAL(textChanged(QString)),this,SLOT(textChanged_ePasswordConfirm()));

    connect(pbCreate,SIGNAL(clicked()),this,SLOT(onClick_pbCreate()));

    connect(pbLeave,SIGNAL(clicked()),this,SLOT(onClick_pbLeave()));

    connect(eAnswer, SIGNAL(returnPressed()), SLOT(onClick_pbCreate()));
}

