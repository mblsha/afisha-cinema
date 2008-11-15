#include <QApplication>
#include <QTimer>

#include "cinemalist.h"
#include "movies.h"

int main (int argc, char *argv[])
{
	QApplication app(argc, argv);

	// CinemaList cinemaList;
	// cinemaList.initFromWeb();

	Movies movies;
	movies.initFromWeb();

	// QTimer::singleShot(1000, &app, SLOT(quit()));
	return app.exec();
}
