/*
 * cinema.cpp
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

#include "cinema.h"

#include <QDebug>
#include <QDomElement>
#include <QDomDocument>

#include "xmpp_xmlcommon.h"
#include "httprequest.h"
#include "afishahelpers.h"
#include "geocoder.h"

Cinema::Cinema()
	: QObject()
	, hasDetailedInfo_(false)
	, finishedProgress_(0)
{
}

Cinema::~Cinema()
{
}

QString Cinema::name() const
{
	return name_;
}

QString Cinema::id() const
{
	return id_;
}

QString Cinema::address() const
{
	return address_;
}

QString Cinema::metro() const
{
	return metro_;
}

QString Cinema::ll() const
{
	return ll_;
}

bool Cinema::hasDetailedInfo() const
{
	return hasDetailedInfo_;
}

void Cinema::updateFromWeb()
{
	Q_ASSERT(!id_.isEmpty());
	if (request_)
		delete request_;
	request_ = new HttpRequest(QString("%1_cinema_%2")
	                           .arg(AfishaHelpers::cinemaCacheDate())
	                           .arg(id_), this);
	request_->setType("cinema_details");
	connect(request_, SIGNAL(finished()), SLOT(requestFinished()));
	request_->request(":queries/cinema.xq",
	                  QString("%1/place.xml?city=MSK&type=cinema&date=%2&place_id=%3&")
	                  .arg(AfishaHelpers::host())
	                  .arg(AfishaHelpers::currentDate())
	                  .arg(id_));
}

void Cinema::requestFinished()
{
	if (request_.isNull()) {
		return;
	}

	if (!request_->error()) {
		if (request_->type() == "cinema_details") {
			QString xml = request_->result();
			delete request_;

			initFromData(xml);
		}
		else if (request_->type() == "geocoder") {
			Geocoder* geocoder = dynamic_cast<Geocoder*>(request_.data());
			if (geocoder) {
				ll_ = geocoder->ll();
				finishedProgress_ = 100;
				emit dataChanged();
			}
		}
	}
}

void Cinema::initFromData(const QString& xml)
{
	QDomDocument doc;
	if (!doc.setContent(xml))
		return;

	initFromXml(doc.documentElement());
}

QDomElement Cinema::toXml(QDomDocument* doc) const
{
	QDomElement e = doc->createElement("cinema");
	e.setAttribute("detailed", "true");
	e.setAttribute("lastUpdatedAt", lastUpdatedAt_.toString(Qt::ISODate));
	e.appendChild(XMLHelper::textTag(*doc, "id", id_));
	e.appendChild(XMLHelper::textTag(*doc, "name", name_));
	e.appendChild(XMLHelper::textTag(*doc, "address", address_));
	e.appendChild(XMLHelper::textTag(*doc, "metro", metro_));
	e.appendChild(XMLHelper::textTag(*doc, "details", details_));
	e.appendChild(XMLHelper::textTag(*doc, "ll", ll_));
	return e;
}

void Cinema::initFromXml(const QDomElement& e)
{
	Q_ASSERT(e.tagName() == "cinema");
	clear();

	id_ = XMLHelper::subTagText(e, "id");
	name_ = XMLHelper::subTagText(e, "name");
	address_ = XMLHelper::subTagText(e, "address");
	metro_ = XMLHelper::subTagText(e, "metro");
	details_ = XMLHelper::subTagText(e, "details");
	ll_ = XMLHelper::subTagText(e, "ll");

	if (!e.attribute("detailed").isEmpty()) {
		hasDetailedInfo_ = true;
		finishedProgress_ = 50;

		if (!address_.isEmpty() && ll_.isEmpty() && !XMLHelper::hasSubTag(e, "ll")) {
			Geocoder* geocoder = new Geocoder(QString("%1_geocoder_%2")
			                                  .arg(AfishaHelpers::cinemaCacheDate())
			                                  .arg(id_), this);
			request_ = geocoder;
			connect(geocoder, SIGNAL(finished()), SLOT(requestFinished()));
			geocoder->request(address_);
		}

		if (!ll_.isEmpty()) {
			finishedProgress_ = 100;
		}
		// if (!details_.isEmpty())
			// qWarning() << id_ << name_ << address_ << metro_;
	}

	emit dataChanged();
}

void Cinema::clear()
{
	id_ = QString();
	name_ = QString();
	address_ = QString();
	metro_ = QString();
	details_ = QString();
	ll_ = QString();
	mapUrl_ = QString();
	phones_ = QStringList();
	lastUpdatedAt_ = QDateTime::currentDateTime();
	hasDetailedInfo_ = false;
}

QString Cinema::detailsLinkForId(const QString& id)
{
	return QString("%1/place.xml?city=MSK&type=cinema&date=%2&place_id=%3&")
	             .arg(AfishaHelpers::host())
	             .arg(AfishaHelpers::currentDate())
	             .arg(id);
}

int Cinema::finishedProgress() const
{
	return finishedProgress_;
}

int Cinema::fullProgress() const
{
	return 100;
}
