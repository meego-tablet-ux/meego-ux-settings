#ifndef _SYNC_EVO_STORAGE_MODEL_H_
#define _SYNC_EVO_STORAGE_MODEL_H_

#include <QList>
#include "SyncEvoStorageModelItem.h"
#include "QProperty.h"

class QDBusServiceWatcher;
class OrgSyncevolutionServerInterface;

namespace MeeGo {

  namespace Sync {

    class SyncEvoStorageModel : public QAbstractListModel
    {
      Q_OBJECT

    Q_SIGNALS:
      void serviceHasDied();

    public:

      enum ProfileRoles {
        /// Display name of the sync profile.
        DisplayNameRole  = Qt::UserRole + 1,

        /// Sync profile Name/ID.
        NameRole,
        /// Service icon image associated with the sync profile.
        ImageRole,

        /// Whether or not the sync profile is has been scheduled
        /// for automatic syncing.
        ActiveRole,

        /// Calendar, Contacts, etc.
        StorageRole,
      };

      SyncEvoStorageModel(QObject *parent = 0);
      virtual ~SyncEvoStorageModel();

      // The usual list model methods.
      virtual int rowCount(QModelIndex const & /* parent */) const;
      virtual QVariant data(QModelIndex const & index, int role) const;

    private slots:
      void serviceOwnerChanged(const QString &name, const QString &oldOwner, const QString &newOwner);
      void asyncCallFinished(QDBusPendingCallWatcher *call);

    private:
      void maybeAddToList(const QString &configName, const QStringMultiMap &config, bool isTemplate);
      int getInsertIndex(const SyncEvoStorageModelItem &newItem, bool *replace, int *storageTypeIdx);

      QDBusServiceWatcher *m_watcher;
      OrgSyncevolutionServerInterface *m_serverInterface;
      bool m_error;
      QString m_serverDBusName;

      /*
       * The idea:
       *     
       * m_storageTypeIndices    m_l
       * ---------------------------------------
       * [ contacts ]------+      [ Google Contacts ]
       * [ calendar ]-----+|      [ Yahoo Contacts  ]
       * [ memo     ]----+|+----->[ Blah Contacts   ]
       * [ todo     ]---+||       [ Yahoo Calendar  ]
       *                ||+------>[ Blah Calendar   ]
       *                ||        [ Nix Memo        ]
       *                |+------->[ Jingles Memo    ]
       *                |         [ Dasher Todo     ]
       *                +-------->[ Prancer Todo    ]
       *s
       * m_storageTypeIndices contains one entry for each type of storage so far encountered. The name of the entry
       * is the name of the storage type (source/addressbook or source/calendar or etc.) and the value is the index of
       * the last element of m_l that is of that storage type.
       *
       * When inserting the storage of a given service, it is inserted at the index pointed to by m_storageTypeIndices.
       * Then, all the members of m_storageTypeIndices equal to and subsequent to the storage type are incremented.
       *
       * In case of a service with a never-before-encountered storage type, it is appended to m_l and a new entry is
       * added to m_storageTypeIndices pointed to the newly added m_l entry.
       *
       * Thus, the entries remain grouped by storage type. They will be in the order encountered, except the first one
       * shall become the last in its group.
       */
      QList< QProperty >m_storageTypeIndices;
      QList< SyncEvoStorageModelItem > m_l;
    };
  }
}

#endif /* !_SYNC_EVO_STORAGE_MODEL_H_ */
