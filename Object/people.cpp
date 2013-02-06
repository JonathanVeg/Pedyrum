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


 #include "people.h"

People::People()
{
}

void People::setComplement(QString complement) {
    this->complement = complement;
}

void People::setDistrict(QString district) {
    this->district = district;
}

void People::setId(int id) {
    this->id = id;
}

void People::setName(QString name) {
    this->name = name;
}

void People::setNumber(QString number) {
    this->number = number;
}

void People::setPhone(QString phone) {
    this->phone = phone;
}

void People::setStreet(QString street) {
    this->street = street;
}

void People::setNote(QString observation){
    this->note = observation;
}

void People::setCEP(QString cep){
    this->cep = cep;
}

int People::getId(){
    return id;
}

QString People::getName(){
    return name;
}

QString People::getPhone()
{
    return phone;
}

QString People::getStreet()
{
    return street;
}

QString People::getDistrict()
{
    return district;
}

QString People::getNumber()
{
    return number;
}

QString People::getComplement()
{
    return complement;
}

QString People::getCEP(){
    return cep;
}

QString People::getNote(){
    return note;
}
