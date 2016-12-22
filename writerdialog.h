#ifndef WRITERDIALOG_H
#define WRITERDIALOG_H

#include <QtCore/QTimer>
#include <QtWidgets/QDialog>

namespace Ui {
	class WriterDialog;
}

class Writer;

class WriterDialog : public QDialog
{
	Q_OBJECT

public:
	explicit WriterDialog(Writer *writer, QWidget *parent = 0);
	~WriterDialog();

private slots:
	void onBrowse();
	void onCount();
	void onPrefix();
	void onPostfix();
	void onStart();
	void onTimer();
	void onFinished();

private:
	Ui::WriterDialog *ui;
	Writer *m_pWriter;
	QTimer m_timer;
};

#endif // WRITERDIALOG_H
