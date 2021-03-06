#include <QPainter>
#include <QLayout>
#include <QMessageBox>
#include <QTime>
#include <QElapsedTimer>
#include "calculator.h"
#include "mainwindow.h"
#include "recursivesearcher.h"
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

bool MainWindow::GetCurrentProbability(double* probability)
{
	QString textVal = ui->probabilityEditBox->text();
	bool ok;
	*probability = textVal.toDouble(&ok);

	if(ok)
	{
		ui->probabilityEditBox->setStyleSheet("QLineEdit { background: green }");
	}
	else
	{
		ui->probabilityEditBox->setStyleSheet("QLineEdit { background: red }");
	}

	return ok;
}


void MainWindow::on_startButton_clicked()
{
    QElapsedTimer timer;
    timer.start();

	RecursiveSearcher *searcher = new RecursiveSearcher();
	Calculator *calculator = new Calculator(searcher);
	Graph<NetParams> *graph = renderArea->GetGraph();

	double mathProbability = calculator->GetMathematicalProbability(graph);
    int milliseconds = timer.elapsed();
    QTime timespan = QTime::fromMSecsSinceStartOfDay(milliseconds);

    QString probString = QString("Time: %0\nMathematical probability: %1\n").arg(timespan.toString()).arg(mathProbability);

	QMessageBox msgBox;
	msgBox.setText(probString);
	msgBox.exec();

	delete calculator;
	delete searcher;
	delete graph;


}

void MainWindow::setupCustomUi()
{
	renderArea = new RenderArea(this);
	renderArea->setMinimumSize(width(), height() - 150);
	QLayout *windowLayout = layout();

	windowLayout->addWidget(renderArea);
}

void MainWindow::on_experimentButton_clicked()
{
	RecursiveSearcher *searcher = new RecursiveSearcher();
	Calculator *calculator = new Calculator(searcher);
	Graph<NetParams> *graph = renderArea->GetGraph();

    double experimentProbability = calculator->GetExperimentalProbability(graph, 100000);

	QString probString = QString("Experimental probability: %1\n").arg(experimentProbability);

	QMessageBox msgBox;
	msgBox.setText(probString);
	msgBox.exec();

	delete calculator;
	delete searcher;
	delete graph;
}

void MainWindow::on_buildSquareGraph_clicked()
{
    QString sizeText = ui->squareGraphSizeBox->text();
    int size = sizeText.toInt();

    renderArea->BuildSquareGraph(size);
}
