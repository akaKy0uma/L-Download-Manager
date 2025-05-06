#include "DownloadTask.h"
#include "DownloadTaskWorker.h"

DownloadTask::DownloadTask(const QUrl &url, const QString &savePath, QObject *parent)
    : QObject{parent}
    , m_url(url)
{
    qDebug() << __FUNCTION__ << "New task added: URL =" << url << ", Save Path =" << savePath;

    m_status = Stopped;
}

void DownloadTask::start()
{
    qDebug() << __FUNCTION__;
    m_status = Downloading;

    QNetworkRequest request(m_url);
    request.setRawHeader("Range", "bytes=0-"); // 示例：请求整个文件
    m_reply = m_networkManager.get(request);

    connect(m_reply, &QNetworkReply::downloadProgress, this, &DownloadTask::onDownloadProgress);
    connect(m_reply, &QNetworkReply::finished, this, &DownloadTask::onFinished);
    connect(m_reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &DownloadTask::onError);
}

void DownloadTask::pause()
{
    qDebug() << __FUNCTION__;
    m_status = Stopped;
}

void DownloadTask::resume()
{
    qDebug() << __FUNCTION__;
}

void DownloadTask::cancel()
{
    qDebug() << __FUNCTION__;
}

int DownloadTask::getStatus()
{
    return m_status;
}

void DownloadTask::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug() << __FUNCTION__;
}

void DownloadTask::onFinished()
{
    qDebug() << __FUNCTION__;
}

void DownloadTask::onError(QNetworkReply::NetworkError code)
{
    qDebug() << __FUNCTION__ << "ErrorCode" << code;
}


