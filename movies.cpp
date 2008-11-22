/*
 * movies.cpp
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

#include "movies.h"

#include <QDebug>
#include <QtAlgorithms>
#include <QDomDocument>
#include <QDomElement>

#include "xmpp_xmlcommon.h"
#include "httprequest.h"
#include "afishahelpers.h"
#include "movie.h"

#define INIT_FROM_AFISHA

Movies::Movies()
	: QObject()
{
}

Movies::~Movies()
{
}

QList<Movie*> Movies::movies() const
{
	return movies_;
}

Movie* Movies::findMovie(const QString& id) const
{
	foreach(Movie* movie, movies_) {
		if (movie->id() == id)
			return movie;
	}
	return 0;
}

void Movies::init()
{
	initFromWeb();
}

void Movies::initFromWeb()
{
	QString id = QString("%1_movies").arg(AfishaHelpers::currentDate());
#ifdef INIT_FROM_AFISHA
	id += "_afisha";
#endif

	if (request_)
		delete request_;
	request_ = new HttpRequest(id, this);
	connect(request_, SIGNAL(finished()), SLOT(requestFinished()));
#ifdef INIT_FROM_AFISHA
	request_->request(":queries/movies_afisha.xq",
	                  QString("http://www.afisha.ru/msk/schedule_cinema/%1/")
	                  .arg(AfishaHelpers::currentDateForAfisha()));
#else
	request_->request(":queries/movies.xq",
	                  QString("%1/type.xml?city=MSK&type=cinema&date=%2&")
	                  .arg(AfishaHelpers::host())
	                  .arg(AfishaHelpers::currentDate()));
#endif
}

void Movies::initFromData(const QString& xml)
{
	qDeleteAll(movies_);
	movies_.clear();

	QDomDocument doc;
	if (!doc.setContent(xml))
		return;

	QDomElement root = doc.documentElement();
	Q_ASSERT(root.tagName() == "movies");
	if (root.tagName() != "movies")
		return;

	for (QDomNode n = root.firstChild(); !n.isNull(); n = n.nextSibling()) {
		QDomElement e = n.toElement();
		if (e.isNull())
			continue;

		if (e.tagName() == "movie") {
			Movie* movie = new Movie();
			movie->initFromXml(e);
			// movie->updateFromWeb();
			movies_ << movie;
		}
	}

	emit dataChanged();
}

void Movies::requestFinished()
{
	if (request_.isNull())
		return;

	if (!request_->error()) {
		initFromData(request_->result());
	}

	delete request_;
}
