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
#include <string>


QJniObject javaClass = QNativeInterface::QAndroidApplication::context();
ContactModel::ContactModel(QObject *parent) : QAbstractListModel(parent)
{

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
        //add updated contact
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

           //if no such contact found in the contactsList then its a new contact
        if(!found){
            qDebug() << "new contact added";
            beginInsertRows(QModelIndex(), i, i);
            for(QVariantMap &newContact: contacts){
                contactsList.append(newContact);
                //std::sort(contactsList.begin(),contactsList.end(), compare_map);

            }
            endInsertRows();

            beginResetModel();
            std::sort(contactsList.begin(),contactsList.end(), compare_map);
            endResetModel();
        }

        }

    }

bool compare_maps(const QVariantMap &a, const QVariantMap &b){

        QString key = "contactName";

        if(a.contains(key) && b.contains(key)){
        QString aValue = a[key].toString();
        QString bValue = b[key].toString();

        return aValue < bValue;
        }

        return false;
}

bool ContactModel::compare_map(const QVariantMap &a, const QVariantMap &b){

        QString key = "contactName";

        if(a.contains(key) && b.contains(key)){
        QString aValue = a[key].toString();
        QString bValue = b[key].toString();

        return aValue < bValue;
        }

        return false;
}

void ContactModel::removeContact(QString contact_id){


        for(int i = 0; i < contactsList.size(); i++){
            if(contactsList.at(i)["contactID"] == contact_id){

                beginRemoveRows(QModelIndex(), i, i);
                contactsList.removeAt(i);
                endRemoveRows();
            }
        }

}

bool ContactModel::removeRows(int row, int count, const QModelIndex &parent){

        jint isSuccess = javaClass.callMethod<int>("deleteContact","(I)I", contactsList.at(row)["contactID"].toInt());

        if(!isSuccess){ //if contact successfully removed from android contactDB
            return false;
        }

        if(row < rowCount()){
            beginRemoveRows(parent, row, row);
            contactsList.removeAt(row);
            endRemoveRows();
        }
        else {
            return false;
        }

        return true;
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



    contactsMapList.sort(compare_maps);

    ContactModel* contactItems = reinterpret_cast<ContactModel*>(ptr);
    contactItems->addContact(contactsMapList, 0);


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

}


extern "C" {

JNIEXPORT void JNICALL Java_com_example_contactsdisplay_MainActivity_removeContact
    (JNIEnv *env, jobject, jstring deletedContactJson, jlong ptr){

    const char *contactID = env->GetStringUTFChars(deletedContactJson, 0);

    QJsonDocument doc = QJsonDocument::fromJson(contactID);
    QJsonArray arr = doc.array();

    ContactModel* contactItems = reinterpret_cast<ContactModel*>(ptr);

    for(int  i = 0; i < arr.count(); ++i){
        QJsonObject jsonObj = arr.at(i).toObject();
        QVariantMap contactMap;
        contactMap.insert("contactID", QVariant(jsonObj["contact_id"].toString()));
        contactItems->removeContact(contactMap["contactID"].toString());

    }
}

}
}
