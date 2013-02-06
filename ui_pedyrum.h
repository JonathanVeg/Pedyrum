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


 /********************************************************************************
** Form generated from reading UI file 'pedyrum.ui'
**
** Created: Wed Jul 4 19:00:41 2012
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PEDYRUM_H
#define UI_PEDYRUM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Pedyrum
{
public:
    QWidget *wCenter;

    void setupUi(QMainWindow *Pedyrum)
    {
        if (Pedyrum->objectName().isEmpty())
            Pedyrum->setObjectName(QString::fromUtf8("Pedyrum"));
        Pedyrum->resize(819, 587);
        wCenter = new QWidget(Pedyrum);
        wCenter->setObjectName(QString::fromUtf8("wCenter"));
        Pedyrum->setCentralWidget(wCenter);

        retranslateUi(Pedyrum);

        QMetaObject::connectSlotsByName(Pedyrum);
    } // setupUi

    void retranslateUi(QMainWindow *Pedyrum)
    {
        Pedyrum->setWindowTitle(QApplication::translate("Pedyrum", "Pedyrum", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Pedyrum: public Ui_Pedyrum {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PEDYRUM_H
