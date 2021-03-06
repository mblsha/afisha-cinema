/*
 * httphelpers.cpp
 * Copyright (C) 2008  Michail Pishchagin
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "httphelpers.h"

#include <QDebug>
#include <QHttp>
#include <QUrl>
#include <QRegExp>
#include <QTextCodec>
#include <QProcess>
#include <QtXmlPatterns>

int HttpHelpers::httpGet(QHttp* http, const QString& urlString)
{
	QUrl url(urlString, QUrl::TolerantMode);
	// Q_ASSERT(url.hasQuery());

	QString query = url.encodedQuery();
	query.replace("?", "&"); // FIXME: Bug in Qt 4.4.2?

	QString fullUri = url.path();
	if (!query.isEmpty()) {
		fullUri += "?" + query;
	}

	// qWarning("fullUri = '%s'", qPrintable(fullUri));
	QHttpRequestHeader header("GET", fullUri);
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
	QString tmp = QString::fromUtf8(data);
	QRegExp contentType("content=(?:\"|')text/html;? charset=([\\w\\d-]+)(?:\"|')");
	if (contentType.indexIn(tmp) != -1) {
		QString charset = contentType.capturedTexts()[1];
		if (charset == "utf-8" || charset.simplified().isEmpty()) {
			return tmp;
		}
		QTextCodec* codec = QTextCodec::codecForName(charset.toLatin1());
		return codec->toUnicode(data);
	}

	return tmp;
}

QString HttpHelpers::htmlToXml(const QString& html)
{
	QRegExp xml("<\\?xml.+\\?>");
	// QRegExp xhtml("W3C\\/\\/DTD XHTML"); // afisha's xhtml is not valid xhtml
	QString result = html;
	if (xml.indexIn(html) == -1 /*&& xhtml.indexIn(html) == -1*/) {
		// bacek@:
		// $ xmllint |grep html
		//     --htmlout : output results as HTML
		//     --html : use the HTML parser
		//     --xmlout : force to use the XML serializer when using --html
		//
		// В API что-то вроде xmlBlahBlahRecover.

		QProcess tidy;
		tidy.start("tidy", QStringList() << "-asxhtml" << "-numeric" << "-utf8");
		if (!tidy.waitForStarted()) {
			qWarning("tidy!waitForStarted");
			return html;
		}

		tidy.write(html.toUtf8());
		tidy.closeWriteChannel();

		if (!tidy.waitForFinished()) {
			qWarning("tidy!waitForFinished");
			return html;
		}

		result = QString::fromUtf8(tidy.readAll());
	}

	QRegExp xmlns("xmlns\\s*=\\s*\".+\"");
	xmlns.setMinimal(true);
	result.replace(xmlns, "");
	return result;
}

QString HttpHelpers::xmlQueryStringResult(const QString& _query, const QString& _data)
{
	QXmlQuery query;

	QByteArray data = _data.toUtf8();
	QBuffer sourceDocument(&data);
	sourceDocument.open(QIODevice::ReadOnly);
	query.bindVariable("input", &sourceDocument);

	query.setQuery(_query);

	if (!query.isValid())
		return QString();

	QByteArray outArray;
	QBuffer buffer(&outArray);
	buffer.open(QIODevice::ReadWrite);

	QXmlFormatter formatter(query, &buffer);

	if (!query.evaluateTo(&formatter))
		return QString();

	buffer.close();
	return QString::fromUtf8(outArray.constData());
}

QString HttpHelpers::xmlQueryResult(const QString& queryFileName, const QString& data)
{
	QFile queryFile(queryFileName);
	if (!queryFile.open(QFile::ReadOnly))
		return QString();

	return xmlQueryStringResult(QString::fromUtf8(queryFile.readAll()), data);
}

QString HttpHelpers::xmlQueryHtmlResult(const QString& queryFileName, const QByteArray& data)
{
	QString html = HttpHelpers::ensureUnicodeHtml(data);
	html = HttpHelpers::htmlToXml(html);
	return xmlQueryResult(queryFileName, html);
}
