#ifndef MOVIE_H
#define MOVIE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QPointer>
#include <QMap>

class QDomElement;
class HttpRequest;

class Movie : public QObject
{
	Q_OBJECT
public:
	Movie();
	~Movie();

	QString name() const;
	QString id() const;

	QStringList times() const;
	QMap<QString, QStringList> cinemasForTimes(const QStringList& times);

	void initFromData(const QString& xml);
	void initFromXml(const QDomElement& element);
	void updateFromWeb();

	void clear();

signals:
	void dataChanged();

private slots:
	void requestFinished();

private:
	QString id_;
	QString name_;
	QMap<QString, QStringList> cinemas_;
	QDateTime lastUpdatedAt_;

	QPointer<HttpRequest> request_;

	QStringList sortTimes(const QStringList& times) const;
};

#endif
