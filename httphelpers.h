/*
 * httphelpers.h
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

#ifndef HTTPHELPERS_H
#define HTTPHELPERS_H

class QHttp;
class QString;
class QByteArray;

namespace HttpHelpers {
	int httpGet(QHttp* http, const QString& url);
	QString ensureUnicodeHtml(const QByteArray& html);
	QString htmlToXml(const QString& html);
	QString xmlQueryStringResult(const QString& query, const QString& data);
	QString xmlQueryResult(const QString& queryFileName, const QString& data);
	QString xmlQueryHtmlResult(const QString& queryFileName, const QByteArray& html);
};

#endif
