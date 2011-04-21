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

    QStringList layouts();
    void setLayout(QString);
    QString currentLayout();

    QStringList dateFormats();
    void setDateFormat(QString);
    QString currentDateFormat();

    QStringList timeFormats();
    void setTimeFormat(QString);
    QString currentTimeFormat();

    QStringList numberFormats();
    void setNumberFormat(QString);
    QString currentNumberFormat();
    
private:
    void loadLayouts();

private:

    QStringList m_localesStrings;
    QStringList m_layoutsStrings;
    QStringList m_dateFormats;
    QStringList m_timeFormats;
    QStringList m_numberFormats;

    QMap<QString, QString> m_layoutsByTitle;
    QMap<QString, QLocale> m_locales;

};

#endif // LOCALESETTINGS_H
