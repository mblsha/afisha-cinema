#ifndef CINEMALIST_H
#define CINEMALIST_H

#include <QObject>

#include <QList>
#include <QPointer>

class HttpRequest;
class Cinema;

class CinemaList : public QObject
{
	Q_OBJECT
public:
	CinemaList();
	~CinemaList();

	void initFromWeb();
	void initFromData(const QString& xml);

signals:
	void dataChanged();

private slots:
	void requestFinished();

private:
	QList<Cinema*> cinemas_;
	QPointer<HttpRequest> request_;
};

#endif
