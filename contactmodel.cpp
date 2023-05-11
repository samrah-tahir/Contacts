#include "contactmodel.h"
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


ContactModel *contact = nullptr;
QList<QVariantMap> contactsList;

ContactModel::ContactModel(QObject *parent) : QAbstractListModel(parent)
{
    QJniObject javaClass = QNativeInterface::QAndroidApplication::context();
    javaClass.callMethod<void>("test","(J)V",(long long)(ContactModel*)this);
}

int ContactModel::rowCount(const QModelIndex &parent) const{
    if(parent.isValid())
        return 0;

    return contactsList.size();
}

QVariant ContactModel::data(const QModelIndex &index, int role) const{
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

QHash <int, QByteArray> ContactModel::roleNames() const {
    QHash<int, QByteArray> roleNames;
    roleNames[NameRole] = "contactName";
    roleNames[PhoneNumRole] = "contactNumber";
    return roleNames;
}

void ContactModel::addContact(std::list<QVariantMap> contacts){

    for (const QVariantMap &contact : contacts) {
        contactsList.push_back(contact);
    }

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



        QVariantMap contactMap;
        contactMap.insert("contactName", QVariant(jsonObj["name"].toString()));
        contactMap.insert("contactNumber",  QVariant(jsonObj["number"].toString()));

        contactsMapList.push_back(contactMap);


    }



    contact->addContact(contactsMapList);


    ContactModel* contactItems = reinterpret_cast<ContactModel*>(ptr);

}

}


