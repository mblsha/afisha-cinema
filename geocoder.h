#ifndef GEOCODER_H
#define GEOCODER_H

#include "httprequest.h"

class QDomElement;

class Geocoder : public HttpRequest
{
	Q_OBJECT
public:
	Geocoder(QString id, QObject* parent = 0);
	~Geocoder();

	QString address() const;
	QString ll() const;

	void request(const QString& address);

private slots:
	// reimplemented
	virtual void delayedFinished();

private:
	QString address_;
	QString ll_;

	void clear();
	void initFromData(const QString& xml);
	void initFromXml(const QDomElement& element);

	QString processAddress(const QString& address) const;
};

#endif
