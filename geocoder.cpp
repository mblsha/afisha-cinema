/*
 * geocoder.cpp
 * Copyright (C) 2008  Michail Pishchagin
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "geocoder.h"

#include <QDebug>
#include <QDomElement>
#include <QDomDocument>
#include <QStringList>

#include "xmpp_xmlcommon.h"
#include "yamapsapi.h"

Geocoder::Geocoder(QString id, QObject* parent)
	: HttpRequest(id, parent)
{
	setType("geocoder");
}

Geocoder::~Geocoder()
{
}

QString Geocoder::address() const
{
	return address_;
}

QString Geocoder::ll() const
{
	return ll_;
}

QString Geocoder::processAddress(const QString& address) const
{
	QStringList addr = address.split(",");
	QStringList bad;
	foreach(QString a, addr) {
		if (a.contains("\"") ||
		    a.contains(QString::fromUtf8("«")) ||
		    a.contains(QString::fromUtf8("»"))) {
			bad << a;
		}

	}

	foreach(QString b, bad) {
		addr.removeAll(b);
	}

	QString result = addr.join(", ");
	// if (!bad.isEmpty()) {
	// 	qWarning() << address << bad << result;
	// }
	
	return result;
}

void Geocoder::request(const QString& address)
{
	address_ = processAddress(address);
	HttpRequest::request(":queries/geocoder.xq",
	                     QString("http://geocode-maps.yandex.ru/1.x/?geocode=%1&key=%2")
	                     .arg(address_)
	                     .arg(YaMapsApi::key())
	                    );
}

void Geocoder::delayedFinished()
{
	if (!error()) {
		initFromData(result());
	}

	HttpRequest::delayedFinished();
}

void Geocoder::initFromData(const QString& xml)
{
	QDomDocument doc;
	if (!doc.setContent(xml))
		return;

	initFromXml(doc.documentElement());
}

void Geocoder::clear()
{
	address_ = QString();
	ll_ = QString();
}

void Geocoder::initFromXml(const QDomElement& element)
{
	Q_ASSERT(element.tagName() == "geocoder");
	clear();

	address_ = XMLHelper::subTagText(element, "address");

	for (QDomNode n = element.firstChild(); !n.isNull(); n = n.nextSibling()) {
		QDomElement e = n.toElement();
		if (e.isNull())
			continue;

		if (e.tagName() == "object") {
			QString country = XMLHelper::subTagText(e, "country");
			QString locality = XMLHelper::subTagText(e, "locality");
			QString ll = XMLHelper::subTagText(e, "ll");

			if (country == QString::fromUtf8("Россия") &&
			    locality == QString::fromUtf8("Москва")) {
				ll_ = ll;
				break;
			}
		}
	}

	if (ll_.isEmpty()) {
		qWarning("Geocoder(%s): ll not found for '%s'", qPrintable(id()), qPrintable(address_));
	}
}
