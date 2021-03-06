#include "backlightsetting.h"

BacklightSetting::BacklightSetting(QObject *parent) :
    QObject(parent)
{
    m_automaticItem = new MGConfItem("/meego/ux/AutomaticBacklightControl", this);
    connect(m_automaticItem, SIGNAL(valueChanged()), this, SLOT(automaticItemUpdated()));

    if (m_automaticItem->value().isNull())
        m_automaticItem->set(true);

    automaticItemUpdated();

    m_manualValueItem = new MGConfItem("/meego/ux/ManualBacklightValue", this);
    connect(m_manualValueItem, SIGNAL(valueChanged()), this, SLOT(manualValueItemUpdated()));

    if (m_manualValueItem->value().isNull())
        m_manualValueItem->set(50);

    manualValueItemUpdated();

    m_screenSaverTimeoutItem = new MGConfItem("/meego/ux/ScreenSaverTimeout", this);
    connect(m_screenSaverTimeoutItem, SIGNAL(valueChanged()), this, SLOT(screenSaverTimeoutItemUpdated()));

    if (m_screenSaverTimeoutItem->value().isNull())
        m_screenSaverTimeoutItem->set(300);

    screenSaverTimeoutItemUpdated();
}

void BacklightSetting::setAutomatic(bool value)
{
    m_automatic = value;
    m_automaticItem->set(value);
}

void BacklightSetting::setManualValue(int value)
{
    m_manualValue = value;
    m_manualValueItem->set(value);
}

void BacklightSetting::setScreenSaverTimeout(int value)
{
    m_screenSaverTimeout = value;
    m_screenSaverTimeoutItem->set(value);
}

void BacklightSetting::automaticItemUpdated()
{
    m_automatic = m_automaticItem->value().toBool();
    emit automaticChanged();
}

void BacklightSetting::manualValueItemUpdated()
{
    m_manualValue = m_manualValueItem->value().toInt();
    emit manualValueChanged();
}

void BacklightSetting::screenSaverTimeoutItemUpdated()
{
    m_screenSaverTimeout = m_screenSaverTimeoutItem->value().toInt();
    emit screenSaverTimeoutChanged();
}
