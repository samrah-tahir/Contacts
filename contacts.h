#ifndef CONTACTS_H
#define CONTACTS_H

#include <QObject>
#include <QVariant>
#include <qqml.h>
#include <QAbstractListModel>

class Contacts: public QAbstractListModel
{
    Q_OBJECT

   // Q_PROPERTY(std::vector<QVariantMap> contactListMap READ rContactListMap WRITE setContactListMap NOTIFY contactListMapChanged)


public:

    explicit Contacts(QObject *parent = nullptr);

    enum{
        NameRole = Qt::UserRole,
        PhoneNumRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex() ) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    void addContact(std::list<QVariantMap> contacts);
    //QList<QVariantMap> contactsList;

};


#endif // CONTACTS_H
