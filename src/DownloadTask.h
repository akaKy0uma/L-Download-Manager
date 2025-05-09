#ifndef DOWNLOADTASK_H
#define DOWNLOADTASK_H

#include <QObject>
#include <QUrl>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMutex>
#include <QElapsedTimer>

enum TaskStatus {
    Stopped = 0,
    Pending,
    Downloading,
    Completed,
    Failed
};

class DownloadTaskWorker;
class DownloadTask : public QObject
{
    Q_OBJECT
public:
    explicit DownloadTask(const QUrl &url, const QString &saveDirPath, QObject *parent = nullptr);
    void start();
    void pause();
    void resume();
    void cancel();
    int getStatus();

    void startInThread();

    qint64 getFileSize();

signals:
    void progress(qint64 bytesReceived, qint64 bytesTotal);
    void finished();
    void error(const QString &errorString);

private slots:
    void onChunkDownloadProgress(int chunk, qint64 bytesReceived, qint64 bytesTotal);
    void onChunkReadyRead(int chunk, const QByteArray& data);
    void onChunkFinished(int chunk);
    void onChunkError(int chunk, int code);

private:
    QUrl m_url;
    QString m_savePath;
    QFile m_file;
    QNetworkAccessManager m_networkManager;
    bool m_paused;
    int m_status;

    int m_totalReceiedByte;
    int m_totalByte;
    QMutex* m_mutex;
    int m_numThreads;
    int m_chunkMask;

    QElapsedTimer* elapseTimer;
};

#endif // DOWNLOADTASK_H
