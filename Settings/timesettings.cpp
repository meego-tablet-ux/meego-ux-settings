/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "timesettings.h"
#include <timed/interface>
#include <QDBusReply>
#include <QProcess>

TimeSettings::TimeSettings(QObject *parent) :
        QObject(parent)
{
    timed = new Maemo::Timed::Interface(this);

    connect(timed,SIGNAL(settings_changed(Maemo::Timed::WallClock::Info,bool)),this,
            SLOT(settingsChangedSlot(Maemo::Timed::WallClock::Info,bool)));
}

bool TimeSettings::flag24()
{
    QDBusReply<Maemo::Timed::WallClock::Info> reply = timed->get_wall_clock_info_sync();

    Maemo::Timed::WallClock::Info info = reply.value();

    m_flag24 = info.flagFormat24();

    return m_flag24;
}


void TimeSettings::setTime(QTime time)
{
    Maemo::Timed::WallClock::Settings set;
    set.setTimeManual(QDateTime(QDateTime::currentDateTime().date().currentDate(),time).toTime_t());
    qDebug() << "calling wall_clock_settings_sync" << set.str() ;
    QDBusReply<bool> reply1 = timed->wall_clock_settings_sync(set) ;
    qDebug() << "wall_clock_settings_sync done" ;
    if(!reply1.isValid())
    {
        qDebug() << "pid call failed" << timed->lastError() ;
    }
}

void TimeSettings::setDate(QDate date)
{
    Maemo::Timed::WallClock::Settings set;
    set.setTimeManual(QDateTime(date,QDateTime::currentDateTime().time().currentTime()).toTime_t());
    qDebug() << "calling wall_clock_settings_sync" << set.str() ;
    QDBusReply<bool> reply1 = timed->wall_clock_settings_sync(set) ;
    qDebug() << "wall_clock_settings_sync done" ;
    if(!reply1.isValid())
    {
        qDebug() << "pid call failed" << timed->lastError() ;
    }
}

bool TimeSettings::automatic()
{
    QDBusReply<Maemo::Timed::WallClock::Info> busReply = timed->get_wall_clock_info_sync();

    return busReply.value().flagTimeNitz();
}

void TimeSettings::setFlag24(bool value)
{
    Maemo::Timed::WallClock::Settings set;
    set.setFlag24(value);
    qDebug() << "calling wall_clock_settings_sync" << set.str() ;
    QDBusReply<bool> reply1 = timed->wall_clock_settings_sync(set) ;
    qDebug() << "wall_clock_settings_sync done" ;
    if(!reply1.isValid())
    {
        qDebug() << "pid call failed" << timed->lastError() ;
    }
}

void TimeSettings::setAutomatic(bool isAutomatic)
{
    Maemo::Timed::WallClock::Settings set;

    if(isAutomatic)
        set.setTimeNitz();
    qDebug() << "calling wall_clock_settings_sync" << set.str() ;
    QDBusReply<bool> reply1 = timed->wall_clock_settings_sync(set) ;
    qDebug() << "wall_clock_settings_sync done" ;
    if(!reply1.isValid())
    {
        qDebug() << "pid call failed" << timed->lastError() ;
    }
}

void TimeSettings::settingsChangedSlot(const Maemo::Timed::WallClock::Info &info, bool time_changed)
{
    settingsChanged();

    if(info.flagFormat24() != m_flag24)
    {
        m_flag24 = info.flagFormat24();
        flag24Changed(m_flag24);
    }

    if(info.flagTimeNitz() != m_flagTimeAutomatic)
    {
        m_flagTimeAutomatic = info.flagTimeNitz();
        automaticChanged(m_flagTimeAutomatic);
    }

    if(info.humanReadableTz()  != m_humanReadableTz)
    {
        m_humanReadableTz = info.humanReadableTz();
        timezoneChanged();
    }
}


QString TimeSettings::getHumanReadableTz()
{
    Maemo::Timed::WallClock::Info info;

    QDBusReply<Maemo::Timed::WallClock::Info> busReply = timed->get_wall_clock_info_sync();

    return busReply.value().humanReadableTz();
}


bool TimeSettings::setTz(QString newTz)
{
    Maemo::Timed::WallClock::Settings set;

    set.setTimezoneManual(newTz);

    QDBusReply<bool> busReply = timed->wall_clock_settings_sync(set) ;

    if(!busReply.isValid())
    {
        qDebug() << "pid call failed" << timed->lastError() ;
    }
    busReply = timed->wall_clock_settings_sync(set);

    return busReply.value();
}

bool TimeSettings::isUsingTzAuto()
{
    QDBusReply<Maemo::Timed::WallClock::Info> reply = timed->get_wall_clock_info_sync();
    Maemo::Timed::WallClock::Info info = reply.value();

    return  info.flagLocalCellular();
}

void TimeSettings::setTzAuto(bool useAuto)
{
    Maemo::Timed::WallClock::Settings set;

    if (useAuto){
        set.setTimezoneCellular("US/Pacific");
    }
    else{
        set.setTimezoneManual("Someplace/indaworld");
    }

    qDebug() << "calling wall_clock_settings_sync" << set.str() ;

    QDBusReply<bool> busReply = timed->wall_clock_settings_sync(set) ;

    qDebug() << "wall_clock_settings_sync done " << timed->lastError();
    if(!busReply.isValid())
    {
        qDebug() << "pid call failed" << timed->lastError() ;
    }
   // return busReply.isValid();
}
