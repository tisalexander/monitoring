#include "monitoring.h"

#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>

/*------- Monitoring --------------------------------------------------------*/
Monitoring::Monitoring(QObject *parent) :
	QObject(parent)
{
	m_pWatcher = new Watcher();
	m_pWatcher->moveToThread(&m_threadWatcher);

	m_pFileInspector = new FileInspector();
	m_pFileInspector->moveToThread(&m_threadFileInspector);

	connect(&m_threadWatcher, SIGNAL(finished()),
			m_pWatcher, SLOT(deleteLater()));

	connect(&m_threadFileInspector, SIGNAL(finished()),
			m_pFileInspector, SLOT(deleteLater()));

	connect(this, SIGNAL(signalStart()), m_pWatcher, SLOT(start()));

	connect(m_pWatcher, SIGNAL(newFileDetected(const QString &)),
			m_pFileInspector, SLOT(appendFile(const QString &)));

	m_threadWatcher.start();
	m_threadFileInspector.start();
}

Monitoring::~Monitoring()
{
	m_threadWatcher.quit();
	m_threadFileInspector.quit();

	m_threadWatcher.wait();
	m_threadFileInspector.wait();
}

void Monitoring::start()
{
	emit signalStart();
}

/*------- Watcher -----------------------------------------------------------*/
Watcher::Watcher(QObject *parent) :
	QObject(parent)
{
	m_path = "C:/ftp/monitoring";
	m_timerId = -1;
}

Watcher::~Watcher()
{
}

void Watcher::start()
{
	const qint64 time1 = QDateTime::currentMSecsSinceEpoch();

	m_filenames.clear();

	foreach (const QFileInfo &info,
			 QDir(m_path).entryInfoList(QDir::Files)) {
		m_filenames.insert(info.absoluteFilePath(), 0);
	}

	const qint64 time2 = QDateTime::currentMSecsSinceEpoch();

	qDebug() << "Copying filenames: " << time2 - time1 << " msec";

	m_timerId = startTimer(3000);
}

void Watcher::timerEvent(QTimerEvent *event)
{
	Q_UNUSED(event)

	const qint64 time1 = QDateTime::currentMSecsSinceEpoch();

	foreach (const QFileInfo &fileInfo,
			 QDir(m_path).entryInfoList(QDir::Files))
	{
		if (!m_filenames.contains(fileInfo.absoluteFilePath())) {
			//	monitorFile(fileInfo.absoluteFilePath());
			emit newFileDetected(fileInfo.absoluteFilePath());
			m_filenames.insert(fileInfo.absoluteFilePath(), 0);
			qDebug() << "New file found: " << fileInfo.absoluteFilePath();
		}
	}

	const qint64 time2 = QDateTime::currentMSecsSinceEpoch();

	qDebug() << "Searching time: " << time2 - time1 << " msec";
}

/*------- FileInspector -----------------------------------------------------*/
FileInspector::FileInspector(QObject *parent) :
	QObject(parent)
{
	m_timerId = -1;
}

FileInspector::~FileInspector()
{
}

void FileInspector::appendFile(const QString &filename)
{
	m_filenames.insert(filename, -1);

	if (m_timerId == -1) {
		m_timerId = startTimer(500);
	}

	qDebug() << "File Inspector";
	qDebug() << filename << " is appended" << endl;
}

void FileInspector::timerEvent(QTimerEvent *event)
{
	Q_UNUSED(event)

	if (m_filenames.isEmpty() && m_timerId != -1) {
		killTimer(m_timerId);
		m_timerId = -1;
		return;
	}

	QMapIterator<QString, qint64> i(m_filenames);
	while (i.hasNext()) {
		i.next();

		const qint64 filesize = QFileInfo(i.key()).size();

		if (filesize == i.value()) {
			emit ready(i.key());

			qDebug() << i.key() << " Filesize: " << i.value() << "bytes";

			m_filenames.remove(i.key());
		} else {
			m_filenames.insert(i.key(), filesize);
		}
	}
}
