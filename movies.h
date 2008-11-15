#ifndef MOVIES_H
#define MOVIES_H

#include <QObject>

#include <QList>
#include <QPointer>

class HttpRequest;
class Movie;

class Movies : public QObject
{
	Q_OBJECT
public:
	Movies();
	~Movies();

	void initFromWeb();
	void initFromData(const QString& xml);

private slots:
	void requestFinished();

private:
	QList<Movie*> movies_;
	QPointer<HttpRequest> request_;
};

#endif
