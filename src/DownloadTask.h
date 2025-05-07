#ifndef DOWNLOADTASK_H
#define DOWNLOADTASK_H

#include <QObject>
#include <QUrl>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>

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

signals:
    void progress(qint64 bytesReceived, qint64 bytesTotal);
    void finished();
    void error(const QString &errorString);

private slots:
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onReadyRead();
    void onFinished();
    void onError(QNetworkReply::NetworkError code);

private:
    QUrl m_url;
    QString m_savePath;
    QFile m_file;
    QNetworkAccessManager m_networkManager;
    QNetworkReply *m_reply;
    bool m_paused;
    int m_status;
};

#endif // DOWNLOADTASK_H
