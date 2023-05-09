#ifndef CONTACTS_H
#define CONTACTS_H

#include <QObject>
#include <qqml.h>

class Contacts: public QObject
{
    Q_OBJECT
    Q_PROPERTY(std::vector<QString> contactList READ rContactList WRITE setContactList NOTIFY contactListChanged)
    //Q_PROPERTY(bool isNightMode READ isNightMode WRITE setIsNightMode NOTIFY isNightModeChanged)

public:
    void randomFunc();
    explicit Contacts(QObject *parent = nullptr);
    std::vector<QString> rContactList() const;
    void setContactList(const std::vector<QString> &newContactList);
    //void test();

signals:
    void contactListChanged();
private:
    std::vector<QString> m_contactList;
};


#endif // CONTACTS_H
