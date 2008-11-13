#ifndef HTTPHELPERS_H
#define HTTPHELPERS_H

class QHttp;
class QString;
class QByteArray;

namespace HttpHelpers {
	int httpGet(QHttp* http, const QString& url);
	QString ensureUnicodeHtml(const QByteArray& html);
	QString htmlToXml(const QString& html);
	QString xmlQueryResult(const QString& query, const QString& data);
};

#endif
