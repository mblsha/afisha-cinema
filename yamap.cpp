/*
 * yamap.cpp
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

#include "yamap.h"

#include <QDebug>
#include <QWebFrame>
#include <QDesktopServices>

YaMap::YaMap(QWidget* parent)
	: QWebView(parent)
{
	page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);
	connect(page(), SIGNAL(linkClicked(const QUrl &)), SLOT(linkClicked(const QUrl &)));

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
		// qWarning("error: '%s';'%s'", qPrintable(ll), qPrintable(description));
		return;
	}
	QString js = QString("addPlace('%1', '%2', '%3');")
	                          .arg(llist[0])
	                          .arg(llist[1])
	                          .arg(description);
	// qWarning("js: '%s'", qPrintable(js));
	frame->evaluateJavaScript(js);
}

void YaMap::linkClicked(const QUrl& url)
{
	QDesktopServices::openUrl(url);
}
