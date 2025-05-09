#include "DownloadTaskWorker.h"

#include <QDebug>
#include <QString>

DownloadTaskWorker::DownloadTaskWorker(QUrl url, qint64 start, qint64 end, int chunkNo)
    : m_url(url)
    , m_start(start)
    , m_end(end)
    , m_chunkNo(chunkNo)
{
    qDebug() << __FUNCTION__ << QThread::currentThread()
             << QString("No:%1 %2[%3, %4]").arg(chunkNo).arg(end - start).arg(start).arg(end);
    m_preByteCnt = 0;
}

void DownloadTaskWorker::run()
{
    qDebug() << __FUNCTION__ << QThread::currentThread();

    // 在目标线程中创建 QNetworkAccessManager
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(m_url);
    request.setRawHeader("Range", QString("bytes=%1-%2").arg(m_start).arg(m_end)
                                      .toStdString().c_str());
    m_reply = manager->get(request);

    connect(m_reply, &QNetworkReply::downloadProgress, this, &DownloadTaskWorker::onDownloadProgress);
    connect(m_reply, &QNetworkReply::readyRead, this, &DownloadTaskWorker::onReadyRead);
    connect(m_reply, &QNetworkReply::finished, this, &DownloadTaskWorker::onFinished);
    connect(m_reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &DownloadTaskWorker::onError);
}

// 下载进度更新
void DownloadTaskWorker::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug() << "[" << QThread::currentThread() << "]"
             << QString("%1%").arg(QString::number(bytesReceived * 100.0 / bytesTotal, 'f', 2))
             << QString("chunkTotalrec:%1 chunkTotal:%2").arg(bytesReceived).arg(bytesTotal);

    emit sigChunkDownloadProgress(m_chunkNo, bytesReceived - m_preByteCnt, bytesTotal);
    m_preByteCnt = bytesReceived;
}

// 每次接收到新的数据块时触发 实时处理接收到的数据
void DownloadTaskWorker::onReadyRead()
{
    QByteArray data = m_reply->readAll(); // 读取当前接收到的数据
    emit sigChunkReadReady(m_chunkNo, data);
}

void DownloadTaskWorker::onFinished()
{
    qDebug() << QThread::currentThread() << "Done!";
    emit sigChunkFinished(m_chunkNo);
}

void DownloadTaskWorker::onError(QNetworkReply::NetworkError code)
{
    qDebug() << QThread::currentThread() << "error! code:" << code;
    emit sigChunkError(m_chunkNo, code);
}
