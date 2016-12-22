#ifndef WRITER_H
#define WRITER_H

#include <QtCore/QObject>
#include <QtCore/QThread>

class WriterWorker;

/*------- Writer ------------------------------------------------------------*/
class Writer : public QObject
{
	Q_OBJECT

public:
	Writer(QObject *parent = 0);
	~Writer();

	void setPath(const QString &path);
	QString path() const;

	void setCount(int count);
	int count() const;

	void setPrefix(const QString &prefix);
	QString prefix() const;

	void setPostfix(const QString &postfix);
	QString postfix() const;

	int currentIndex() const;

public slots:
	void start();
	void stop();

signals:
	void finished();
	void signalStart();

private:
	QThread m_thread;
	WriterWorker *m_pWorker;
};

/*------- WriterWorker ------------------------------------------------------*/
class WriterWorker : public QObject
{
	Q_OBJECT

	friend class Writer;

public:
	WriterWorker(QObject *parent = 0);
	~WriterWorker();

private slots:
	void start();

signals:
	void finished();

private:
	QString m_path;
	int m_count;
	QString m_prefix;
	QString m_postfix;

	int m_currentIndex;
	bool m_stop;
};

#endif // WRITER_H
