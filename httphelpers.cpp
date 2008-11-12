#include "httphelpers.h"

#include <QDebug>
#include <QHttp>
#include <QUrl>
#include <QRegExp>
#include <QTextCodec>

int HttpHelpers::httpGet(QHttp* http, const QString& urlString)
{
	QUrl url(urlString, QUrl::TolerantMode);
	Q_ASSERT(url.hasQuery());

	QString query = url.encodedQuery();
	query.replace("?", "&"); // FIXME: Bug in Qt 4.4.2?

	QHttpRequestHeader header("GET", url.path() + "?" + query);
	header.setValue("User-Agent", "Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10_5_5; en-us) AppleWebKit/525.18 (KHTML, like Gecko) Version/3.1.2 Safari/525.20.1");
	header.setValue("Host", url.port() == -1 ? url.host() : QString("%1:%2").arg(url.host(), url.port()));
	header.setValue("Accept-Language", "en-us");
	header.setValue("Accept", "*/*");

	QByteArray bytes;

	int contentLength = bytes.length();
	header.setContentLength(contentLength);

	http->setHost(url.host(), url.port() == -1 ? 80 : url.port());
	return http->request(header, bytes);
}

QString HttpHelpers::ensureUnicodeHtml(const QByteArray& data)
{
	QString tmp(data);
	QRegExp contentType("content=(?:\"|')text/html; charset=([\\w\\d-]+)(?:\"|')");
	if (contentType.indexIn(tmp)) {
		QTextCodec* codec = QTextCodec::codecForName(contentType.capturedTexts()[1].toLatin1());
		return codec->toUnicode(data);
	}

	return tmp;
}
