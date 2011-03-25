#ifndef LOCALESETTINGS_H
#define LOCALESETTINGS_H

#include <QObject>
#include <QLocale>
#include <QStringList>
#include <QMap>

class LocaleSettings : public QObject
{
    Q_OBJECT
public:
    explicit LocaleSettings(QObject *parent = 0);

public slots:
    QStringList locales();

    void setLocale(QString);
    QString currentLocale();

private:

    QStringList m_localesStrings;
    QMap<QString, QLocale> m_locales;

};

#endif // LOCALESETTINGS_H
