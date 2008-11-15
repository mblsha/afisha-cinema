#include "movie.h"

#include <QDebug>
#include <QDomElement>
#include <QDomDocument>

#include "xmpp_xmlcommon.h"
#include "httprequest.h"
#include "afishahelpers.h"

Movie::Movie()
	: QObject()
{
}

Movie::~Movie()
{
}

void Movie::initFromData(const QString& xml)
{
	QDomDocument doc;
	if (!doc.setContent(xml))
		return;

	initFromXml(doc.documentElement());
}

void Movie::initFromXml(const QDomElement& e)
{
	Q_ASSERT(e.tagName() == "movie");
	clear();

	id_ = XMLHelper::subTagText(e, "id");
	name_ = XMLHelper::subTagText(e, "name");

	for (QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling()) {
		QDomElement ee = n.toElement();
		if (ee.isNull())
			continue;

		if (ee.tagName() == "cinema") {
			QString name = ee.attribute("name");
			QString id = ee.attribute("id");
			QStringList times;

			for (QDomNode nn = ee.firstChild(); !nn.isNull(); nn = nn.nextSibling()) {
				QDomElement eee = nn.toElement();
				if (eee.isNull())
					continue;

				if (eee.tagName() == "time") {
					times << eee.text();
				}
			}

			cinemas_[id] = times;
		}
	}

	if (e.attribute("detailed").isEmpty()) {
		if (id_ == "190306" || id_ == "171644" || id_ == "181518") {
			updateFromWeb();
		}
	}
	else {
		qWarning() << id_ << name_;
		qWarning() << cinemas_;
	}
}

void Movie::updateFromWeb()
{
	Q_ASSERT(!id_.isEmpty());
	if (request_)
		delete request_;
	request_ = new HttpRequest(QString("%1_movie_%2")
	                           .arg(AfishaHelpers::currentDate())
	                           .arg(id_), this);
	connect(request_, SIGNAL(finished()), SLOT(requestFinished()));

	// in order to get times we should fetch with '&page=1'
	request_->request(":queries/movie.xq",
	                  QString("http://afisha.yandex.ru/event.xml?event_id=%2&date=%1&page=1")
	                  .arg(AfishaHelpers::currentDate())
	                  .arg(id_));
}

void Movie::clear()
{
	id_ = QString();
	name_ = QString();
	cinemas_.clear();
	lastUpdatedAt_ = QDateTime::currentDateTime();
}

void Movie::requestFinished()
{
	if (request_.isNull()) {
		return;
	}

	if (!request_->error()) {
		initFromData(request_->result());
	}

	delete request_;
}
