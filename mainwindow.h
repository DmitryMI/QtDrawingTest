#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "probabilityprovider.h"
#include "renderarea.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define MAIN_OK 0
#define MAIN_FORMAT_ERROR -1

class MainWindow : public QMainWindow, public ProbabilityProvider
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	bool GetCurrentProbability(double*) override;

private slots:
	void on_startButton_clicked();

	void on_experimentButton_clicked();

private:
	Ui::MainWindow *ui;

	void setupCustomUi();

	RenderArea *renderArea;
};
#endif // MAINWINDOW_H
