#ifndef CINEMA_H
#define CINEMA_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDateTime>

class QDomElement;

class Cinema : public QObject
{
	Q_OBJECT
public:
	Cinema();
	~Cinema();

	void initFromUrl(const QString& url);
	void initFromData(const QDomElement& element);

private:
	QString id_;
	QString address_;
	QString mapUrl_;
	QStringList phones_;
	QDateTime lastUpdatedAt_;
};

#endif
