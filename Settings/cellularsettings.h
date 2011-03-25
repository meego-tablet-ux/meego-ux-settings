#ifndef CELLULARSETTINGS_H
#define CELLULARSETTINGS_H

#include <QObject>
#include <QString>
#include <ofonomodemmanager.h>

#include "connectioncontext.h"

class CellularSettings : public QObject
{
    Q_OBJECT

public:
    explicit CellularSettings(QObject *parent = 0);

public slots:
    void setApn(QString apn, QString username, QString password);

    QString apn();
    QString username();
    QString password();

private:
    ConnectionContext *context;
};

#endif // CELLULARSETTINGS_H
