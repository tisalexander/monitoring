#ifndef MONITORING_H
#define MONITORING_H

#include <QtCore/QHash>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QThread>

class Watcher;
class FileInspector;

/*------- Monitoring --------------------------------------------------------*/
class Monitoring : public QObject
{
	Q_OBJECT

public:
	Monitoring(QObject *parent = 0);
	~Monitoring();

	void start();

signals:
	void signalStart();

private:
	QThread m_threadWatcher;
	QThread m_threadFileInspector;
	Watcher *m_pWatcher;
	FileInspector *m_pFileInspector;
};

/*------- Watcher -----------------------------------------------------------*/
class Watcher : public QObject
{
	Q_OBJECT

	friend class Monitoring;

public:
	Watcher(QObject *parent = 0);
	~Watcher();

public slots:
	void start();

signals:
	void newFileDetected(const QString &filename);

protected:
	void timerEvent(QTimerEvent *event);

private:
	QString m_path;
	int m_timerId;
	QHash<QString, int> m_filenames;
};

/*------- FileInspector -----------------------------------------------------*/
class FileInspector : public QObject
{
	Q_OBJECT

public:
	FileInspector(QObject *parent = 0);
	~FileInspector();

signals:
	void ready(const QString &filename);

public slots:
	void appendFile(const QString &filename);

protected:
	void timerEvent(QTimerEvent *event);

private:
	QMap<QString, qint64> m_filenames;
	int m_timerId;
};

#endif // MONITORING_H
