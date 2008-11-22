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
	                  .arg(AfishaHelpers::cinemaCacheDate())
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

		if (!address_.isEmpty() && ll_.isEmpty() && !XMLHelper::hasSubTag(e, "ll")) {
			Geocoder* geocoder = new Geocoder(QString("%1_geocoder_%2")
			                                  .arg(AfishaHelpers::cinemaCacheDate())
			                                  .arg(id_), this);
			request_ = geocoder;
			connect(geocoder, SIGNAL(finished()), SLOT(requestFinished()));
			geocoder->request(address_);
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
