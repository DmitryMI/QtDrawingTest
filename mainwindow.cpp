#include <QPainter>
#include <QLayout>
#include "mainwindow.h"
#include "renderarea.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	setupCustomUi();
}

MainWindow::~MainWindow()
{
	delete renderArea;
	delete ui;
}


void MainWindow::on_startButton_clicked()
{
	renderArea->DrawGraphNode(100, 100);
	renderArea->DrawGraphNode(200, 200);
	renderArea->DrawGraphEdge(100, 100, 200, 200);

	renderArea->update();
}

void MainWindow::setupCustomUi()
{
	renderArea = new RenderArea();
	renderArea->setMinimumSize(width(), height() - 150);
	QLayout *windowLayout = layout();

	windowLayout->addWidget(renderArea);
}
