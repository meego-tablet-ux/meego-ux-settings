#ifndef _SYNC_EVO_STORAGE_MODEL_ITEM_H_
#define _SYNC_EVO_STORAGE_MODEL_ITEM_H_

#include <QString>
#include <syncevolution-qt-dbus/syncevo-server-full.h>

namespace MeeGo {

  namespace Sync {

    /*
     * Simple class containing a single storage model item
     */
    class SyncEvoStorageModelItem
    {
    public:
      SyncEvoStorageModelItem(const QString &name, const QString &storage, const QStringMap &header, const QStringMap &storageInfo, bool isTemplate);

      /// Name of config
      QString name() const;

      /// Friendly name of config
      QString displayName() const;

      /// Type of config, i.e. Calendar, Contacts, etc.
      QString storage() const;

      /// syncevolution config header
      QStringMap header() const;

      /// syncevolution source/? config section
      QStringMap storageInfo() const;

      /// Useful for debugging
      QString toString() const;

      /// Whether this is a template config
      bool isTemplate() const;

    private:

      QString m_name;
      QString m_storage;
      QStringMap m_header;
      QStringMap m_storageInfo;
      bool m_isTemplate;
    };
  }
}

#endif /* !_SYNC_EVO_STORAGE_MODEL_ITEM_H_ */
