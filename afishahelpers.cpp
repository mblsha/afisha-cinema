/*
 * afishahelpers.cpp
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

#include "afishahelpers.h"

#include <QDateTime>
#include <QDir>

#if defined(Q_WS_WIN)
#if __GNUC__ >= 3
#	define WINVER    0x0500
#	define _WIN32_IE 0x0500
#endif
#include <windows.h>
#include <shlobj.h>
#endif

#if defined(Q_WS_X11) or defined(Q_WS_MAC)
#include <sys/stat.h> // chmod
#endif

// ripped from yapsi
QString AfishaHelpers::homeDir()
{
	// Try the environment override first
	char *p = getenv("AFISHACINEMADATADIR");
	if (p) {
		QDir proghome(p);
		if (!proghome.exists()) {
			QDir d;
			d.mkpath(proghome.path());
		}
		return proghome.path();
	}

#if defined(Q_WS_X11) || defined(Q_WS_MAC)
	QDir proghome(QDir::homePath() + "/.afisha-cinema");
	if (!proghome.exists()) {
		QDir home = QDir::home();
		home.mkdir(".afisha-cinema");
		chmod(QFile::encodeName(proghome.path()), 0700);
	}
	return proghome.path();
#elif defined(Q_WS_WIN)
	QString base = QDir::homePath();
	WCHAR str[MAX_PATH+1] = { 0 };
	if (SHGetSpecialFolderPathW(0, str, CSIDL_APPDATA, true))
		base = QString::fromWCharArray(str);

	QDir proghome(base + "/AfishaCinemaData");
	if (!proghome.exists()) {
		QDir home(base);
		home.mkdir("AfishaCinemaData");
	}

	return proghome.path();
#endif
}

QString AfishaHelpers::cacheDir()
{
	QString dir(homeDir() + "/cache");
	QDir d(dir);
	if (!d.exists()) {
		QDir d(homeDir());
		d.mkdir("cache");
	}
	return dir;
}

QString AfishaHelpers::host()
{
	return "http://pda.afisha.yandex.ru";
}

QString AfishaHelpers::currentDate()
{
	return QDateTime::currentDateTime().toString("yyyy-MM-dd");
}

QString AfishaHelpers::currentDateForAfisha()
{
	return QDateTime::currentDateTime().toString("dd-MM-yyyy");
}

QString AfishaHelpers::cinemaCacheDate()
{
	return "2008-11-15";
}