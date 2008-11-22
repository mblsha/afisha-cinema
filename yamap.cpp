#include "yamap.h"

#include <QDebug>
#include <QWebFrame>

YaMap::YaMap(QWidget* parent)
	: QWebView(parent)
{
	load(QUrl("http://mblsha.psi-im.org/tmp/map.html"));
}

YaMap::~YaMap()
{
}

void YaMap::clearPlaces()
{
	QWebFrame* frame = page()->mainFrame();
	frame->evaluateJavaScript(QString("clearPlaces();"));
}

void YaMap::addPlace(const QString& ll, const QString description)
{
	QWebFrame* frame = page()->mainFrame();
	QStringList llist = ll.split(" ");
	if (llist.count() != 2) {
		qWarning("error: '%s';'%s'", qPrintable(ll), qPrintable(description));
		return;
	}
	QString js = QString("addPlace('%1', '%2', '%3');")
	                          .arg(llist[0])
	                          .arg(llist[1])
	                          .arg(description);
	qWarning("js: '%s'", qPrintable(js));
	frame->evaluateJavaScript(js);
}
