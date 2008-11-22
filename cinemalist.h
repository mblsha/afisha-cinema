/*
 * cinemalist.h
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

#ifndef CINEMALIST_H
#define CINEMALIST_H

#include <QObject>

#include <QList>
#include <QPointer>

class HttpRequest;
class Cinema;
class QTimer;

class CinemaList : public QObject
{
	Q_OBJECT
public:
	CinemaList();
	~CinemaList();

	Cinema* findCinema(const QString& id) const;

public slots:
	void init();
	void initFromWeb();
	void initFromData(const QString& xml, bool fromCache);

signals:
	void dataChanged();

private slots:
	void requestFinished();
	void cinemaDataChanged();
	void saveCache();

private:
	QList<Cinema*> cinemas_;
	QPointer<HttpRequest> request_;
	QTimer* saveCacheTimer_;

	QString cacheFileName() const;
};

#endif
