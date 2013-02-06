/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

//
/// O Código sofreu modificações, por Luiz Cândido.
/// O código original pode ser encontrado no site http://doc.qt.nokia.com/stable/itemviews-addressbook-tablemodel-cpp.html
//

#include "Abstract/datamodel.h"
#include <QDebug>

DataModel::DataModel(QObject *parent, QList<QString> headers) :
    QAbstractTableModel(parent)
{
    listheader = headers;
    col = headers.count();
}

DataModel::DataModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}

int DataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return modelmatrix.count();
}

int DataModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return col;
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= modelmatrix.count() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {

        return modelmatrix.value(index.row()).value(index.column());
    }

    if (role == Qt::TextAlignmentRole){

        return Qt::AlignHCenter + Qt::AlignVCenter;
    }
    return QVariant();
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
            return listheader.value(section);
    }
    return QVariant();
}

bool DataModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position+rows-1);

    for (int row=0; row < rows; row++) {
        QList<QString> linha;
        for (int n = 0; n < col; ++n)
        {
            linha << "";
        }
        modelmatrix.insert(position,linha);
    }

    endInsertRows();
    return true;
}

bool DataModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row=0; row < rows; ++row)
        modelmatrix.removeAt(position);

    endRemoveRows();

    this->submit();

    return true;
}

bool DataModel::setData(QModelIndex index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        int row = index.row();
        int column = index.column();

        QList<QString> p;

        p = modelmatrix.value(row);

        p.replace(column,value.toString());

        modelmatrix.replace(row,p);

        emit(dataChanged(index, index));

        return true;
    }

    return false;
}


Qt::ItemFlags DataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

void DataModel::clear()
{
    if(modelmatrix.count() > 0)
    {
        this->removeRows(0,modelmatrix.count(),QModelIndex());
    }
}

void DataModel::add(QList<QString> values)
{
    QModelIndex index;
    unsigned short int position;

    index = this->index(0, 0, QModelIndex());

    position = modelmatrix.count();

    this->insertRow(position,QModelIndex());

    for (int column = 0; column < values.count(); column++) {

        index = this->index(position, column, QModelIndex());

        this->setData(index,values.value(column),Qt::EditRole);
    }

    this->submit();
}

bool DataModel::remove(int row)
{
    qDebug() << row;

    if(row < 0)
    {
        qDebug("---------------------------------- Tratamento de erro.");
        return false;
    }

    beginRemoveRows(QModelIndex(),row,row);

    modelmatrix.removeAt(row);

    endRemoveRows();

    this->submit();

    return true;
}

float DataModel::sum(int column)
{
    float value;
    QString temp;

    for (int row = 0; row < modelmatrix.count(); row++)
    {
        temp =  this->modelmatrix.value(row).value(column);
        value += temp.toFloat();
    }

    return value;
}

float DataModel::media(int column)
{
    return (sum(column) / this->rowCount(QModelIndex()));
}

QList<QString> DataModel::row(int row)
{
    return this->modelmatrix.value(row);
}

QList<QString> DataModel::column(int column)
{
    QList<QString> columns;

    for (int row = 0; row < modelmatrix.count(); row++)
        columns << this->modelmatrix.value(row).value(column);

    return columns;
}

QList<QString> DataModel::search(int column, QVariant value)
{
    for (int row = 0; row < modelmatrix.count(); row++)
    {
        if(value.toString() == modelmatrix.value(row).value(column))
           return modelmatrix.value(row);
    }

    //criei uma lista vazia de retorno, pois se por algum motivo chegar aqui, daria erro.
    QList<QString> listEmpty;

    return listEmpty;
}

void DataModel::addText(QString value)
{
    QModelIndex index;
    int position;

    index = this->index(0, 0, QModelIndex());

    position = modelmatrix.count();

    this->insertRow(position,QModelIndex());

    index = this->index(position, 0, QModelIndex());

    this->setData(index,value,Qt::EditRole);

    this->submit();
}

bool DataModel::isEmpty()
{
    return modelmatrix.isEmpty();
}




