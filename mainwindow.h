#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPointer>

#include "ui_mainwindow.h"

class CinemaList;
class Movies;
class Movie;

class MainWindow : public QWidget
{
	Q_OBJECT
public:
	MainWindow();
	~MainWindow();

private slots:
	void moviesChanged();
	void cinemasChanged();
	void movieChanged();

	void moviesSelectionChanged();
	void timesSelectionChanged();
	void cinemasSelectionChanged();

private:
	Ui::MainWindow ui_;
	QPointer<CinemaList> cinemas_;
	QPointer<Movies> movies_;
	QPointer<Movie> movie_;
};

#endif
