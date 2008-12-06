/*
 * cinemaprogress.cpp
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

#include "cinemaprogress.h"

#include <QProgressDialog>
#include <QTimer>
#include <QCoreApplication>

#include "cinemalist.h"
#include "mainwindow.h"

CinemaProgress::CinemaProgress()
	: QObject()
	, startAppTimer_(0)
{
	startAppTimer_ = new QTimer(this);
	connect(startAppTimer_, SIGNAL(timeout()), SLOT(startApp()));
	startAppTimer_->setInterval(500);
	startAppTimer_->setSingleShot(true);

	cinemas_ = new CinemaList();
	cinemas_->setCanInitFromWeb(true);
	connect(cinemas_, SIGNAL(progressChanged()), SLOT(progressChanged()));
	QTimer::singleShot(100, cinemas_, SLOT(init()));

	progress_ = new QProgressDialog(trUtf8("Загружаю кинотеатры..."), trUtf8("Отмена"), 0, 0);
	connect(progress_, SIGNAL(canceled()), SLOT(cancel()));
	// progress_->setParent(this);
	progress_->setWindowModality(Qt::WindowModal);
	progress_->show();
}

CinemaProgress::~CinemaProgress()
{
	delete progress_;
}

void CinemaProgress::progressChanged()
{
	progress_->setValue(cinemas_->finishedProgress());
	progress_->setMaximum(cinemas_->fullProgress());
	// qWarning("progressChanged: %d / %d", cinemas_->finishedProgress(), cinemas_->fullProgress());

	if (cinemas_->fullProgress() > 0 &&
	    cinemas_->finishedProgress() >= cinemas_->fullProgress())
	{
		startAppTimer_->start();
	}
	else {
		startAppTimer_->stop();
	}
}

void CinemaProgress::cancel()
{
	QCoreApplication::instance()->quit();
}

void CinemaProgress::startApp()
{
	qWarning("startApp");
	cinemas_->saveCache();

	MainWindow* mw = new MainWindow();
	mw->show();
	mw->raise();
	mw->activateWindow();

	deleteLater();
}
