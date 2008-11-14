#include "httprequest.h"

#include <QHttp>
#include <QUrl>
#include <QFile>
#include <QTimer>

#include "httphelpers.h"

HttpRequest::HttpRequest(QString id, QObject* parent)
	: QObject(parent)
	, id_(id)
	, error_(false)
	, http_(0)
	, httpRequestId_(-1)
{
	http_ = new QHttp(this);
	connect(http_, SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestFinished(int, bool)));
}

HttpRequest::~HttpRequest()
{
}

QString HttpRequest::id() const
{
	return id_;
}

bool HttpRequest::error() const
{
	return error_;
}

QString HttpRequest::result() const
{
	return result_;
}

void HttpRequest::request(const QString& queryFileName, const QString& url)
{
	queryFileName_ = queryFileName;

	if (QFile::exists(cacheFileName())) {
		QFile file(cacheFileName());
		if (file.open(QFile::ReadOnly)) {
			processData(file.readAll());
			return;
		}
	}

	httpRequestId_ = HttpHelpers::httpGet(http_, url);
}

void HttpRequest::httpRequestFinished(int requestId, bool error)
{
	if (error) {
		error_ = true;
		qWarning("HttpRequest::httpRequestFinished(%s): error", qPrintable(id_));
		QTimer::singleShot(0, this, SLOT(delayedFinished()));
		return;
	}

	if (requestId != httpRequestId_) {
		// error_ = true;
		// qWarning("HttpRequest::httpRequestFinished(%s): requestId(%d) != httpRequestId_(%d)", qPrintable(id_), requestId, httpRequestId_);
		// QTimer::singleShot(0, this, SLOT(delayedFinished()));
		return;
	}

	QByteArray data = http_->readAll();
	QFile file(cacheFileName());
	if (file.open(QFile::WriteOnly)) {
		file.write(data);
	}

	processData(data);
}

void HttpRequest::delayedFinished()
{
	emit finished();
}

QString HttpRequest::cacheFileName() const
{
	return QString("cache/%1.data").arg(id_);
}

void HttpRequest::processData(const QByteArray& data)
{
	result_ = HttpHelpers::xmlQueryHtmlResult(queryFileName_, data);
	QTimer::singleShot(0, this, SLOT(delayedFinished()));
}
