/*
 * httprequest.cpp
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

#include "httprequest.h"

#include <QDebug>
#include <QHttp>
#include <QUrl>
#include <QFile>
#include <QTimer>
#include <QCoreApplication>

#include "httphelpers.h"
#include "afishahelpers.h"

//----------------------------------------------------------------------------
// HttpRequestQueue
//----------------------------------------------------------------------------

class HttpRequestQueue : public QObject
{
	Q_OBJECT
public:
	static HttpRequestQueue* instance()
	{
		static HttpRequestQueue* instance_ = 0;
		if (!instance_) {
			instance_ = new HttpRequestQueue();
		}
		return instance_;
	}

	void queue(HttpRequest* request)
	{
		queue_ << request;
		connect(request, SIGNAL(destroyed(QObject*)), SLOT(requestDestroyed(QObject*)));
		updateTimer();
	}

private slots:
	void requestDestroyed(QObject* obj)
	{
		HttpRequest* request = static_cast<HttpRequest*>(obj);
		queue_.removeAll(request);
	}

	void timeout()
	{
		if (queue_.isEmpty())
			return;

		HttpRequest* request = queue_.takeFirst();
		request->start();
		updateTimer();
	}

	void updateTimer()
	{
		if (queue_.isEmpty()) {
			timer_->stop();
			return;
		}

		if (!timer_->isActive()) {
			timer_->start();
		}
	}

private:
	HttpRequestQueue()
		: QObject(QCoreApplication::instance())
	{
		timer_ = new QTimer(this);
		timer_->setSingleShot(true);
		timer_->setInterval(1000);
		connect(timer_, SIGNAL(timeout()), SLOT(timeout()));
	}

	QList<HttpRequest*> queue_;
	QTimer* timer_;
};

//----------------------------------------------------------------------------
// HttpRequest
//----------------------------------------------------------------------------

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

QString HttpRequest::type() const
{
	return type_;
}

void HttpRequest::setType(const QString& type)
{
	type_ = type;
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
	url_ = url;

	if (QFile::exists(cacheFileName())) {
		QFile file(cacheFileName());
		if (file.open(QFile::ReadOnly)) {
			processData(file.readAll());
			return;
		}
	}

	HttpRequestQueue::instance()->queue(this);
}

void HttpRequest::start()
{
	qWarning("HttpRequest::start(): '%s' (%s)", qPrintable(url_), qPrintable(cacheFileName()));
	httpRequestId_ = HttpHelpers::httpGet(http_, url_);
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
	return QString("%1/%2.data")
	       .arg(AfishaHelpers::cacheDir())
	       .arg(id_);
}

void HttpRequest::processData(const QByteArray& data)
{
	result_ = HttpHelpers::xmlQueryHtmlResult(queryFileName_, data);
	QTimer::singleShot(0, this, SLOT(delayedFinished()));

	QFile file(QString("%1/%2.xml")
	           .arg(AfishaHelpers::cacheDir())
	           .arg(id_));
	if (file.open(QFile::WriteOnly)) {
		QString html = HttpHelpers::ensureUnicodeHtml(data);
		html = HttpHelpers::htmlToXml(html);

		file.write(html.toUtf8());
	}
}

#include "httprequest.moc"
