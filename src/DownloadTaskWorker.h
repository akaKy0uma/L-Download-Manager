#ifndef DOWNLOADTASKWORKER_H
#define DOWNLOADTASKWORKER_H

#include <QObject>
#include <QThread>
#include <QUrl>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class DownloadTaskWorker : public QThread
{
    Q_OBJECT
public:
    explicit DownloadTaskWorker(QUrl url, qint64 start, qint64 end, int chunkNo);

    void run() override;

signals:
    void sigChunkDownloadProgress(int chunkNo, qint64 bytesReceived, qint64 bytesTotal);
    void sigChunkReadReady(int chunkNo, const QByteArray& data);
    void sigChunkFinished(int chunkNo);
    void sigChunkError(int chunkNo, int code);

private slots:
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onReadyRead();
    void onFinished();
    void onError(QNetworkReply::NetworkError code);

private:
    QUrl m_url;
    qint64 m_start;
    qint64 m_end;
    int m_chunkNo;
    qint64 m_preByteCnt;

    QNetworkAccessManager m_networkManager;
    QNetworkReply *m_reply;
};

#endif // DOWNLOADTASKWORKER_H
