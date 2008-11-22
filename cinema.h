/*
 * cinema.h
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

#ifndef CINEMA_H
#define CINEMA_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QPointer>

class QDomElement;
class QDomDocument;
class HttpRequest;

class Cinema : public QObject
{
	Q_OBJECT
public:
	Cinema();
	~Cinema();

	QString name() const;
	QString id() const;
	QString address() const;
	QString metro() const;
	QString ll() const;
	bool hasDetailedInfo() const;

	void initFromData(const QString& xml);
	void initFromXml(const QDomElement& element);
	void updateFromWeb();

	QDomElement toXml(QDomDocument* doc) const;

	void clear();

	static QString detailsLinkForId(const QString& id);

signals:
	void dataChanged();

private slots:
	void requestFinished();

private:
	QString id_;
	QString name_;
	QString address_;
	QString metro_;
	QString details_;
	QString ll_;
	QString mapUrl_;
	QStringList phones_;
	QDateTime lastUpdatedAt_;
	bool hasDetailedInfo_;

	QPointer<HttpRequest> request_;
};

#endif
