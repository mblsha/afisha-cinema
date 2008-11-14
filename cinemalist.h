#ifndef CINEMALIST_H
#define CINEMALIST_H

#include <QObject>

#include "cinema.h"

#include <QList>
#include <QPointer>

class QHttp;
class HttpRequest;

class CinemaList : public QObject
{
	Q_OBJECT
public:
	CinemaList();
	~CinemaList();

	void initFromWeb();
	void initFromData(const QString& xml);

private slots:
	void requestFinished();

private:
	QList<Cinema*> cinemas_;
	QPointer<HttpRequest> request_;
};

#endif
