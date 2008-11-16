#ifndef CINEMA_H
#define CINEMA_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QPointer>

class QDomElement;
class HttpRequest;

class Cinema : public QObject
{
	Q_OBJECT
public:
	Cinema();
	~Cinema();

	QString name() const;
	QString id() const;
	QString address() const;
	QString metro() const;

	void initFromData(const QString& xml);
	void initFromXml(const QDomElement& element);
	void updateFromWeb();

	void clear();

private slots:
	void requestFinished();

private:
	QString id_;
	QString name_;
	QString address_;
	QString metro_;
	QString details_;
	QString mapUrl_;
	QStringList phones_;
	QDateTime lastUpdatedAt_;

	QPointer<HttpRequest> request_;
};

#endif
