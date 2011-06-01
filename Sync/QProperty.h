#ifndef _Q_PROPERTY_H_
#define _Q_PROPERTY_H_

#include <QString>
#include <QVariant>

/*
 * Class to store Qt properties for D-Bus annotation.
 */
class QProperty : public QString
{
  public:
    QProperty(const QString &name, const QVariant &value = QVariant());

    QVariant value() const;

    void set(const QVariant &newValue);

  private:
    QVariant m_value;
};

#endif /* !_Q_PROPERTY_H_ */
