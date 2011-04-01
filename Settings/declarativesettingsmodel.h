/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef THIRDPARTYSETTINGSCOMPONENT_H
#define THIRDPARTYSETTINGSCOMPONENT_H

#include <QGraphicsWidget>
#include <QDeclarativeListProperty>

namespace TabletSettings
{

	class Setting: public QObject
	{
		Q_OBJECT
		Q_PROPERTY(QString key READ key);
		Q_PROPERTY(QString title READ title);
		Q_PROPERTY(QVariant value READ value WRITE setValue);
		Q_PROPERTY(Type type READ type);
		Q_ENUMS(Type)

	public:

		enum Type
		{
			Unknown=0,
			TextType,
			SelectionType,
			BooleanType,
			IntegerType
		};

		Setting(QString key="", Type t=Unknown, QObject *parent=0):QObject(parent),m_key(key),m_type(t) { }
		QString key() { return m_key; }
		void setKey(QString key) { m_key = key; }

		Type type() { return m_type; }
		void setType(Type t) { m_type = t; }

		QString title() { return m_title; }
		void setTitle(QString t) { m_title = t; }

		QVariant value() { return m_value; }
		void setValue(QVariant v) { m_value = v; }

	private:
		QString m_key;
		QString m_title;
		QVariant m_value;
		Type m_type;
	};

	class Text: public Setting
	{
		Q_OBJECT

	public:
		Text(QString key="",QObject *parent=0):Setting(key, TextType, parent) { }
	};

	class Option: public QObject
	{
		Q_OBJECT
		Q_PROPERTY(QString title READ title WRITE setTitle);
		Q_PROPERTY(QVariant value READ value WRITE setValue);

	public:
		Option(QObject* parent=0):QObject(parent) { }
		QString title(){ return m_title; }
		QVariant value() { return m_value; }

		///setters:
		void setTitle(QString t) { m_title = t; }
		void setValue(QVariant v) { m_value = v; }

	private:
		QString m_title;
		QVariant m_value;
	};

	class Selection: public Setting
	{
		Q_OBJECT
		Q_PROPERTY(QStringList model READ model);
		Q_PROPERTY(QDeclarativeListProperty<QVariant> payload READ payload);
	public:
		Selection(QString key="",QObject *parent=0): Setting(key, SelectionType, parent) { }

		QStringList model() { return m_model; }
		QDeclarativeListProperty<QVariant> payload() { return QDeclarativeListProperty<QVariant>(this, m_payload); }

	public slots:
		void setModel(QStringList o) { m_model = o; }
		void setPayload(QList<QVariant*> p) { m_payload = p; }

	private:
		QStringList m_model;
		QList<QVariant*> m_payload;
	};

	class Boolean: public Setting
	{
		Q_OBJECT
	public:
		Boolean(QString key="",QObject *parent=0): Setting(key,BooleanType, parent) { }
	};

	class Integer: public Setting
	{
		Q_OBJECT
		Q_PROPERTY(int min READ min WRITE setMin);
		Q_PROPERTY(int max READ max WRITE setMax);

	public:
		Integer(QString key="",QObject *parent=0): Setting(key, IntegerType, parent) { }
		int min() { return m_min; }
		int max() { return m_max; }

		void setMin(int m){ m_min = m; }
		void setMax(int m){ m_max = m; }

	private:
		int m_min;
		int m_max;
	};

	class Group: public QObject
	{
		Q_OBJECT
		Q_PROPERTY(QDeclarativeListProperty<TabletSettings::Setting> settings READ settings);


	public:
		Group(QObject* parent=0): QObject(parent) { }
		QDeclarativeListProperty<Setting> settings()
		{
			return QDeclarativeListProperty<Setting>(this, m_settings);
		}

		void addSetting(Setting* s) { m_settings.append(s); }

	private:
		QList<Setting*> m_settings;

	};
}

class DeclarativeSettingsModel : public QObject
{
    Q_OBJECT
	Q_PROPERTY(QString document READ document WRITE setDocument);
	Q_PROPERTY(State state READ state);
	Q_PROPERTY(QDeclarativeListProperty<TabletSettings::Group> groups READ groups);
	Q_ENUMS(State)

public:

	enum State {
		Idle = 0,
		Loading = 1,
		Ready = 2
	};

	explicit DeclarativeSettingsModel(QObject *parent = 0);

	void setDocument(QString doc) { m_doc = doc; parseXml(); }
	QString document() { return m_doc; }

	State state() { return m_state; }

	int count() { return m_group.count(); }

signals:
	void stateChanged(int newState);
	void valueChanged(QString key, QVariant value);

public slots:

	QDeclarativeListProperty<TabletSettings::Group> groups()
	{ return QDeclarativeListProperty<TabletSettings::Group>(this, m_group); }

	void setValue(QString key, QVariant value);

private slots:
	void valueChanged();

private:
	void parseXml();
	void setState(State s);

	QList<TabletSettings::Group*> m_group;

	QString m_doc;
	State m_state;
};

#endif // THIRDPARTYSETTINGSCOMPONENT_H
