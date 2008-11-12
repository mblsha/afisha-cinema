#ifndef HTTPHELPERS_H
#define HTTPHELPERS_H

class QHttp;
class QString;
class QByteArray;

namespace HttpHelpers {
	int httpGet(QHttp* http, const QString& url);
	QString ensureUnicodeHtml(const QByteArray& html);
};

#endif
