#include "contactmodel.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QJniObject>
#include <QDebug>
#include <QVariant>
#include <QObject>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


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

bool ContactModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(data(index, role) != value){
        //qDebug() << value;
        if(role == NameRole){
            contactsList[index.row()]["contactName"] = value;
        }
        else if(role == PhoneNumRole){
            contactsList[index.row()]["contactNumber"] = value;
        }
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

QHash <int, QByteArray> ContactModel::roleNames() const {
    QHash<int, QByteArray> roleNames;
    roleNames[NameRole] = "contactName";
    roleNames[PhoneNumRole] = "contactNumber";
    return roleNames;
}

void ContactModel::addContact(std::list<QVariantMap> contacts, int updated){
    int i = 0;
    if(updated == 0){
        for (const QVariantMap &contact : contacts) {
            contactsList.push_back(contact);
        }
    }
    else if (updated == 1) {

        bool found = false;

        for(QVariantMap &contact: contactsList){
            for(QVariantMap &newContact: contacts){
                if(contact.value("contactID") == newContact.value("contactID")){
                   found = true;
                   const auto contactIndex = i;
                   QModelIndex modelIndex = index(contactIndex, 0, QModelIndex());
                   setData(modelIndex, newContact.value("contactName"), NameRole);
                   setData(modelIndex, newContact.value("contactNumber"), PhoneNumRole);
                }
            }

            i++;

           }

        if(!found){
            qDebug() << "new contact added";
            beginInsertRows(QModelIndex(), i, i);
            for(QVariantMap &newContact: contacts)
                contactsList.append(newContact);
            endInsertRows();
        }

        }

    }



extern "C" {

JNIEXPORT void JNICALL Java_com_example_contactsdisplay_MainActivity_displayContacts
    (JNIEnv *env, jobject obj, jstring jsonContactsStr, jlong ptr){

    int updated = 0;

    const char *cStr = env->GetStringUTFChars(jsonContactsStr, 0);

    QJsonDocument doc = QJsonDocument::fromJson(cStr);
    QJsonArray arr = doc.array();


    std::list<QVariantMap> contactsMapList;

    for(int  i = 0; i < arr.count(); ++i){
        QJsonObject jsonObj = arr.at(i).toObject();

        QVariantMap contactMap;
        contactMap.insert("contactName", QVariant(jsonObj["name"].toString()));
        contactMap.insert("contactNumber",  QVariant(jsonObj["number"].toString()));
        contactMap.insert("contactID", QVariant(jsonObj["id"].toString()));

        contactsMapList.push_back(contactMap);


    }

    ContactModel* contactItems = reinterpret_cast<ContactModel*>(ptr);
    contactItems->addContact(contactsMapList, 0);

    //qDebug() << contactsMapList;
}

}

extern "C" {
JNIEXPORT void JNICALL Java_com_example_contactsdisplay_MainActivity_getUpdatedContacts
    (JNIEnv *env, jobject, jstring updatedContactJson, jlong ptr){

    int updated = 1;

    const char *cStr = env->GetStringUTFChars(updatedContactJson, 0);

    QJsonDocument doc = QJsonDocument::fromJson(cStr);
    QJsonArray arr = doc.array();

    std::list<QVariantMap> contactsMapList;
    for(int  i = 0; i < arr.count(); ++i){
        QJsonObject jsonObj = arr.at(i).toObject();
        QVariantMap contactMap;
        contactMap.insert("contactName", QVariant(jsonObj["name"].toString()));
        contactMap.insert("contactNumber",  QVariant(jsonObj["number"].toString()));
        contactMap.insert("contactID", QVariant(jsonObj["id"].toString()));

        contactsMapList.push_back(contactMap);


    }

    ContactModel* contactItems = reinterpret_cast<ContactModel*>(ptr);
    contactItems->addContact(contactsMapList, 1);

    //qDebug() << contactsMapList;
}

}
