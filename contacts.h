#ifndef CONTACTS_H
#define CONTACTS_H

#include <QObject>
#include <QVariant>
#include <qqml.h>
#include "contact.h"
class Contacts: public QObject
{
    Q_OBJECT
    Q_PROPERTY(std::vector<QString> contactList READ rContactList WRITE setContactList NOTIFY contactListChanged)
    //Q_PROPERTY(std::vector<Contact> contactListObj READ rContactListObj WRITE setContactListObj NOTIFY contactListObjChanged)
    Q_PROPERTY(std::vector<QVariantMap> contactListMap READ rContactListMap WRITE setContactListMap NOTIFY contactListMapChanged)
    //Q_PROPERTY(bool isNightMode READ isNightMode WRITE setIsNightMode NOTIFY isNightModeChanged)

public:
    void randomFunc();
    explicit Contacts(QObject *parent = nullptr);
    std::vector<QString> rContactList() const;
    void setContactList(const std::vector<QString> &newContactList);
    int isEqual(const std::vector<Contact> &newContactListObj);
    //void test();

//    std::vector<Contact> rContactListObj() const;
//    void setContactListObj(const std::vector<Contact> &newContactListObj);

    std::vector<QVariantMap> rContactListMap() const;
    void setContactListMap(const std::vector<QVariantMap> &newContactListMap);

signals:
    void contactListChanged();
    //void contactListObjChanged();

    void contactListMapChanged();

private:
    std::vector<QString> m_contactList;
    //std::vector<Contact> m_contactListObj;
    std::vector<QVariantMap> m_contactListMap;
};


#endif // CONTACTS_H
