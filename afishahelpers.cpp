#include "afishahelpers.h"

#include <QDateTime>

QString AfishaHelpers::host()
{
	return "http://pda.afisha.yandex.ru";
}

QString AfishaHelpers::currentDate()
{
	return QDateTime::currentDateTime().toString("yyyy-MM-dd");
}

QString AfishaHelpers::cinemaCacheDate()
{
	return "2008-11-15";
}