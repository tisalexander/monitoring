#include "writer.h"

#include <QtCore/QDir>
#include <QtCore/QFile>

/*------- Writer ------------------------------------------------------------*/
Writer::Writer(QObject *parent)	:
	QObject(parent)
{
	m_pWorker = new WriterWorker();
	m_pWorker->moveToThread(&m_thread);

	connect(&m_thread, SIGNAL(finished()), m_pWorker, SLOT(deleteLater()));
	connect(m_pWorker, SIGNAL(finished()), this, SIGNAL(finished()));
	connect(this, SIGNAL(signalStart()), m_pWorker, SLOT(start()));

	m_thread.start();
}

Writer::~Writer()
{
	m_thread.quit();
	m_thread.wait();
}

void Writer::setPath(const QString &path)
{
	m_pWorker->m_path = path;
}

QString Writer::path() const
{
	return m_pWorker->m_path;
}

void Writer::setCount(int count)
{
	m_pWorker->m_count = count;
}

int Writer::count() const
{
	return m_pWorker->m_count;
}

void Writer::setPrefix(const QString &prefix)
{
	m_pWorker->m_prefix = prefix;
}

QString Writer::prefix() const
{
	return m_pWorker->m_prefix;
}

void Writer::setPostfix(const QString &postfix)
{
	m_pWorker->m_postfix = postfix;
}

QString Writer::postfix() const
{
	return m_pWorker->m_postfix;
}

int Writer::currentIndex() const
{
	return m_pWorker->m_currentIndex;
}

void Writer::start()
{
	emit signalStart();
}

void Writer::stop()
{
	m_pWorker->m_stop = true;
}

/*------- WriterWorker ------------------------------------------------------*/
WriterWorker::WriterWorker(QObject *parent) :
	QObject(parent)
{
	m_count = 100;
	m_currentIndex = 0;
	m_stop = false;
}

WriterWorker::~WriterWorker()
{
}

void WriterWorker::start()
{
	QString filepath;
	char array[8] = {1, 1, 1, 1, 1, 1, 1, 1};

	m_currentIndex = 0;
	m_stop = false;

	for (int i = 0; i < m_count; i++) {

		if (m_stop) {
			break;
		}

		filepath = m_path + QDir::separator() + m_prefix +
				   QString::number(i + 1) + m_postfix;

		QFile file(filepath);
		if (!file.open(QIODevice::WriteOnly)) {
			return;
		}

		file.write(array, 8);
		file.close();

		m_currentIndex = i;
	}

	emit finished();
}
