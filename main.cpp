#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QJniObject>
#include <iostream>
#include <string>
#include <QQmlContext>
#include <QVariant>
#include "contacts.h"

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/ContactsDisplay/Main.qml"_qs);

    qmlRegisterType<Contacts>("Contactspackage",1,0,"Contacts");

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
