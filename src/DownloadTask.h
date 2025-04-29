#ifndef DOWNLOADTASK_H
#define DOWNLOADTASK_H

#include <QObject>
#include <QUrl>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class DownloadTask : public QObject
{
    Q_OBJECT
public:
    explicit DownloadTask(const QUrl &url, const QString &savePath, QObject *parent = nullptr);
    void start();
    void pause();
    void resume();
    void cancel();

signals:
    void progress(qint64 bytesReceived, qint64 bytesTotal);
    void finished();
    void error(const QString &errorString);

private slots:
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onFinished();
    void onError(QNetworkReply::NetworkError code);

private:
    QUrl m_url;
    QString m_savePath;
    QFile m_file;
    QNetworkAccessManager m_networkManager;
    QNetworkReply *m_reply;
    bool m_paused;
};

#endif // DOWNLOADTASK_H
