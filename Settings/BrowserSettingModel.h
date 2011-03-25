/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */


//
// Filename:
//    BrowserSettingModel.h
//
// Description:
//   Provide a data model for browser settings which can be
//   retrieved from GConf. 
//
// Author:
//   hongbo.min@intel.com
// 
// Date:
//   Dec 2, 2010

#ifndef _BROWSER_SETTING_MODEL_H__
#define _BROWSER_SETTING_MODEL_H__

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <QModelIndex>

#include <gconf/gconf-client.h>

class BrowserSettingModel : public QObject
{
	Q_OBJECT
	Q_ENUMS(BrowserSettingType)
	Q_ENUMS(PageOpenedType)
	Q_ENUMS(SyncState)
	Q_ENUMS(RemoveItemsType)

	Q_PROPERTY(PageOpenedType pageOpenedOnStartup 
					   READ pageOpenedOnStartup 
						 WRITE setPageOpenedOnStartup
						 NOTIFY pageOpenedOnStartupChanged
						 )

	Q_PROPERTY(bool newTabIsHomePage 
					   READ newTabIsHomePage 
						 WRITE setNewTabIsHomePage
						 NOTIFY newTabIsHomePageChanged
						 )

	Q_PROPERTY(QString homePage 
					   READ homePage 
						 WRITE setHomePage
						 NOTIFY homePageChanged
						 )

	Q_PROPERTY(bool showBookmarkBar 
					   READ showBookmarkBar 
						 WRITE setShowBookmarkBar
						 NOTIFY showBookmarkBarChanged
						 )

	Q_PROPERTY(QString defaultSearchEngine 
					   READ defaultSearchEngine
						 WRITE setDefaultSearchEngine
						 NOTIFY defaultSearchEngineChanged
						 )

	Q_PROPERTY(QStringList searchEngineList READ searchEngineList WRITE setSearchEngineList)

	Q_PROPERTY(QStringList dataItemsToBeRemoved READ dataItemsToBeRemoved WRITE setDataItemsToBeRemoved)

	Q_PROPERTY(bool needClear
					   READ needClear
						 WRITE setNeedClear)

	Q_PROPERTY(bool savePassword 
					   READ savePassword 
						 WRITE setSavePassword
						 NOTIFY savePasswordChanged)

	Q_PROPERTY(bool allowJavascript 
					   READ allowJavascript 
						 WRITE setAllowJavascript
						 NOTIFY allowJavascriptChanged
						 )

	Q_PROPERTY(bool allowCookies 
					   READ allowCookies 
						 WRITE setAllowCookies
						 NOTIFY allowCookiesChanged
						 )

	Q_PROPERTY(bool allowImages
					   READ allowImages 
						 WRITE setAllowImages
						 NOTIFY allowImagesChanged
						 )

	Q_PROPERTY(bool allowPopup 
					   READ allowPopup
						 WRITE setAllowPopup
						 NOTIFY allowPopupChanged
						 )

	Q_PROPERTY(QString username 
					   READ username 
						 WRITE setUsername
						 NOTIFY usernameChanged
						 )

	Q_PROPERTY(QString password WRITE setPassword)

	Q_PROPERTY(QString lastSyncedTime READ lastSyncedTime)

	Q_PROPERTY(SyncState syncState
					   READ syncState 
						 WRITE setSyncState
						 NOTIFY syncStatusChanged)

	// Http Proxy
	
	Q_PROPERTY(bool useHttpProxy
						 READ useHttpProxy
						 WRITE setUseProxyHost
						 NOTIFY useHttpProxyChanged)

	Q_PROPERTY(QString proxyHost 
						 READ proxyHost
						 WRITE setProxyHost
             NOTIFY proxyHostChanged)

	Q_PROPERTY(int proxyPort
					   READ proxyPort
						 WRITE setProxyPort
             NOTIFY proxyPortChanged)

public:
	BrowserSettingModel(QObject* parent = NULL);
	~BrowserSettingModel();

	// Callback for gconf setting changes
	static void OnGConfSettingChanged(GConfClient* client, guint cnxn_id,
					GConfEntry* entry, gpointer data);

	enum BrowserSettingType
	{
		kPageOpenedOnStartup = 0,
		kNewTabIsHomePage, // Use new tab page as home page
		kHomePage,
		kShowBookmarkBar,
		kDefaultSearchEngine,
		kSearchEngineList,
		kClearDataItems,
		kNeedClear,
		kSavePassword,
		kAllowJavascript,
		kAllowCookies,
		kAllowImages,
		kAllowPopup,
		kUsername,
		kPassword,
		kLastSyncedTime,
		kSyncStatus,
		
		kEndType // the end type
	};

	enum PageOpenedType
	{
		OpenDefaultPages = 1,
		OpenLastSessionPages,
		OpenUrls
	};

	// Use gconf to communicate browser about sync progress
	// Only Request* enum can be set by standalone setting, while
	// Response* set by browser
	// 
	// Note: MUST be consist with definitions in browser
	//
	enum SyncState
	{
		SyncStopped = 0, // Sync is not setup 
		RequestSetupSync = 1,// User input their gmail account and 
		                     // request browser to setup sync
		ResponseAuthError = 2, // browser's response indicating auth error
		ResponseSyncingNow = 3,// browser try to sync now. 
		ResponseSyncDone = 4,  // browser completes sync successfully
		RequestStopSync =5   // request browser to stop
	};

	enum RemoveItemsType
	{
		RemoveHistory = 0,
		RemoveDownloads,
		RemovePasswords,
		RemoveCookies,
		RemoveFormData,
		RemoveCached
	};

public:

	GConfClient* client() const { return client_; }

	const QMap<BrowserSettingType, const char*>& keynames() const { return keynames_; }
	// getters
	PageOpenedType pageOpenedOnStartup() const { return pageOpenedOnStartup_; }
	bool newTabIsHomePage() const { return newTabIsHomePage_; }
	QString homePage() const { return homePage_; }
	bool showBookmarkBar() const { return showBookmarkBar_; }
	bool savePassword() const  { return savePassword_; }
	QString defaultSearchEngine() const { return defaultSearchEngine_; }
	QStringList searchEngineList() const { return searchEngineList_; }
	QStringList dataItemsToBeRemoved() const { return dataItemsToBeRemoved_; }
	bool needClear() const { return needClear_; }
	bool allowJavascript() const { return allowJavascript_; }
	bool allowCookies() const { return allowCookies_; }
	bool allowImages() const { return allowImages_; }
	bool allowPopup() const { return allowPopup_; }
	QString username() const { return username_; }
	QString lastSyncedTime() const { return lastSyncedTime_; }
	SyncState syncState() const { return syncState_; }

	QString proxyHost() const { return proxyHost_; }
	bool useHttpProxy() const { return useHttpProxy_; }
	int proxyPort() const { return proxyPort_; }

	// setters
	void setPageOpenedOnStartup(PageOpenedType type);
	void setNewTabIsHomePage(bool newtab);
	void setHomePage(const QString& url);
	void setShowBookmarkBar(bool show);
	void setSavePassword(bool save);
	void setDefaultSearchEngine(const QString& name);
	void setSearchEngineList(const QStringList& engines);
	void setDataItemsToBeRemoved(const QStringList& items);
	void setNeedClear(bool clear);
	void setAllowJavascript(bool allow);
	void setAllowCookies(bool allow);
	void setAllowImages(bool allow);
	void setAllowPopup(bool allow);
	void setUsername(const QString& username);
	void setPassword(const QString& password);
	void setSyncState(SyncState state);

	void setProxyHost(const QString& proxyHost);
	void setUseProxyHost(bool use);
	void setProxyPort(int port);

public Q_SLOTS:
	void RestoreDefaultValues() { SetGConfDefaultValues(); emit defaultRestored(); }

	// Maintain data items to be removed
  void ResetDataItemsToBeRemoved();
	// Add a data item into list:
	// Item must be: History, Passwords, Cookies, Cache, FormData
	void AddDataItem(QString item);
	// Remove a data item from list
	void RemoveDataItem(QString item);
	// Return true if the removing list contains the given item
	bool Contains(const QString& item);

signals:
	void defaultRestored();
	void pageOpenedOnStartupChanged(int type);
	void homePageChanged(const QString& homepage);
	void showBookmarkBarChanged(bool show);
	void newTabIsHomePageChanged(bool new_tab_is_home_page);
	void defaultSearchEngineChanged(const QString& search);
	void savePasswordChanged(bool save);
	void allowJavascriptChanged(bool allow);
	void allowCookiesChanged(bool allow);
	void allowImagesChanged(bool allow);
	void allowPopupChanged(bool allow);
	void usernameChanged(const QString& username);
	void lastSyncedTimeChanged(const QString& synced_time);
	void syncStatusChanged(SyncState status);

	void useHttpProxyChanged(bool use);
	void proxyHostChanged(const QString& proxy);
	void proxyPortChanged(int port);

private:
	void Initialize();
	
	void SetGConfDefaultValues();
	
	// Save values to GConf 
	void SaveGConfValues();

	void SetGConfValue(BrowserSettingType type, const QVariant& value);

private:
	QMap<BrowserSettingType, const char*> keynames_;
	GConfClient* client_;
	int gconf_connection_id_;

	// Properties
	PageOpenedType pageOpenedOnStartup_;
	QString homePage_;
	bool newTabIsHomePage_;
	bool showBookmarkBar_;
	QString defaultSearchEngine_;
	QStringList searchEngineList_;
	QStringList dataItemsToBeRemoved_;
	bool needClear_;
	bool allowJavascript_;
	bool allowCookies_;
	bool allowPopup_;
	bool allowImages_;
	bool savePassword_;
	QString username_;   
	QString password_;
	QString lastSyncedTime_;
	SyncState syncState_;

	bool useHttpProxy_;
	QString proxyHost_;
	int proxyPort_;
};

#endif


