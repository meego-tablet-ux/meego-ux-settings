#include "localesettings.h"
#include <locale.h>

#include <QLocale>
#include <QDebug>
#include <QDate>
#include <QSettings>
#include <QDir>

#include <QDBusInterface>
#include <QDBusReply>
#include <MGConfItem>
#include <minputmethodnamespace.h>

const QString VKBConfigurationPath("/usr/share/meegotouch/virtual-keyboard/layouts/");
const QString VKBLayoutsFilterRule("*.xml");
const QString VKBLayoutsIgnoreRules("symbols|number|test|customer|default");

LocaleSettings::LocaleSettings(QObject *parent) :
    QObject(parent)
{
    for(int i=0; i<215; i++)
    {
        QString language = QLocale::languageToString((QLocale::Language)i);
        if(language != "")
        {
            foreach(QLocale::Country country,QLocale::countriesForLanguage((QLocale::Language)i))
            {
                QString localeString = language + " - " + QLocale::countryToString(country);
                m_localesStrings<<localeString;

                m_locales[localeString] = QLocale((QLocale::Language)i, country);
            }
        }
    }

    loadLayouts();
}

void LocaleSettings::loadLayouts()
{
    QStringList layoutFiles;
    QDir dir(VKBConfigurationPath, VKBLayoutsFilterRule);
    QRegExp ignoreExp(VKBLayoutsIgnoreRules, Qt::CaseInsensitive);

    foreach (const QFileInfo &keyboardFileInfo, dir.entryInfoList()) {
        if (keyboardFileInfo.fileName().contains(ignoreExp))
            continue;
        layoutFiles << keyboardFileInfo.fileName();
    }

    MGConfItem layoutsItem("/meegotouch/inputmethods/virtualkeyboard/layouts");
    layoutsItem.set(QVariant(layoutFiles));//

    if(!QDBusConnection::sessionBus().isConnected())
        return;

    QDBusInterface iface("com.meego.inputmethodpluginmanager1",
                         "/com/meego/inputmethodpluginmanager1");

    if(!iface.isValid())
        return;

    QDBusReply<QStringList> reply = iface.call("queryAvailablePlugins", MInputMethod::OnScreen);
    if(!reply.isValid())
        return;

    if(!reply.value().contains("MeegoKeyboard"))
        return;

    QDBusReply<QMap<QString,QVariant> > subViews = iface.call("queryAvailableSubViews", "MeegoKeyboard", MInputMethod::OnScreen);
    if(!subViews.isValid())
        return;

    QMapIterator<QString,QVariant> it(subViews);
    while(it.hasNext()){
        it.next();
        QString layoutTitle = it.value().toString();

        m_layoutsByTitle[layoutTitle] = it.key();
        m_layoutsStrings << layoutTitle;
    }
 }

QStringList LocaleSettings::locales()
{
    return m_localesStrings;
}

void LocaleSettings::setLocale(QString locale)
{
    Q_ASSERT(m_locales.contains(locale));

    ///Set the current process's locale:
    qDebug()<<"setting local to "<<locale<<" or "<<m_locales[locale].name();
    qDebug()<<"from "<<QLocale::system().name();

    QLocale::setDefault(m_locales[locale]);

    QByteArray envvar = QLocale().name().toAscii() + ".utf8";

    setenv("LANG",envvar.data(),1);
    setenv("LC_ALL",envvar.data(),1);

    if(!QFile::exists(QDir::homePath() + "/.config/sysconfig/i18n"))
    {
        if(!QDir::home().exists(QDir::homePath() + "/.config/sysconfig"))
        {
            QDir::home().mkpath(".config/sysconfig");
        }

        QFile::copy("/etc/sysconfig/i18n", QDir::homePath() + "/.config/sysconfig/i18n");
    }

    QSettings i18n(QDir::homePath() + "/.config/sysconfig/i18n", QSettings::NativeFormat, this);
    i18n.setValue("LANG",QLocale().name() + ".UTF-8");
}

QString LocaleSettings::currentLocale()
{
    QLocale current = QLocale::system();

    return QLocale::languageToString(current.language()) + " - " + QLocale::countryToString(current.country());
}


QStringList LocaleSettings::layouts()
{
    return m_layoutsStrings;
}

void LocaleSettings::setLayout(QString layoutTitle)
{
    if(!m_layoutsByTitle.contains(layoutTitle))
        return;

    QString layoutId = m_layoutsByTitle.value(layoutTitle);

    if(!QDBusConnection::sessionBus().isConnected())
        return;

    QDBusInterface iface("com.meego.inputmethodpluginmanager1",
                         "/com/meego/inputmethodpluginmanager1");

    if(!iface.isValid())
        return;

    iface.call("setActiveSubView", layoutId, MInputMethod::OnScreen);
}

QString LocaleSettings::currentLayout()
{
    if(!QDBusConnection::sessionBus().isConnected())
        return "";

    QDBusInterface iface("com.meego.inputmethodpluginmanager1",
                         "/com/meego/inputmethodpluginmanager1");

    QDBusReply<QMap<QString,QVariant> > activeSubViews = iface.call("queryActiveSubView", MInputMethod::OnScreen);
    if(!activeSubViews.isValid())
        return "";

    if(activeSubViews.value().count() != 1)
        return "";

    QString layoutId = activeSubViews.value().keys().first();

    QMapIterator<QString,QString> it(m_layoutsByTitle);
    while(it.hasNext()){
        it.next();
        if(it.value() == layoutId)
            return it.key();
    }

    return "";
}

QStringList LocaleSettings::dateFormats()
{
    m_dateFormats.clear();

    QLocale locale = QLocale::system();

    m_dateFormats << locale.toString(QDate::currentDate(), QLocale::LongFormat);
    m_dateFormats << locale.toString(QDate::currentDate(), QLocale::ShortFormat);
    m_dateFormats << locale.toString(QDate::currentDate(), QLocale::NarrowFormat);

    return m_dateFormats;
}

void LocaleSettings::setDateFormat(QString)
{

}

QString LocaleSettings::currentDateFormat()
{
    // this is just for test !!!
    return m_dateFormats.first();
}

QStringList LocaleSettings::timeFormats()
{
    m_timeFormats.clear();

    QLocale locale = QLocale::system();

    m_timeFormats << locale.toString(QTime::currentTime(), QLocale::LongFormat);
    m_timeFormats << locale.toString(QTime::currentTime(), QLocale::ShortFormat);
    m_timeFormats << locale.toString(QTime::currentTime(), QLocale::NarrowFormat);

    return m_timeFormats;
}

void LocaleSettings::setTimeFormat(QString)
{

}

QString LocaleSettings::currentTimeFormat()
{
    // this is just for test !!!
    return m_timeFormats.first();
}

QStringList LocaleSettings::numberFormats()
{
    double dbl = 1234.5678;
    m_numberFormats.clear();

    QLocale locale = QLocale::system();

    locale.setNumberOptions(QLocale::OmitGroupSeparator);
    m_numberFormats << locale.toString(dbl);

    locale.setNumberOptions(QLocale::RejectGroupSeparator);
    m_numberFormats << locale.toString(dbl);

    return m_numberFormats;
}

void LocaleSettings::setNumberFormat(QString)
{

}

QString LocaleSettings::currentNumberFormat()
{
    // this is just for test !!!
    return m_numberFormats.first();
}

