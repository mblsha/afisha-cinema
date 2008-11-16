#include "mainwindow.h"

#include <QDebug>
#include <QListWidgetItem>
#include <QMap>
#include <QMapIterator>
#include <QTimer>

#include "cinemalist.h"
#include "movies.h"
#include "cinema.h"
#include "movie.h"

MainWindow::MainWindow()
	: QWidget()
{
	ui_.setupUi(this);

	connect(ui_.movies, SIGNAL(itemSelectionChanged()), SLOT(moviesSelectionChanged()));
	connect(ui_.times, SIGNAL(itemSelectionChanged()), SLOT(timesSelectionChanged()));
	connect(ui_.cinemas, SIGNAL(itemSelectionChanged()), SLOT(cinemasSelectionChanged()));

	cinemas_ = new CinemaList();
	QTimer::singleShot(100, cinemas_, SLOT(initFromWeb()));
	// cinemas_->initFromWeb();

	movies_ = new Movies();
	connect(movies_, SIGNAL(dataChanged()), SLOT(moviesChanged()));
	movies_->initFromWeb();
}

MainWindow::~MainWindow()
{
	delete cinemas_;
	delete movies_;
}

void MainWindow::moviesChanged()
{
	ui_.movies->setUpdatesEnabled(false);
	ui_.movies->clear();
	foreach(Movie* movie, movies_->movies()) {
		QListWidgetItem* item = new QListWidgetItem(ui_.movies);
		item->setText(movie->name());
		item->setData(Qt::UserRole, movie->id());
	}
	ui_.movies->setUpdatesEnabled(true);
}

void MainWindow::cinemasChanged()
{
}

void MainWindow::movieChanged()
{
	ui_.times->setUpdatesEnabled(false);
	ui_.times->clear();

	if (movie_) {
		foreach(QString t, movie_->times()) {
			QListWidgetItem* item = new QListWidgetItem(ui_.times);
			item->setText(t);
			item->setData(Qt::UserRole, t);
		}
	}

	ui_.times->setUpdatesEnabled(true);
}

void MainWindow::moviesSelectionChanged()
{
	movie_ = 0;
	ui_.times->setUpdatesEnabled(false);
	ui_.times->clear();

	if (ui_.movies->currentItem()) {
		movie_ = movies_->findMovie(ui_.movies->currentItem()->data(Qt::UserRole).toString());
		if (movie_) {
			connect(movie_, SIGNAL(dataChanged()), SLOT(movieChanged()));
			movie_->updateFromWeb();
		}
	}

	ui_.times->setUpdatesEnabled(true);
}

void MainWindow::timesSelectionChanged()
{
	QStringList times;
	foreach(QListWidgetItem* item, ui_.times->selectedItems()) {
		times += item->data(Qt::UserRole).toString();
	}

	ui_.cinemas->setUpdatesEnabled(false);
	ui_.cinemas->clear();

	if (movie_) {
		QMap<QString, QStringList> cinemas = movie_->cinemasForTimes(times);
		QMapIterator<QString, QStringList> it(cinemas);
		while (it.hasNext()) {
			it.next();

			Cinema* cinema = cinemas_->findCinema(it.key());
			if (!cinema) {
				qWarning("Cinema not found: '%s'", qPrintable(it.key()));
				continue;
			}

			Q_ASSERT(cinema);
			QListWidgetItem* item = new QListWidgetItem(ui_.cinemas);
			item->setText(cinema->name());
			item->setData(Qt::UserRole, cinema->id());
		}
	}

	ui_.cinemas->setUpdatesEnabled(true);
}

void MainWindow::cinemasSelectionChanged()
{
	ui_.webView->clearPlaces();
	foreach(QListWidgetItem* item, ui_.cinemas->selectedItems()) {
		Cinema* cinema = cinemas_->findCinema(item->data(Qt::UserRole).toString());
		if (cinema) {
			ui_.webView->addPlace(cinema->ll(), cinema->name());
		}
	}
}
