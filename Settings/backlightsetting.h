#ifndef BACKLIGHTSETTING_H
#define BACKLIGHTSETTING_H

#include <QObject>
#include <MGConfItem>

class BacklightSetting : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool automatic READ getAutomatic WRITE setAutomatic NOTIFY automaticChanged)
    Q_PROPERTY(int manualValue READ getManualValue WRITE setManualValue NOTIFY manualValueChanged)
    Q_PROPERTY(int screenSaverTimeout READ getScreenSaverTimeout WRITE setScreenSaverTimeout NOTIFY screenSaverTimeoutChanged)
public:
    explicit BacklightSetting(QObject *parent = 0);

    bool getAutomatic() {
        return m_automatic;
    }
    void setAutomatic(bool value);

    int getManualValue() {
        return m_manualValue;
    }
    void setManualValue(int value);

    int getScreenSaverTimeout() {
        return m_screenSaverTimeout;
    }
    void setScreenSaverTimeout(int value);

signals:
    void automaticChanged();
    void manualValueChanged();
    void screenSaverTimeoutChanged();

public slots:
    void automaticItemUpdated();
    void manualValueItemUpdated();
    void screenSaverTimeoutItemUpdated();

private:
    MGConfItem *m_automaticItem;
    MGConfItem *m_manualValueItem;
    MGConfItem *m_screenSaverTimeoutItem;
    bool m_automatic;
    int m_manualValue;
    int m_screenSaverTimeout;
};

#endif // BACKLIGHTSETTING_H
