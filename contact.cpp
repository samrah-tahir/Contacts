#include "contact.h"
#include <string>
#include <QString>

Contact::Contact()
{

}

Contact::Contact(const QString &name, const QString &number){
    contactName = name;
    contactNumber = number;
}
