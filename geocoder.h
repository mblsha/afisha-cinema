/*
 * geocoder.h
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

#ifndef GEOCODER_H
#define GEOCODER_H

#include "httprequest.h"

class QDomElement;

class Geocoder : public HttpRequest
{
	Q_OBJECT
public:
	Geocoder(QString id, QObject* parent = 0);
	~Geocoder();

	QString address() const;
	QString ll() const;

	void request(const QString& address);

private slots:
	// reimplemented
	virtual void delayedFinished();

private:
	QString address_;
	QString ll_;

	void clear();
	void initFromData(const QString& xml);
	void initFromXml(const QDomElement& element);

	QString processAddress(const QString& address) const;
};

#endif
