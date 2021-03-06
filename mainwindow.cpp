/*
 * mainwindow.cpp
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
	cinemas_->setCanInitFromWeb(false);
	QTimer::singleShot(100, cinemas_, SLOT(init()));
	// cinemas_->init();

	movies_ = new Movies();
	connect(movies_, SIGNAL(dataChanged()), SLOT(moviesChanged()));
	movies_->init();
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
			if (movie_->times().isEmpty()) {
				connect(movie_, SIGNAL(dataChanged()), SLOT(movieChanged()));
				movie_->updateFromWeb();
			}
			else {
				movieChanged();
			}
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
			QStringList times;
			if (movie_) {
				QStringList timesForCinema = movie_->timesForCinema(cinema->id());
				foreach(QListWidgetItem* item, ui_.times->selectedItems()) {
					QString t = item->data(Qt::UserRole).toString();
					if (timesForCinema.contains(t))
						times << t;
				}
			}

			QString html = QString("<a href=\"%1\">%2</a><p>%3</p>")
			               .arg(Cinema::detailsLinkForId(cinema->id()))
			               .arg(cinema->name())
			               .arg(times.join(" "));
			ui_.webView->addPlace(cinema->ll(), html);
		}
	}
}
