#include "writerdialog.h"
#include "ui_writerdialog.h"

#include "writer.h"
#include <QtWidgets/QFileDialog>

WriterDialog::WriterDialog(Writer *writer, QWidget *parent) :
	QDialog(parent),
	m_pWriter(writer), ui(new Ui::WriterDialog)
{
	ui->setupUi(this);

	connect(&m_timer, SIGNAL(timeout()), SLOT(onTimer()));

	connect(ui->pushButtonBrowse, SIGNAL(clicked()), SLOT(onBrowse()));
	connect(ui->spinBoxCount, SIGNAL(valueChanged(int)), SLOT(onCount()));
	connect(ui->lineEditPrefix, SIGNAL(textChanged(QString)), SLOT(onPrefix()));
	connect(ui->lineEditPostfix, SIGNAL(textChanged(QString)), SLOT(onPostfix()));

	connect(ui->pushButtonStart, SIGNAL(clicked()), SLOT(onStart()));
	connect(m_pWriter, SIGNAL(finished()), SLOT(onFinished()));

	ui->progressBar->setEnabled(false);
	ui->lineEditPath->setText(m_pWriter->path());
	ui->spinBoxCount->setValue(m_pWriter->count());
	ui->lineEditPrefix->setText(m_pWriter->prefix());
	ui->lineEditPostfix->setText(m_pWriter->postfix());
}

WriterDialog::~WriterDialog()
{
	m_pWriter->stop();

	delete ui;
}

void WriterDialog::onBrowse()
{
	QString path = QFileDialog::getExistingDirectory(this);
	m_pWriter->setPath(path);
	ui->lineEditPath->setText(m_pWriter->path());
}

void WriterDialog::onCount()
{
	m_pWriter->setCount(ui->spinBoxCount->value());
}

void WriterDialog::onPrefix()
{
	m_pWriter->setPrefix(ui->lineEditPrefix->text());
}

void WriterDialog::onPostfix()
{
	m_pWriter->setPostfix(ui->lineEditPostfix->text());
}

void WriterDialog::onStart()
{
	if (ui->pushButtonStart->text() == "Start") {
		ui->pushButtonStart->setText(tr("Stop"));
		ui->progressBar->setEnabled(true);

		ui->lineEditPath->setEnabled(false);
		ui->pushButtonBrowse->setEnabled(false);
		ui->spinBoxCount->setEnabled(false);
		ui->lineEditPrefix->setEnabled(false);
		ui->lineEditPostfix->setEnabled(false);

		m_timer.start(500);
		m_pWriter->start();
	} else {
		ui->pushButtonStart->setText(tr("Start"));
		m_pWriter->stop();
	}
}

void WriterDialog::onTimer()
{
	ui->progressBar->setValue((m_pWriter->currentIndex() + 1) * 100 /
							  m_pWriter->count());
}

void WriterDialog::onFinished()
{
	m_timer.stop();

	ui->pushButtonStart->setText(tr("Start"));

	ui->lineEditPath->setEnabled(true);
	ui->pushButtonBrowse->setEnabled(true);
	ui->spinBoxCount->setEnabled(true);
	ui->lineEditPrefix->setEnabled(true);
	ui->lineEditPostfix->setEnabled(true);

	ui->progressBar->setValue(0);
	ui->progressBar->setEnabled(false);
}
