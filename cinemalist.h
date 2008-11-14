#ifndef CINEMALIST_H
#define CINEMALIST_H

#include <QObject>

#include "cinema.h"

#include <QList>

class QHttp;

class CinemaList : public QObject
{
	Q_OBJECT
public:
	CinemaList();
	~CinemaList();

	void initFromWeb();
	void initFromData(const QByteArray& data);

private slots:
	void httpRequestFinished(int requestId, bool error);

private:
	QList<Cinema*> cinemas_;
	QHttp* http_;
	int httpRequestId_;
};

#endif
