#include "cinemalist.h"

#include <QDebug>
#include <QHttp>
#include <QDomDocument>
#include <QDomElement>
#include <QUrl>
#include <QFile>
#include <QtXmlPatterns>

#include "httphelpers.h"

CinemaList::CinemaList()
	: QObject()
	, http_(0)
	, httpRequestId_(-1)
{
	http_ = new QHttp(this);
	connect(http_, SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestFinished(int, bool)));
}

CinemaList::~CinemaList()
{
}

void CinemaList::initFromWeb()
{
	// QString urlString = "http://pda.afisha.yandex.ru/chooser_place.xml?type=cinema&date=2008-11-13&";
	// httpRequestId_ = HttpHelpers::httpGet(http_, urlString);

	QFile file("cinemalist-cache.html");
	if (file.open(QFile::ReadOnly)) {
		initFromData(file.readAll());
	}
}

void CinemaList::httpRequestFinished(int requestId, bool error)
{
	if (error) {
		qWarning("Bah! Error!");
		return;
	}

	if (requestId != httpRequestId_) {
		// qWarning("requestId != httpRequestId_");
		return;
	}

	QFile file("cinemalist-cache.html");
	if (file.open(QFile::WriteOnly)) {
		file.write(http_->readAll());
	}
}

void CinemaList::initFromData(const QByteArray& data)
{
	QString html = HttpHelpers::ensureUnicodeHtml(data);
	// qWarning("%s", qPrintable(html));
	QXmlQuery query;
	query.setQuery("a");
	
}