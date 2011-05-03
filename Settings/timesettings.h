/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef TIMESETTINGS_H
#define TIMESETTINGS_H

#include <QObject>
#include <QDateTime>
#include <QDebug>
#include <QLocale>

namespace Maemo
{
    namespace Timed
    {
        class Interface;

        namespace WallClock
        {
            class  Info;
        }
    }
}

class TimeSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool flag24 READ flag24 WRITE setFlag24 NOTIFY flag24Changed)
    Q_PROPERTY(bool automatic READ automatic WRITE setAutomatic NOTIFY automaticChanged)
    Q_PROPERTY(bool automaticTimeZone READ isUsingTzAuto WRITE setTzAuto NOTIFY automaticTimeZoneChanged)
    Q_PROPERTY(QString timezone READ getHumanReadableTz WRITE setTz NOTIFY timezoneChanged)

public:
    explicit TimeSettings(QObject *parent = 0);

    bool flag24();

signals:
    void settingsChanged();
    void flag24Changed(bool);
    void automaticChanged(bool);
    void automaticTimeZoneChanged(bool);
    void timezoneChanged();

public slots:

    QDateTime currentDateTime() { return QDateTime::currentDateTime(); }

    QString currentDate()
    {
        QString date = QDateTime::currentDateTime().date().currentDate().toString(
                QLocale::system().dateFormat(QLocale::LongFormat));
        qDebug()<<"date: " <<date;
        return date;
    }
    QString currentTime()
    {
        return QDateTime::currentDateTime().time().currentTime().toString(
                QLocale::system().timeFormat(QLocale::ShortFormat));
    }

    QString currentHour()
    {
        return QString::number(QDateTime::currentDateTime().time().currentTime().hour());
    }

    QString currentMinute()
    {
        return QString::number(QDateTime::currentDateTime().time().currentTime().minute());
    }

    QTime time(QString h, QString m, QString s) { return QTime(h.toInt(),m.toInt(),s.toInt()); }
    QDate date(QString d, QString m, QString y) { return QDate(y.toInt(),m.toInt(),d.toInt()); }

    void setTime(QTime time);
    void setDate(QDate date);
    bool automatic();

    void setFlag24(bool);
    void setAutomatic(bool isAutomatic);

    void settingsChangedSlot(const Maemo::Timed::WallClock::Info &info, bool time_changed);

    QString getHumanReadableTz();

    QString getTzAbbreviation();

    bool setTz(QString newTz);

    bool isUsingTzAuto();

    void setTzAuto(bool useAuto);

private:
    Maemo::Timed::Interface *timed;
    bool m_flag24;
    bool m_flagTimeAutomatic;
    QString m_humanReadableTz;
};

#endif // TIMESETTINGS_H
