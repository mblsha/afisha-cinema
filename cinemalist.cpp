#include "cinemalist.h"

#include <QDebug>
#include <QtAlgorithms>
#include <QDomDocument>
#include <QDomElement>

#include "xmpp_xmlcommon.h"
#include "httprequest.h"
#include "afishahelpers.h"
#include "cinema.h"

CinemaList::CinemaList()
	: QObject()
{
}

CinemaList::~CinemaList()
{
}

void CinemaList::initFromWeb()
{
	if (request_)
		delete request_;
	request_ = new HttpRequest(QString("%1_cinemas").arg(AfishaHelpers::currentDate()), this);
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
		initFromData(request_->result());
	}

	delete request_;
}

void CinemaList::initFromData(const QString& xml)
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

	for (QDomNode n = root.firstChild(); !n.isNull(); n = n.nextSibling()) {
		QDomElement e = n.toElement();
		if (e.isNull())
			continue;

		if (e.tagName() == "cinema") {
			Cinema* cinema = new Cinema();
			cinema->initFromXml(e);
			cinema->updateFromWeb();
			cinemas_ << cinema;
		}
	}

	emit dataChanged();
}
