#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "monitoring.h"
#include "writer.h"
#include "writerdialog.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_pMonitoring = new Monitoring(this);
	m_pWriter = new Writer(this);

	connect();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::onFilesWriter()
{
	WriterDialog dlg(m_pWriter);
	dlg.exec();
}

void MainWindow::onStartMonitoring()
{
	m_pMonitoring->start();
}

void MainWindow::connect()
{
	QObject::connect(ui->actionFiles_Writer, SIGNAL(triggered()),
					 SLOT(onFilesWriter()));

	QObject::connect(ui->actionStart, SIGNAL(triggered()),
					 SLOT(onStartMonitoring()));

	QObject::connect(ui->actionExit, SIGNAL(triggered()),
					 qApp, SLOT(quit()));
}
