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
#include "contact.h"

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

    //std::vector<Contact> contactsVector;
    std::vector<QString> contactsVector;

    for(int  i = 0; i < arr.count(); ++i){
        QJsonObject jsonObj = arr.at(i).toObject();
        //Contact contact(jsonObj["name"].toString(), jsonObj["number"].toString());
        //contactsVector.push_back(contact);
        contactsVector.push_back(jsonObj["name"].toString());
        contactsVector.push_back(jsonObj["number"].toString());
        //qDebug() << contact.contactName;
    }




    Contacts* contactItems = reinterpret_cast<Contacts*>(ptr);

    //contactItems->setContactListObj(contactsVector);
    contactItems->setContactList(contactsVector);

}

}

std::vector<QString> Contacts::rContactList() const
{
    return m_contactList;
}

void Contacts::setContactList(const std::vector<QString> &newContactList)
{
    if (m_contactList == newContactList)
        return;
    m_contactList = newContactList;
    emit contactListChanged();
}


//ignored for now
std::vector<Contact> Contacts::rContactListObj() const
{
    return m_contactListObj;
}



void Contacts::setContactListObj(const std::vector<Contact> &newContactListObj)
{
//    if (isEqual(newContactListObj) == true)
//        return;
    m_contactListObj = newContactListObj;
    //qDebug() << m_contactListObj[0].contactName;
    emit contactListObjChanged();
}

int Contacts::isEqual(const std::vector<Contact> &newContactListObj){
    int isEqualObj = true;

    for(int i = 0; i < m_contactListObj.size(); i++){

        if(m_contactListObj[i].contactName == newContactListObj[i].contactName && m_contactListObj[i].contactNumber == newContactListObj[i].contactNumber){
            isEqualObj = true;
        }
        else{
            isEqualObj = false;
            return isEqualObj;
        }

    }
    return isEqualObj;
}
