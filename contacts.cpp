#include "contacts.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QJniObject>
#include <iostream>
#include <QQmlContext>
#include <QDebug>
#include <string>
#include <QVariant>
#include <QObject>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


Contacts *contact = nullptr;

Contacts::Contacts(QObject *parent) : QAbstractListModel(parent)
{
    QJniObject javaClass = QNativeInterface::QAndroidApplication::context();
    javaClass.callMethod<void>("test","(J)V",(long long)(Contacts*)this);
}

int Contacts::rowCount(const QModelIndex &parent) const{
    if(parent.isValid())
        return 0;

    return contactsList.size();
}

QVariant Contacts::data(const QModelIndex &index, int role) const{
    if(!index.isValid())
        return QVariant();
    if(role == NameRole){
        return contactsList[index.row()].value("contactName");
    }
    else if(role == PhoneNumRole){
        return contactsList[index.row()].value("contactNumber");
    }

    return QVariant();
}

QHash <int, QByteArray> Contacts::roleNames() const {
    static QHash<int, QByteArray> mapping {

        {NameRole, "contactName"},
        {PhoneNumRole, "contactNumber"},
        };
    return mapping;
}

void Contacts::addContact(QVariantMap contact){

//    for (const QVariantMap &contact : contacts) {
//        contactsList.append(contact);

//    }

    contactsList.append(contact);
    qDebug() << contactsList.at(0);

}

extern "C" {

JNIEXPORT void JNICALL Java_com_example_contactsdisplay_MainActivity_displayContacts
    (JNIEnv *env, jobject obj, jstring jsonContactsStr, jlong ptr){


    const char *cStr = env->GetStringUTFChars(jsonContactsStr, 0);

    QJsonDocument doc = QJsonDocument::fromJson(cStr);
    QJsonArray arr = doc.array();


    std::list<QVariantMap> contactsMapList;

    for(int  i = 0; i < arr.count(); ++i){
        QJsonObject jsonObj = arr.at(i).toObject();
        //Contact contact(jsonObj["name"].toString(), jsonObj["number"].toString());
        //contacts2Vector.push_back(contact);
        QVariantMap contactMap;


        contactMap.insert("contactName", jsonObj["name"].toString());
        contactMap.insert("contactNumber", jsonObj["number"].toString());


        contactsMapList.push_back(contactMap);

        qDebug() << contactMap;
        //contact->addContact(contactMap);
        //qDebug() << contact.contactName;
    }





    Contacts* contactItems = reinterpret_cast<Contacts*>(ptr);
    //contact->addContact(contactsMapList);
    //contactItems->setContactListMap(contactsMapList);

    //qDebug() << contactItems->rContactListMap()[0].value("contactName");
}

}



//ignored for now
//std::vector<Contact> Contacts::rContactListObj() const
//{
//    return m_contactListObj;
//}



//void Contacts::setContactListObj(const std::vector<Contact> &newContactListObj)
//{
//    if (isEqual(newContactListObj) == true)
//        return;
//    m_contactListObj = newContactListObj;
//    //qDebug() << m_contactListObj[0].contactName;
//    emit contactListObjChanged();
//}

//int Contacts::isEqual(const std::vector<Contact> &newContactListObj){
//    int isEqualObj = true;

//    for(int i = 0; i < m_contactListObj.size(); i++){

//        if(m_contactListObj[i].contactName == newContactListObj[i].contactName && m_contactListObj[i].contactNumber == newContactListObj[i].contactNumber){
//            isEqualObj = true;
//        }
//        else{
//            isEqualObj = false;
//            return isEqualObj;
//        }

//    }
//    return isEqualObj;
//}

std::vector<QVariantMap> Contacts::rContactListMap() const
{
    return m_contactListMap;
}

void Contacts::setContactListMap(const std::vector<QVariantMap> &newContactListMap)
{
    if (m_contactListMap == newContactListMap)
        return;
    m_contactListMap = newContactListMap;
    emit contactListMapChanged();
}
