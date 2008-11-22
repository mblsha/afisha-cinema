#ifndef CINEMALIST_H
#define CINEMALIST_H

#include <QObject>

#include <QList>
#include <QPointer>

class HttpRequest;
class Cinema;
class QTimer;

class CinemaList : public QObject
{
	Q_OBJECT
public:
	CinemaList();
	~CinemaList();

	Cinema* findCinema(const QString& id) const;

public slots:
	void init();
	void initFromWeb();
	void initFromData(const QString& xml, bool fromCache);

signals:
	void dataChanged();

private slots:
	void requestFinished();
	void cinemaDataChanged();
	void saveCache();

private:
	QList<Cinema*> cinemas_;
	QPointer<HttpRequest> request_;
	QTimer* saveCacheTimer_;

	QString cacheFileName() const;
};

#endif
