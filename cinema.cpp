#include "cinema.h"

#include <QDebug>
#include <QDomElement>
#include <QDomDocument>

#include "xmpp_xmlcommon.h"
#include "httprequest.h"
#include "afishahelpers.h"

Cinema::Cinema()
	: QObject()
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

void Cinema::updateFromWeb()
{
	Q_ASSERT(!id_.isEmpty());
	if (request_)
		delete request_;
	request_ = new HttpRequest(QString("%1_cinema_%2")
	                           .arg(AfishaHelpers::currentDate())
	                           .arg(id_), this);
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
		initFromData(request_->result());
	}

	delete request_;
}

void Cinema::initFromData(const QString& xml)
{
	QDomDocument doc;
	if (!doc.setContent(xml))
		return;

	initFromXml(doc.documentElement());
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

	if (!e.attribute("detailed").isEmpty()) {
		// if (!details_.isEmpty())
			// qWarning() << id_ << name_ << address_ << metro_;
	}
}

void Cinema::clear()
{
	id_ = QString();
	name_ = QString();
	address_ = QString();
	metro_ = QString();
	details_ = QString();
	mapUrl_ = QString();
	phones_ = QStringList();
	lastUpdatedAt_ = QDateTime::currentDateTime();
}
