#include "cellularsettings.h"
#include <QDBusInterface>
#include <QtDBus>

struct OfonoPropertyStruct {
    QDBusObjectPath path;
    QVariantMap properties;
};
typedef QList<OfonoPropertyStruct> OfonoItemList;

Q_DECLARE_METATYPE(OfonoPropertyStruct)
Q_DECLARE_METATYPE(OfonoItemList)

QDBusArgument &operator<<(QDBusArgument &argument, const OfonoPropertyStruct &modem)
{
    argument.beginStructure();
    argument << modem.path << modem.properties;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, OfonoPropertyStruct &modem)
{
    argument.beginStructure();
    argument >> modem.path >> modem.properties;
    argument.endStructure();
    return argument;
}

CellularSettings::CellularSettings(QObject *parent) :
    QObject(parent),context(NULL)
{
    OfonoModemManager manager;
    if(manager.modems().count() == 0 ) return;

    QString modem = manager.modems().at(0);
    QDBusInterface interface("org.ofono",modem,"org.ofono.ConnectionManager",QDBusConnection::systemBus());

    QDBusMessage request;

    qDBusRegisterMetaType<OfonoPropertyStruct>();
    qDBusRegisterMetaType<OfonoItemList>();

    QDBusReply<OfonoItemList> reply = interface.call("GetContexts");

    OfonoItemList contexts = reply.value();

    if(contexts.count() < 1)
    {
        qDebug()<<"CellularSettings::ERROR: could not get active context";
        return;
    }

    qDebug()<<"CellularSettings::Active context: "<<contexts.at(0).path.path();

    context = new ConnectionContext("org.ofono",contexts.at(0).path.path(),QDBusConnection::systemBus(),this);
}

QString CellularSettings::apn()
{
    if(!context) return "";
    QDBusReply<QVariantMap> reply = context->GetProperties();

    if(reply.error().isValid())
        qDebug()<<"CellularSettings::Error getting properties:"<<reply.error().message();

    QVariantMap props = reply.value();
    return props["AccessPointName"].toString();
}

QString CellularSettings::username()
{
    if(!context) return "";
    QDBusReply<QVariantMap> reply = context->GetProperties();

    if(reply.error().isValid())
        qDebug()<<"CellularSettings::Error getting properties:"<<reply.error().message();

    QVariantMap props = reply.value();

    return props["Username"].toString();
}

QString CellularSettings::password()
{
    if(!context) return "";
    QDBusReply<QVariantMap> reply = context->GetProperties();

    if(reply.error().isValid())
        qDebug()<<"CellularSettings::Error getting properties:"<<reply.error().message();

    QVariantMap props = reply.value();

    return props["Password"].toString();
}

void CellularSettings::setApn(QString ap, QString user, QString pass)
{
    if(!context) return;

    qDebug()<<"CellularSettings::setApn() with:"<<ap<<" "<<user<<" "<<pass;

    QDBusReply<void> reply;

    QDBusReply<QVariantMap> isactive = context->GetProperties();

    if(isactive.value()["Active"].toBool())
    {
        reply = context->SetProperty(QString("Active"), QDBusVariant(QVariant(false)));

        if(reply.error().isValid())
            qDebug()<<"CellularSettings::Error setting properties:"<<reply.error().message();
    }

    reply = context->SetProperty(QString("AccessPointName"),QDBusVariant(QVariant(ap)));

    if(reply.error().isValid())
        qDebug()<<"CellularSettings::Error setting AcessPointName properties:"<<reply.error().message();

    reply = context->SetProperty(QString("Username"),QDBusVariant(QVariant(user)));

    if(reply.error().isValid())
        qDebug()<<"CellularSettings::Error setting Username properties:"<<reply.error().message();

    reply = context->SetProperty(QString("Password"),QDBusVariant(QVariant(pass)));

    if(reply.error().isValid())
        qDebug()<<"CellularSettings::Error setting Password property:"<<reply.error().message();

    if(isactive.value()["Active"].toBool())
    {
        context->SetProperty(QString("Active"),QDBusVariant(QVariant(true)));

        if(reply.error().isValid())
            qDebug()<<"CellularSettings::Error setting Active property:"<<reply.error().message();
    }
}
