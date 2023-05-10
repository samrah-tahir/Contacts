#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include <QAbstractListModel>


class ContactModel: public QAbstractListModel
{
    Q_OBJECT


public:
    explicit ContactModel(QObject *parent = nullptr);

    QList<QVariantMap> contactsList;
};

#endif // CONTACTMODEL_H
