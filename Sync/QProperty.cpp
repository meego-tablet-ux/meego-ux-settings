#include "QProperty.h"

QProperty::QProperty(const QString &name, const QVariant &value)
  : QString(name)
  , m_value(value)
{
}

QVariant
QProperty::value() const
{
  return m_value; 
}

void
QProperty::set(const QVariant &newValue)
{
  m_value = newValue ;
}
