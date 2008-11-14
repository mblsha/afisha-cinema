#include "afishahelpers.h"

#include <QDateTime>

QString AfishaHelpers::currentDate()
{
	return QDateTime::currentDateTime().toString("yyyy-MM-dd");
}
