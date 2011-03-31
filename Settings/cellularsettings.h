#ifndef CELLULARSETTINGS_H
#define CELLULARSETTINGS_H

#include <QObject>
#include <QString>
#include <ofonomodemmanager.h>

#include "connectioncontext.h"

class CellularSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString apn READ apn WRITE setApn NOTIFY apnChanged)

public:
    explicit CellularSettings(QObject *parent = 0);

public slots:
    void setApn(QString apn, QString username, QString password);
    void setApn(QString apn);

    QString apn();
    QString username();
    QString password();

    QStringList countries();
    QStringList providers(QString country);
    QStringList apns(QString country, QString provider);

    void propertyChanged(QString,QDBusVariant);

signals:
    void apnChanged();

private:
    ConnectionContext *context;
};

#endif // CELLULARSETTINGS_H
