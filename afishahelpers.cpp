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

QString AfishaHelpers::currentDateForAfisha()
{
	return QDateTime::currentDateTime().toString("dd-MM-yyyy");
}

QString AfishaHelpers::cinemaCacheDate()
{
	return "2008-11-15";
}