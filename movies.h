/*
 * movies.h
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

#ifndef MOVIES_H
#define MOVIES_H

#include <QObject>

#include <QList>
#include <QPointer>

class HttpRequest;
class Movie;

class Movies : public QObject
{
	Q_OBJECT
public:
	Movies();
	~Movies();

	QList<Movie*> movies() const;
	Movie* findMovie(const QString& id) const;

	void init();
	void initFromWeb();
	void initFromData(const QString& xml);

signals:
	void dataChanged();

private slots:
	void requestFinished();

private:
	QList<Movie*> movies_;
	QPointer<HttpRequest> request_;
};

#endif
