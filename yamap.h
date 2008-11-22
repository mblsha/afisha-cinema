#ifndef YAMAP_H
#define YAMAP_H

#include <QWebView>

class QNetworkAccessManager;

class YaMap : public QWebView
{
	Q_OBJECT
public:
	YaMap(QWidget* parent);
	~YaMap();

	void clearPlaces();
	void addPlace(const QString& ll, const QString description);

private slots:
	void linkClicked(const QUrl& url);
};

#endif
