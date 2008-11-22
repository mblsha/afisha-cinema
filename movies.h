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

	QList<Movie*> movies() const;
	Movie* findMovie(const QString& id) const;

	void init();
	void initFromWeb();
	void initFromData(const QString& xml);

signals:
	void dataChanged();

private slots:
	void requestFinished();

private:
	QList<Movie*> movies_;
	QPointer<HttpRequest> request_;
};

#endif
