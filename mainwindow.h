#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

namespace Ui {
	class MainWindow;
}

class Monitoring;
class Writer;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void onFilesWriter();
	void onStartMonitoring();

private:
	Ui::MainWindow *ui;
	Monitoring *m_pMonitoring;
	Writer *m_pWriter;

	void connect();
};

#endif // MAINWINDOW_H
