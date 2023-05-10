#include <string>
#include <QString>
#ifndef CONTACT_H
#define CONTACT_H


class Contact
{
public: QString contactName;
public: QString contactNumber;

public:
    Contact();
public: Contact(const QString &name, const QString &number);

};

#endif // CONTACT_H
