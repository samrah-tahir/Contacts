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


Contacts::Contacts(QObject *parent) : QObject{parent}
{
    QJniObject javaClass = QNativeInterface::QAndroidApplication::context();
    javaClass.callMethod<void>("test","(J)V",(long long)(Contacts*)this);
}

extern "C" {

JNIEXPORT void JNICALL Java_com_example_contactsdisplay_MainActivity_displayContacts
    (JNIEnv *env, jobject obj, jstring jsonContactsStr, jlong ptr){


    const char *cStr = env->GetStringUTFChars(jsonContactsStr, 0);

    QJsonDocument doc = QJsonDocument::fromJson(cStr);
    QJsonArray arr = doc.array();


    std::vector<QVariantMap> contactsMapVector;

    for(int  i = 0; i < arr.count(); ++i){
        QJsonObject jsonObj = arr.at(i).toObject();
        //Contact contact(jsonObj["name"].toString(), jsonObj["number"].toString());
        //contacts2Vector.push_back(contact);
        QVariantMap contactMap;

        contactMap.insert("contactName", jsonObj["name"].toString());
        contactMap.insert("contactNumber", jsonObj["number"].toString());

        contactsMapVector.push_back(contactMap);

        //qDebug() << contact.contactName;
    }




    Contacts* contactItems = reinterpret_cast<Contacts*>(ptr);

    contactItems->setContactListMap(contactsMapVector);

    qDebug() << contactItems->rContactListMap()[0].value("contactName");
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
