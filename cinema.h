#ifndef CINEMA_H
#define CINEMA_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QPointer>

class QDomElement;
class QDomDocument;
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
	QString ll() const;
	bool hasDetailedInfo() const;

	void initFromData(const QString& xml);
	void initFromXml(const QDomElement& element);
	void updateFromWeb();

	QDomElement toXml(QDomDocument* doc) const;

	void clear();

	static QString detailsLinkForId(const QString& id);

signals:
	void dataChanged();

private slots:
	void requestFinished();

private:
	QString id_;
	QString name_;
	QString address_;
	QString metro_;
	QString details_;
	QString ll_;
	QString mapUrl_;
	QStringList phones_;
	QDateTime lastUpdatedAt_;
	bool hasDetailedInfo_;

	QPointer<HttpRequest> request_;
};

#endif
