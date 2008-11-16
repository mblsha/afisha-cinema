#include "movies.h"

#include <QDebug>
#include <QtAlgorithms>
#include <QDomDocument>
#include <QDomElement>

#include "xmpp_xmlcommon.h"
#include "httprequest.h"
#include "afishahelpers.h"
#include "movie.h"

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

void Movies::initFromWeb()
{
	if (request_)
		delete request_;
	request_ = new HttpRequest(QString("%1_movies").arg(AfishaHelpers::currentDate()), this);
	connect(request_, SIGNAL(finished()), SLOT(requestFinished()));
	request_->request(":queries/movies.xq",
	                  QString("%1/type.xml?city=MSK&type=cinema&date=%2&")
	                  .arg(AfishaHelpers::host())
	                  .arg(AfishaHelpers::currentDate()));
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
