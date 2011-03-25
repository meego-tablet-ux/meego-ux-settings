/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "declarativesettingsmodel.h"
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <MGConfItem>

DeclarativeSettingsModel::DeclarativeSettingsModel(QObject *parent)
	:QObject(parent),m_state(Idle)
{

}

void DeclarativeSettingsModel::setState(State s)
{
	m_state = s;
	emit stateChanged(m_state);
}

void DeclarativeSettingsModel::setValue(QString key, QVariant value)
{
	Q_UNUSED(key);
	Q_UNUSED(value);

	///todo: actually set the key to the value here!
	MGConfItem item(key);
	item.set(value);
	qDebug()<<"setting key "<<key<<" to "<<value;
}

void DeclarativeSettingsModel::valueChanged()
{
	MGConfItem* item = qobject_cast<MGConfItem*>(sender());
	emit valueChanged(item->key(),item->value());
}

void DeclarativeSettingsModel::parseXml()
{
	setState(Loading);

	if(m_doc == "")
	{
		qDebug("no document source specified");
		return;
	}

	QDomDocument doc;

	QFile file(m_doc);
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug("failed to load file");
		return;
	}

	if(!doc.setContent(&file))
	{
		qDebug("Failed to parse document");
	}

	file.close();

	QDomNode node = doc.documentElement().firstChild();

	while(!node.isNull())
	{
		if(node.toElement().nodeName().toLower() == "group")
		{
			///parse the group

			TabletSettings::Group * group = new TabletSettings::Group(this);

			QDomNode widgetNode = node.firstChild();

			while(!widgetNode.isNull())
			{
				QDomElement element = widgetNode.toElement();
				qDebug()<<"element name: "<<element.nodeName();
				if(element.nodeName().toLower() == "text")
				{
					QString key = element.attribute("key");
					QString title = element.attribute("title");

					TabletSettings::Text *text = new TabletSettings::Text(key,group);
					MGConfItem* item = new MGConfItem(key, text);
					qDebug()<<"inititial value for "<<key<<": "<<item->value();
					connect(item,SIGNAL(valueChanged()),this,SLOT(valueChanged()));

					if(item->value().isValid())
						text->setValue(item->value());
					else text->setValue(element.text());

					text->setTitle(title);


					group->addSetting(text);
				}
				else if(element.nodeName().toLower() == "boolean")
				{
					QString key = element.attribute("key");
					QString title = element.attribute("title");

					TabletSettings::Boolean *bahool = new TabletSettings::Boolean(key,group);

					MGConfItem* item = new MGConfItem(key, bahool);
					qDebug()<<"inititial value for "<<key<<": "<<item->value();
					connect(item,SIGNAL(valueChanged()),this,SLOT(valueChanged()));

					bahool->setTitle(title);
					bahool->setValue(item->value());

					group->addSetting(bahool);
				}
				else if(element.nodeName().toLower() == "integer")
				{
					qDebug("parsing integer");
					QString key = element.attribute("key");
					QString title = element.attribute("title");
					int min = element.attribute("min").toInt();
					int max = element.attribute("max").toInt();
					TabletSettings::Integer *integer = new TabletSettings::Integer(key,group);

					MGConfItem* item = new MGConfItem(key,integer);
					qDebug()<<"inititial value for "<<key<<": "<<item->value();
					connect(item,SIGNAL(valueChanged()),this,SLOT(valueChanged()));

					integer->setValue(item->value());
					integer->setTitle(title);
					integer->setMax(max);
					integer->setMin(min);

					group->addSetting(integer);
				}


				widgetNode = widgetNode.nextSibling();
			}

			m_group.append(group);
		}
		node = node.nextSibling();
	}


	setState(Ready);
}
