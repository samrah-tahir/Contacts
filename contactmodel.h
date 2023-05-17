#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include <QObject>
#include <QVariant>
#include <qqml.h>
#include <QAbstractListModel>

class ContactModel: public QAbstractListModel
{
    Q_OBJECT
    QList<QVariantMap> contactsList;

public:

    explicit ContactModel(QObject *parent = nullptr);

    enum{
        NameRole = Qt::UserRole,
        PhoneNumRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex() ) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    QHash<int, QByteArray> roleNames() const override;

    void addContact(std::list<QVariantMap> contacts, int);

    void removeContact(QString contact_id);

    static bool compare_map(const QVariantMap &a, const QVariantMap &b);

    Q_INVOKABLE bool removeRows(int row, int count, const QModelIndex &parent=QModelIndex()) override;
};


#endif // CONTACTMODEL_H
