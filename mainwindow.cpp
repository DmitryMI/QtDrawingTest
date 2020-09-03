#include <QPainter>
#include <QLayout>
#include <QMessageBox>
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
	RecursiveSearcher *searcher = new RecursiveSearcher();
	Calculator *calculator = new Calculator(searcher);
	Graph *graph = renderArea->GetGraph();

	double mathProbability = calculator->GetMathematicalProbability(graph);

	QString probString = QString("%1\n").arg(mathProbability);

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
