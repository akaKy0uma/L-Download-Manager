#include "DownloadTask.h"

DownloadTask::DownloadTask(const QUrl &url, const QString &savePath, QObject *parent)
    : QObject{parent}
    , m_url(url)
{

}

void DownloadTask::start()
{

}

void DownloadTask::pause()
{

}

void DownloadTask::resume()
{

}

void DownloadTask::cancel()
{

}

void DownloadTask::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{

}

void DownloadTask::onFinished()
{

}

void DownloadTask::onError(QNetworkReply::NetworkError code)
{

}


