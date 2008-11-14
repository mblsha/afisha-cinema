#include <QApplication>
#include <QTimer>

#include "cinemalist.h"

int main (int argc, char *argv[])
{
	QApplication app(argc, argv);

	CinemaList cinemaList;
	cinemaList.initFromWeb();

	// QTimer::singleShot(0, &app, SLOT(quit()));
	return app.exec();
}
