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
	                  .arg(AfishaHelpers::cinemaCacheDate()));
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
	return QString("cache/cinemas.xml");
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
	root.setAttribute("detailed", "true");
	root.setAttribute("lastUpdatedAt", QDateTime::currentDateTime().toString(Qt::ISODate));
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
