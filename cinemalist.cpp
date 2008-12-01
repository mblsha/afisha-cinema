/*
 * cinemalist.cpp
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

#include "cinemalist.h"

#include <QDebug>
#include <QtAlgorithms>
#include <QDomDocument>
#include <QDomElement>
#include <QTimer>
#include <QFile>

#include "xmpp_xmlcommon.h"
#include "httprequest.h"
#include "afishahelpers.h"
#include "cinema.h"

CinemaList::CinemaList()
	: QObject()
	, saveCacheTimer_(0)
{
	saveCacheTimer_ = new QTimer(this);
	saveCacheTimer_->setInterval(5000);
	saveCacheTimer_->setSingleShot(true);
	connect(saveCacheTimer_, SIGNAL(timeout()), SLOT(saveCache()));
}

CinemaList::~CinemaList()
{
}

Cinema* CinemaList::findCinema(const QString& id) const
{
	foreach(Cinema* cinema, cinemas_) {
		if (cinema->id() == id)
			return cinema;
	}
	return 0;
}

void CinemaList::init()
{
	if (QFile::exists(cacheFileName())) {
		QFile file(cacheFileName());
		if (file.open(QFile::ReadOnly)) {
			initFromData(QString::fromUtf8(file.readAll()), true);
			return;
		}
	}

	initFromWeb();
}

void CinemaList::initFromWeb()
{
	if (request_)
		delete request_;
	request_ = new HttpRequest(QString("%1_cinemas").arg(AfishaHelpers::cinemaCacheDate()), this);
	connect(request_, SIGNAL(finished()), SLOT(requestFinished()));
	request_->request(":queries/cinemas.xq",
	                  QString("%1/chooser_place.xml?city=MSK&type=cinema&date=%2&")
	                  .arg(AfishaHelpers::host())
	                  .arg(AfishaHelpers::currentDate()));
}

void CinemaList::requestFinished()
{
	if (request_.isNull())
		return;

	if (!request_->error()) {
		initFromData(request_->result(), false);
	}

	delete request_;
}

void CinemaList::initFromData(const QString& xml, bool fromCache)
{
	qDeleteAll(cinemas_);
	cinemas_.clear();

	QDomDocument doc;
	if (!doc.setContent(xml))
		return;

	QDomElement root = doc.documentElement();
	Q_ASSERT(root.tagName() == "cinemas");
	if (root.tagName() != "cinemas")
		return;

	if (root.attribute("cached") == "true") {
		if (!root.hasAttribute("numCinemas") || !root.attribute("numCinemas").toInt()) {
			initFromWeb();
			return;
		}
	}

	for (QDomNode n = root.firstChild(); !n.isNull(); n = n.nextSibling()) {
		QDomElement e = n.toElement();
		if (e.isNull())
			continue;

		if (e.tagName() == "cinema") {
			Cinema* cinema = new Cinema();
			connect(cinema, SIGNAL(dataChanged()), SLOT(cinemaDataChanged()));
			cinema->initFromXml(e);
			if (!cinema->hasDetailedInfo()) {
				cinema->updateFromWeb();
			}
			cinemas_ << cinema;
		}
	}

	cinemaDataChanged();
	emit dataChanged();
}

QString CinemaList::cacheFileName() const
{
	return QString("%1/cinemas.xml")
	       .arg(AfishaHelpers::cacheDir());
}

void CinemaList::cinemaDataChanged()
{
	saveCacheTimer_->start();
}

void CinemaList::saveCache()
{
	qWarning("CinemaList::saveCache()");
	QDomDocument doc;
	QDomElement root = doc.createElement("cinemas");
	root.setAttribute("cached", "true");
	root.setAttribute("lastUpdatedAt", QDateTime::currentDateTime().toString(Qt::ISODate));
	root.setAttribute("numCinemas", QString::number(cinemas_.count()));
	doc.appendChild(root);

	foreach(Cinema* cinema, cinemas_) {
		QDomElement e = cinema->toXml(&doc);
		root.appendChild(e);
	}

	QFile file(cacheFileName());
	if (file.open(QFile::WriteOnly)) {
		file.write(doc.toByteArray(2));
	}
}
