#include "DownloadTask.h"
#include "DownloadTaskWorker.h"

#include <QThread>
#include <QFileInfo>
#include <QDir>
#include <QtConcurrent/QtConcurrent>
#include <QRunnable>

// http://www.w3school.com.cn/i/movie.mp4

DownloadTask::DownloadTask(const QUrl &url, const QString &saveDirPath, QObject *parent)
    : QObject{parent}
    , m_url(url)
{
    m_numThreads = 8; // 分块数量
    m_status = Stopped;    

    m_mutex = new QMutex;
    elapseTimer = new QElapsedTimer;

    qDebug() << "New task added: URL =" << url << ", Save Dir Path =" << saveDirPath;
    // 检查并创建目录
    QFileInfo fileInfo(saveDirPath);
    QDir dir = fileInfo.isDir() ? QDir(saveDirPath) : fileInfo.absoluteDir();
    if (!dir.exists() && !dir.mkpath(".")) {
        qDebug() << "Failed to create directory:" << dir.absolutePath();
    }

    // 如果 saveDirPath 是目录，则从 URL 提取文件名
    QString filePath;
    if (fileInfo.isDir()) {
        QString fileName = m_url.fileName();
        if (fileName.isEmpty()) {
            fileName = "downloaded_file"; // 默认文件名
        }
        filePath = dir.filePath(fileName + ".tmp"); // 添加 .tmp 后缀
    } else {
        filePath = saveDirPath + ".tmp"; // 如果是完整路径，直接添加 .tmp 后缀
    }

    // 打开文件
    m_file.setFileName(filePath);
    if (!m_file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Failed to open file for writing:" << filePath;
    }
    else
    {
        qDebug() << "File opened successfully for writing:" << filePath;
    }

    qDebug() << "SSL support:" << QSslSocket::supportsSsl();
    qDebug() << "SSL library version:" << QSslSocket::sslLibraryVersionString();
}

void DownloadTask::startInThread()
{
    qDebug() << __FUNCTION__ << QThread::currentThread();

    QThread *thread = new QThread;
    this->moveToThread(thread);

    // 当线程启动时，调用 DownloadTask 的 start 方法
    connect(thread, &QThread::started, this, &DownloadTask::start);

    // 当任务完成时，退出线程并释放资源
    connect(this, &DownloadTask::finished, thread, &QThread::quit);
    connect(this, &DownloadTask::finished, this, &DownloadTask::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    thread->start();
}

void DownloadTask::start()
{
    qDebug() << __FUNCTION__ << QThread::currentThread();

    m_totalReceiedByte = 0;
    m_totalByte = getFileSize(); // 获取文件总大小
    qint64 chunkSize = m_totalByte / m_numThreads;

    qDebug() << "total size" << m_totalByte;
    elapseTimer->restart();

    m_chunkMask = (1 << m_numThreads) - 1;

    for(int i = 0; i < m_numThreads; ++i)
    {
        qint64 start = i * chunkSize;
        qint64 end = (i == m_numThreads - 1) ? m_totalByte - 1 : (start + chunkSize - 1);

        DownloadTaskWorker* worker = new DownloadTaskWorker(m_url, start, end, i);
        QThread* thread = new QThread;

        worker->moveToThread(thread);

        // 当线程启动时，调用 worker 的 run 方法
        connect(thread, &QThread::started, worker, &DownloadTaskWorker::run);

        // 当 worker 完成时，退出线程并释放资源
        connect(worker, &DownloadTaskWorker::sigChunkFinished, thread, &QThread::quit);
        connect(worker, &DownloadTaskWorker::sigChunkFinished, worker, &DownloadTaskWorker::deleteLater);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);

        // 连接 worker 的信号到 DownloadTask 的槽
        connect(worker, &DownloadTaskWorker::sigChunkDownloadProgress, this, &DownloadTask::onChunkDownloadProgress);
        connect(worker, &DownloadTaskWorker::sigChunkReadReady, this, &DownloadTask::onChunkReadyRead);
        connect(worker, &DownloadTaskWorker::sigChunkError, this, &DownloadTask::onChunkError);
        connect(worker, &DownloadTaskWorker::sigChunkFinished, this, &DownloadTask::onChunkFinished);

        thread->start();
    }

    m_status = Downloading;

    return;
}

void DownloadTask::pause()
{
    qDebug() << __FUNCTION__ << QThread::currentThread();
    m_status = Stopped;
}

void DownloadTask::resume()
{
    qDebug() << __FUNCTION__ << QThread::currentThread();
}

void DownloadTask::cancel()
{
    qDebug() << __FUNCTION__ << QThread::currentThread();
}

int DownloadTask::getStatus()
{
    return m_status;
}

// 下载进度更新
void DownloadTask::onChunkDownloadProgress(int chunk, qint64 bytesReceived, qint64 bytesTotal)
{
    m_mutex->lock();
    m_totalReceiedByte += bytesReceived;
    double totalReceiedByte = m_totalReceiedByte * 100.0;
    m_mutex->unlock();

    qDebug() << "[" << QThread::currentThread() << "]"
             << QString("%1%").arg(QString::number(totalReceiedByte / m_totalByte, 'f', 2))
             << QString("rec:%1 chunkTotal:%2").arg(bytesReceived).arg(bytesTotal)
             << QString("totalReceive:%1 total:%2").arg(m_totalReceiedByte).arg(m_totalByte);
}

// 每次接收到新的数据块时触发 实时处理接收到的数据
void DownloadTask::onChunkReadyRead(int chunk, const QByteArray& data)
{
    if (m_file.isOpen())
    {
        m_file.write(data); // 写入文件
    }
}

void DownloadTask::onChunkFinished(int chunk)
{
    qDebug()<< QString::number(elapseTimer->elapsed() / 1000.0, 'f', 3);
    m_chunkMask ^= (1 << chunk);
    if(m_chunkMask != 0)
        return;

    if (m_file.isOpen()) {
        m_file.close(); // 关闭文件
    }

    // if (m_reply->error() == QNetworkReply::NoError)
    // {
    //     QString contentDisposition = m_reply->rawHeader("Content-Disposition");
    //     QString fileName;

    //     // 从 Content-Disposition 提取文件名
    //     if (contentDisposition.contains("filename=")) {
    //         fileName = contentDisposition.split("filename=").last().replace("\"", "").trimmed();
    //     } else {
    //         // 如果没有 Content-Disposition，则从 URL 提取文件名
    //         fileName = m_url.fileName();
    //         if (fileName.isEmpty()) {
    //             fileName = "downloaded_file"; // 默认文件名
    //         }
    //     }

    //     // 获取保存路径的目录
    //     QFileInfo fileInfo(m_file.fileName());
    //     QString newFilePath = fileInfo.absoluteDir().filePath(fileName);

    //     // 存在相同名称文件
    //     if(QFile::exists(newFilePath))
    //     {
    //         int cnt = 0;
    //         QString left = newFilePath.left(newFilePath.lastIndexOf('.'));
    //         QString right = newFilePath.right(newFilePath.size() - newFilePath.lastIndexOf('.'));
    //         while(QFile::exists(newFilePath))
    //         {
    //             newFilePath = QString("%1(%2)%3").arg(left).arg(++cnt).arg(right);
    //         }
    //     }

    //     if (QFile::rename(m_file.fileName(), newFilePath)) {
    //         qDebug() << "File renamed to:" << newFilePath;
    //     } else {
    //         qDebug() << "Failed to rename file to:" << newFilePath;
    //     }

    //     qDebug() << "Done!";
    // }
    // else
    // {
    //     qDebug() << "Download failed with error:" << m_reply->errorString();
    // }
}

void DownloadTask::onChunkError(int chunk, int code)
{
    qDebug() << __FUNCTION__ << "ErrorCode" << code << QThread::currentThread();
}

qint64 DownloadTask::getFileSize()
{
    QNetworkAccessManager manager;
    QNetworkRequest request(m_url);
    QNetworkReply *reply = manager.head(request);

    // 等待完成（同步方式，不推荐用于 GUI 线程）
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    qint64 size = reply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
    reply->deleteLater();

    return size;
}

