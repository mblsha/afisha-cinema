#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>

class QHttp;

class HttpRequest : public QObject
{
	Q_OBJECT
public:
	HttpRequest(QString id, QObject* parent = 0);
	~HttpRequest();

	QString type() const;
	void setType(const QString& type);

	QString id() const;
	bool error() const;
	QString result() const;

	void request(const QString& queryFileName, const QString& url);

signals:
	void finished();

private slots:
	virtual void start();
	void httpRequestFinished(int requestId, bool error);

protected slots:
	virtual void delayedFinished();

private:
	QString id_;
	QString type_;
	bool error_;
	QString queryFileName_;
	QHttp* http_;
	int httpRequestId_;
	QString url_;
	QString result_;

	QString cacheFileName() const;
	void processData(const QByteArray& data);
	friend class HttpRequestQueue;
};

#endif
