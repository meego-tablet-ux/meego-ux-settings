/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QDebug>
#include <gnome-keyring.h>
#include <gnome-keyring-result.h>
#include <QtAlgorithms>
#include "BrowserSettingModel.h"

static void free_data(void* data, void* userdata);

BrowserSettingModel::BrowserSettingModel(QObject* parent) : QObject(parent),
	client_(NULL)
{
	qDebug() << "Initialize BrowserSetting Model ... ";

	keynames_[kPageOpenedOnStartup ] = "/apps/browser/PageOpenedOnStartup";
	keynames_[kNewTabIsHomePage] = "/apps/browser/NewTabIsHomePage";
	keynames_[kHomePage] = "/apps/browser/HomePage";
	keynames_[kShowBookmarkBar] = "/apps/browser/ShowBookmarkBar";
	keynames_[kDefaultSearchEngine] = "/apps/browser/DefaultSearchEngine";
	keynames_[kSearchEngineList] = "/apps/browser/SearchEngineList";
	keynames_[kClearDataItems] = "/apps/browser/ClearDataItems";
	keynames_[kNeedClear] = "/apps/browser/NeedClearBrowsingData";
	keynames_[kSavePassword] = "/apps/browser/SavePassword";
	keynames_[kAllowJavascript] = "/apps/browser/AllowJavascript";
	keynames_[kAllowPopup] = "/apps/browser/AllowPopup";
	keynames_[kAllowCookies] = "/apps/browser/AllowCookies";
	keynames_[kAllowImages] = "/apps/browser/AllowImages";
	keynames_[kUsername] = "/apps/browser/sync/Username";
	keynames_[kPassword] = "/apps/browser/sync/Password";
	keynames_[kSyncStatus] = "/apps/browser/sync/Status";
	keynames_[kLastSyncedTime] = "/apps/browser/sync/LastSyncedTime";

	Initialize();
}

void BrowserSettingModel::SaveGConfValues()
{
	SetGConfValue(kPageOpenedOnStartup, QVariant(pageOpenedOnStartup_));
	SetGConfValue(kNewTabIsHomePage, QVariant(newTabIsHomePage_));
	SetGConfValue(kHomePage, QVariant(homePage_));
	SetGConfValue(kShowBookmarkBar, QVariant(showBookmarkBar_));
	SetGConfValue(kSavePassword, QVariant(savePassword_));
	SetGConfValue(kDefaultSearchEngine, QVariant(defaultSearchEngine_));
	SetGConfValue(kSearchEngineList, QVariant(searchEngineList_));
	SetGConfValue(kClearDataItems, QVariant(dataItemsToBeRemoved_));
	SetGConfValue(kNeedClear, QVariant(needClear_));
	SetGConfValue(kAllowJavascript, QVariant(allowJavascript_));
	SetGConfValue(kAllowCookies, QVariant(allowCookies_));
	SetGConfValue(kAllowImages, QVariant(allowImages_));
	SetGConfValue(kAllowPopup, QVariant(allowPopup_));
	SetGConfValue(kUsername, QVariant(username_));
	SetGConfValue(kPassword, QVariant(password_));
	SetGConfValue(kSyncStatus, QVariant(syncState_));
}

BrowserSettingModel::~BrowserSettingModel()
{
	qDebug() << "Destroy BrowserSettingModel";

	// Save setting into GConf
	SaveGConfValues();

	if(gconf_connection_id_ > 0) 
			gconf_client_notify_remove(client_, 
					gconf_connection_id_);
}

void BrowserSettingModel::setPageOpenedOnStartup(PageOpenedType type)
{
	if(pageOpenedOnStartup_ == type) return;
	pageOpenedOnStartup_  = type;
	emit pageOpenedOnStartupChanged(type);
	SetGConfValue(kPageOpenedOnStartup, QVariant(pageOpenedOnStartup_));
}

void BrowserSettingModel::setNewTabIsHomePage(bool newtab)
{
	if(newtab == newTabIsHomePage_) return;

	newTabIsHomePage_ = newtab;
	emit newTabIsHomePageChanged(newtab);
	SetGConfValue(kNewTabIsHomePage, QVariant(newTabIsHomePage_));
}

void BrowserSettingModel::setHomePage(const QString& url)
{
	if(url == homePage_) return;
	homePage_ = url;
	emit homePageChanged(url);
	SetGConfValue(kHomePage, QVariant(homePage_));
}
void BrowserSettingModel::setShowBookmarkBar(bool show)
{
	if(show == showBookmarkBar_) return;
	showBookmarkBar_ = show;
	emit showBookmarkBarChanged(show);
	SetGConfValue(kShowBookmarkBar, QVariant(showBookmarkBar_));
}

void BrowserSettingModel::setSavePassword(bool save)
{
		if(save == savePassword_) return;
		savePassword_ = save;
		emit savePasswordChanged(save);
		SetGConfValue(kSavePassword, QVariant(savePassword_));
}
void BrowserSettingModel::setDefaultSearchEngine(const QString& search_engine)
{
	if(search_engine == defaultSearchEngine_) return;
	defaultSearchEngine_ = search_engine;
	emit defaultSearchEngineChanged(search_engine);
	SetGConfValue(kDefaultSearchEngine, QVariant(defaultSearchEngine_));
}
void BrowserSettingModel::setSearchEngineList(const QStringList& engines)
{
	if(engines == searchEngineList_) return;
	searchEngineList_ = engines;
}

void BrowserSettingModel::setDataItemsToBeRemoved(const QStringList& items)
{
		if(dataItemsToBeRemoved_ == items) return;
		dataItemsToBeRemoved_ = items;
}
void BrowserSettingModel::setNeedClear(bool clear)
{
		if(needClear_ == clear) return;
		needClear_ = clear;
		SetGConfValue(kNeedClear, QVariant(needClear_));
		SetGConfValue(kClearDataItems, QVariant(dataItemsToBeRemoved_));
}

void BrowserSettingModel::setAllowJavascript(bool allow)
{
	if(allowJavascript_ == allow) return;
	allowJavascript_ = allow;
	emit allowJavascriptChanged(allow);
	SetGConfValue(kAllowJavascript, QVariant(allowJavascript_));
}
void BrowserSettingModel::setAllowCookies(bool allow)
{
	if(allowCookies_ == allow) return;
	allowCookies_ = allow;
	emit allowCookiesChanged(allow);
	SetGConfValue(kAllowCookies, QVariant(allowCookies_));
}
void BrowserSettingModel::setAllowImages(bool allow)
{
	if(allowImages_ == allow) return;
	allowImages_ = allow;
	emit allowImagesChanged(allow);
	SetGConfValue(kAllowImages, QVariant(allowImages_));
}
void BrowserSettingModel::setAllowPopup(bool allow)
{
	if(allowPopup_ == allow) return;
	allowPopup_ = allow;
	emit allowPopupChanged(allow);
	SetGConfValue(kAllowPopup, QVariant(allowPopup_));
}
void BrowserSettingModel::setUsername(const QString& username)
{
	if(username_ == username) return;
	username_ = username;
	emit usernameChanged(username);
	SetGConfValue(kUsername, QVariant(username_));
}

void BrowserSettingModel::setPassword(const QString& password)
{
	if(password_ == password) return;
	password_ = password;
}

void BrowserSettingModel::setSyncState(SyncState state)
{
		if(state == syncState_) return;

		syncState_ = state;
		emit syncStatusChanged(syncState_);
		SetGConfValue(kUsername, QVariant(username_));
		SetGConfValue(kPassword, QVariant(password_));
		SetGConfValue(kSyncStatus, QVariant(syncState_));
}

// Callback called when /apps/browser/SyncState changed
void BrowserSettingModel::OnGConfSettingChanged(GConfClient* client, 
				guint cnxn_id, GConfEntry* entry, gpointer data)
{
		BrowserSettingModel* model = static_cast<BrowserSettingModel*>(data);
		const QMap<BrowserSettingType, const char*>& keynames = model->keynames();
		if(g_strcmp0(entry->key, keynames[BrowserSettingModel::kSyncStatus]) == 0)
		{
				int status = gconf_client_get_int(client, entry->key, NULL);
				model->setSyncState(static_cast<SyncState>(status));

		} else if(!g_strcmp0(entry->key, keynames[BrowserSettingModel::kNewTabIsHomePage])) {

				bool new_tab_is_home_page = gconf_client_get_bool(model->client(), entry->key, NULL);
				
				model->setNewTabIsHomePage(new_tab_is_home_page);

		} /*else if(!g_strcmp0(entry->key, keynames[BrowserSettingModel::kHomePage])) {
				
				char* homepage = gconf_client_get_string(model->client(), entry->key, NULL);
				QString full_url(homepage);
				
				// Append http prefix to a raw URL
				if(!full_url.startsWith("http://", Qt::CaseInsensitive) &&
								!full_url.startsWith("https://", Qt::CaseInsensitive)) {
						full_url.prepend("http://");
				}
				
				model->setHomePage(full_url);

				g_free(homepage);

		} */ else if(!g_strcmp0(entry->key, keynames[BrowserSettingModel::kShowBookmarkBar])) {
				bool show = gconf_client_get_bool(model->client(), entry->key, NULL);

				model->setShowBookmarkBar(show);
		} else if(!g_strcmp0(entry->key, keynames[BrowserSettingModel::kDefaultSearchEngine])) {
				char* tmp = gconf_client_get_string(model->client(), entry->key, NULL);
        QString engine = QString::fromUtf8(tmp);
				model->setDefaultSearchEngine(engine);
				g_free(tmp);
		} else {
				qDebug() << " No handler for " << entry->key;
		}


}

void BrowserSettingModel::Initialize()
{

	char* tmp = NULL;

	g_type_init();

	client_ = gconf_client_get_default();
	
	gconf_client_add_dir(client_, "/apps/browser", GCONF_CLIENT_PRELOAD_NONE, NULL);

	gconf_client_add_dir(client_, "/system/http_proxy", GCONF_CLIENT_PRELOAD_NONE, NULL);
	
	gconf_connection_id_ = gconf_client_notify_add(client_,
		"/apps/browser",
		OnGConfSettingChanged,
		this,
		NULL, // destroy func
		NULL);

	if(!client_) { qDebug() << " failed to get default gconf client"; return; }

	if(gconf_client_dir_exists(client_, "/apps/browser", NULL))
	{
		// Page opened type
		pageOpenedOnStartup_ = 
			static_cast<PageOpenedType>(
							gconf_client_get_int(client_, keynames_[kPageOpenedOnStartup], NULL)
							);

		newTabIsHomePage_ =
				gconf_client_get_bool(client_, keynames_[kNewTabIsHomePage], NULL);

		tmp =	gconf_client_get_string(client_, keynames_[kHomePage], NULL);
		homePage_ = QString::fromUtf8(tmp);
		if(tmp) g_free(tmp);

		showBookmarkBar_  = 
			gconf_client_get_bool(client_, keynames_[kShowBookmarkBar], NULL);

		
		tmp = gconf_client_get_string(client_, keynames_[kDefaultSearchEngine], NULL);
		defaultSearchEngine_ = QString::fromUtf8(tmp);
		if(tmp) g_free(tmp);

		savePassword_ =
				gconf_client_get_bool(client_, keynames_[kSavePassword], NULL);

		// Search Engine List
		GSList* list = NULL;
		list = gconf_client_get_list(client_, keynames_[kSearchEngineList], 
				GCONF_VALUE_STRING, NULL);
		for(GSList* node = list; node; node = g_slist_next(node)){
				QString se = QString::fromUtf8(static_cast<char*>(node->data));
				if(!searchEngineList_.contains(se))
					searchEngineList_ << se;
		}
		g_slist_foreach(list, free_data, NULL);
		g_slist_free(list);

		// Data items removed
		GSList* items = NULL;
		items = gconf_client_get_list(client_, keynames_[kClearDataItems],
				GCONF_VALUE_STRING, NULL);
		for(GSList* item = items; item; item = g_slist_next(item))
		{
			QString tmp = QString(static_cast<char*>(item->data));
			if(!dataItemsToBeRemoved_.contains(tmp))
					dataItemsToBeRemoved_.append(tmp);
		}
		g_slist_foreach(items, free_data, NULL);
		g_slist_free(items);

		needClear_ = gconf_client_get_bool(client_, keynames_[kNeedClear], NULL);

		allowJavascript_ =
			gconf_client_get_bool(client_, keynames_[kAllowJavascript], NULL);

		allowCookies_ =
			gconf_client_get_bool(client_, keynames_[kAllowCookies], NULL);

		allowImages_ =
			gconf_client_get_bool(client_, keynames_[kAllowImages], NULL);

		allowPopup_ = 
			gconf_client_get_bool(client_, keynames_[kAllowPopup], NULL);

		tmp = gconf_client_get_string(client_, keynames_[kUsername], NULL);
		username_ = QString::fromUtf8(tmp);
		if(tmp) g_free(tmp);

		tmp = gconf_client_get_string(client_, keynames_[kLastSyncedTime], NULL);
		lastSyncedTime_ = QString::fromUtf8(tmp);
		if(tmp) g_free(tmp);

	} else { // GConf has no setting for browser, set default values
		qDebug() << "No /apps/browser gconf dir found. Create it with default value";
		
		SetGConfDefaultValues();
	}

	// Networking Proxy Handling 
	gboolean use_http_proxy = gconf_client_get_bool(client_, 
					"/system/http_proxy/use_http_proxy", NULL);
	char* proxy_mode = gconf_client_get_string(client_, 
					"/system/proxy/mode", NULL);

	// Only when proxy is in manual mode and http proxy is enabled
	if(g_strcmp0(proxy_mode, "manual") == 0 && use_http_proxy)
			useHttpProxy_ = true;
	else useHttpProxy_ = false;

	if(proxy_mode) g_free(proxy_mode);

	// Get proxy host and port
	tmp = gconf_client_get_string(client_, 
					"/system/http_proxy/host", NULL); 
	proxyHost_ = QString::fromUtf8(tmp);
	if(tmp) g_free(tmp);

	proxyPort_ = gconf_client_get_int(client_, 
					"/system/http_proxy/port", NULL);
}

void BrowserSettingModel::SetGConfDefaultValues()
{
	setPageOpenedOnStartup(BrowserSettingModel::OpenDefaultPages);
	setNewTabIsHomePage(true);
	setHomePage("http://meego.com");
	setShowBookmarkBar(true);
	setDefaultSearchEngine("Google");

	QStringList engines;
	engines << "Google";
	setSearchEngineList(engines);

	setAllowJavascript(true);
	setAllowCookies(true);
	setAllowImages(true);
	setAllowPopup(false);
	setSavePassword(true);
	setSyncState(BrowserSettingModel::SyncStopped);

	setNeedClear(false);
}

static void free_data(void* data, void* userdata)
{
		g_free(data);
}

void BrowserSettingModel::SetGConfValue(BrowserSettingType type, const QVariant& val)
{
	switch(type)
	{
		case kPageOpenedOnStartup:
			gconf_client_set_int(client_, keynames_[kPageOpenedOnStartup], 
					val.toInt(), NULL);
			break;
		case kNewTabIsHomePage:
			{
					gconf_client_set_bool(client_, keynames_[kNewTabIsHomePage], val.toBool(), NULL);
					break;
			}
		case kHomePage:
			{
        QString home_page = val.toString();
				gconf_client_set_string(client_, keynames_[kHomePage], 
						home_page.toUtf8().data(), NULL);
			}
			break;
		case kShowBookmarkBar:
			gconf_client_set_bool(client_, keynames_[kShowBookmarkBar], 
					val.toBool(), NULL);
			break;
		case kDefaultSearchEngine:
			{
        QString search_engine = val.toString();
				gconf_client_set_string(client_, keynames_[kDefaultSearchEngine],
						search_engine.toUtf8().data(), NULL);
			}
			break;
		case kSearchEngineList:
			// Search engine list is written by chromium browser
			// Standalone is not able to modify the search engine list
			{
				QStringList engines = val.toStringList();
				GSList* list = NULL;
				for(int i = 0; i < engines.size(); i++)
				{
					QByteArray bytes = engines[i].toUtf8();
					list = g_slist_append(list, (void*)g_strdup(bytes.data()));
				}
				gconf_client_set_list(client_, keynames_[kSearchEngineList],
						GCONF_VALUE_STRING, list, NULL);
				g_slist_foreach(list, free_data, NULL);
				g_slist_free(list);

			}
			break;
		case kNeedClear:
			{
					gconf_client_set_bool(client_, keynames_[kNeedClear], needClear_, NULL);
					break;
			}
		case kClearDataItems:
			{
				QStringList items = val.toStringList();
				GSList* list = NULL;
				for(int i = 0; i < items.size(); i++)
				{
					QByteArray bytes = items[i].toUtf8();
					list = g_slist_append(list, (gpointer)g_strdup(bytes.data()));
				}
				gconf_client_set_list(client_, keynames_[kClearDataItems],
						GCONF_VALUE_STRING, list, NULL);
				g_slist_foreach(list, free_data, NULL);
				g_slist_free(list);
			}
			break;
		case kAllowJavascript:
			gconf_client_set_bool(client_, keynames_[kAllowJavascript], 
					val.toBool(), NULL);
			break;
		case kAllowCookies:
			gconf_client_set_bool(client_, keynames_[kAllowCookies],
					val.toBool(), NULL);
			break;
		case kAllowPopup:
			gconf_client_set_bool(client_, keynames_[kAllowPopup],
					val.toBool(), NULL);
			break;
		case kAllowImages:
			gconf_client_set_bool(client_, keynames_[kAllowImages],
					val.toBool(), NULL);

			break;
		case kSyncStatus:
			gconf_client_set_int(client_, keynames_[kSyncStatus],
					val.toInt(), NULL);
			break;

		case kUsername:
			{
				if(val.toString().length() == 0) return;
				QString username = val.toString();
				gconf_client_set_string(client_, keynames_[kUsername],
						username.toUtf8().data(), NULL);
			}
			break;
	
		case kPassword:
			{
				if(val.toString().length() == 0) break;

				std::string password = val.toString().toStdString();
				char* base64_password = 
						g_base64_encode((guchar*)password.c_str(), strlen(password.c_str()));
				gconf_client_set_string(client_, keynames_[kPassword], 
								base64_password,  NULL);
				g_free(base64_password);
			}
			break;

		case kSavePassword:
			{
					gconf_client_set_bool(client_, keynames_[kSavePassword], val.toBool(), NULL);
					break;
			}

		case kLastSyncedTime:
			{
        QString time = val.toString();
				gconf_client_set_string(client_, keynames_[kLastSyncedTime],
						time.toUtf8().data(), NULL);
			}
			break;
		default:
			qDebug() << "Unknown type: " << keynames_[type]; break;
	}
}

void BrowserSettingModel::ResetDataItemsToBeRemoved()
{
		dataItemsToBeRemoved_.clear();
		SetGConfValue(kClearDataItems, QVariant(dataItemsToBeRemoved_));
}

void BrowserSettingModel::AddDataItem(QString item)
{
		if(!dataItemsToBeRemoved_.contains(item)) {
				dataItemsToBeRemoved_.append(item);
				SetGConfValue(kClearDataItems, QVariant(dataItemsToBeRemoved_));
		}
}
void BrowserSettingModel::RemoveDataItem(QString item)
{
		dataItemsToBeRemoved_.removeAll(item);
		SetGConfValue(kClearDataItems, QVariant(dataItemsToBeRemoved_));
}
bool BrowserSettingModel::Contains(const QString& item)
{
		QStringList::iterator iter = qFind(dataItemsToBeRemoved_.begin(),
						dataItemsToBeRemoved_.end(), item);
		return iter != dataItemsToBeRemoved_.end();
}

// Networking proxy handler
void BrowserSettingModel::setUseProxyHost(bool use)
{
    qDebug() << "### set use http proxy: " << use;
		useHttpProxy_ = use;
		gconf_client_set_bool(client_, "/system/http_proxy/use_http_proxy", 
						useHttpProxy_, NULL);
		if(use) {
				gconf_client_set_string(client_, "/system/proxy/mode", "manual", NULL);
				gconf_client_set_bool(client_, "/system/http_proxy/use_same_proxy", TRUE, NULL);
		} else {
				gconf_client_set_string(client_, "/system/proxy/mode", "none", NULL);
		}

		emit useHttpProxyChanged(use);

		return;
}

void BrowserSettingModel::setProxyHost(const QString& proxy)
{
		proxyHost_ = proxy;
		std::string s = proxyHost_.toStdString();
		gconf_client_set_string(client_, "/system/http_proxy/host",
						s.c_str(), NULL);
		emit proxyHostChanged(proxy);
		return;
}

void BrowserSettingModel::setProxyPort(int port)
{
		if(port < 0 || port > 65535) return;

		proxyPort_ = port;

		gconf_client_set_int(client_, "/system/http_proxy/port",
						proxyPort_, NULL);
		emit proxyPortChanged(port);
		return;
}


