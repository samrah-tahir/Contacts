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


Contacts::Contacts(QObject *parent) : QObject{parent}
{
    QJniObject javaClass = QNativeInterface::QAndroidApplication::context();
    javaClass.callMethod<void>("test","(J)V",(long long)(Contacts*)this);
}

extern "C" {

JNIEXPORT void JNICALL Java_com_example_contactsdisplay_MainActivity_displayContacts
    (JNIEnv *env, jclass, jobject arraylistObj, jlong ptr){
    jclass cList = env->FindClass("java/util/List");
    jmethodID mSize = env->GetMethodID(cList, "size", "()I");
    jmethodID mGet = env->GetMethodID(cList, "get", "(I)Ljava/lang/Object;");

    // get the size of the list
    jint size = env->CallIntMethod(arraylistObj, mSize);

    std::vector<QString> contactsVector;

    for(jint i=0;i<size;i++) {
        jstring strObj = (jstring)env->CallObjectMethod(arraylistObj, mGet, i);
        const char * chr = env->GetStringUTFChars(strObj, NULL);
        contactsVector.push_back(chr);
        //qDebug() << chr;
        env->ReleaseStringUTFChars(strObj, chr);
    }

    Contacts* contactItems = reinterpret_cast<Contacts*>(ptr);

    contactItems->setContactList(contactsVector);

    //qDebug() << size;
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

