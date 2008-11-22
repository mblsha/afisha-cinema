/*
 * movie.h
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

#ifndef MOVIE_H
#define MOVIE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QPointer>
#include <QMap>

class QDomElement;
class HttpRequest;

class Movie : public QObject
{
	Q_OBJECT
public:
	Movie();
	~Movie();

	QString name() const;
	QString id() const;

	QStringList times() const;
	QMap<QString, QStringList> cinemasForTimes(const QStringList& times);
	QStringList timesForCinema(const QString& cinemaId);

	void initFromData(const QString& xml);
	void initFromXml(const QDomElement& element);
	void updateFromWeb();

	void clear();

signals:
	void dataChanged();

private slots:
	void requestFinished();

private:
	QString id_;
	QString name_;
	QMap<QString, QStringList> cinemas_;
	QDateTime lastUpdatedAt_;

	QPointer<HttpRequest> request_;

	QStringList sortTimes(const QStringList& times) const;
};

#endif
