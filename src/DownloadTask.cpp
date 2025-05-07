#include "DownloadTask.h"
#include "DownloadTaskWorker.h"

#include <QThread>
#include <QFileInfo>
#include <QDir>

DownloadTask::DownloadTask(const QUrl &url, const QString &saveDirPath, QObject *parent)
    : QObject{parent}
    , m_url(url)
{
    qDebug() << __FUNCTION__ << "New task added: URL =" << url << ", Save Dir Path =" << saveDirPath;
    m_reply = nullptr;
    m_status = Stopped;

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
    if (m_reply) {
        m_reply->abort(); // 取消之前的请求
        m_reply->deleteLater();
        m_reply = nullptr;
    }

    qDebug() << __FUNCTION__ << QThread::currentThread();
    m_status = Downloading;

    // 在目标线程中创建 QNetworkAccessManager
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(m_url);
    request.setRawHeader("Range", "bytes=0-"); // 示例：请求整个文件
    m_reply = manager->get(request);

    connect(m_reply, &QNetworkReply::downloadProgress, this, &DownloadTask::onDownloadProgress);
    connect(m_reply, &QNetworkReply::readyRead, this, &DownloadTask::onReadyRead);
    connect(m_reply, &QNetworkReply::finished, this, &DownloadTask::onFinished);
    connect(m_reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &DownloadTask::onError);
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
void DownloadTask::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug() << "[" << QThread::currentThread() << "]"
             << QString("%1%").arg(QString::number(bytesReceived * 100.0 / bytesTotal, 'f', 2));
}

// 每次接收到新的数据块时触发 实时处理接收到的数据
void DownloadTask::onReadyRead()
{
    if (m_reply && m_file.isOpen()) {
        QByteArray data = m_reply->readAll(); // 读取当前接收到的数据
        m_file.write(data);                  // 写入文件
    }
}

void DownloadTask::onFinished()
{
    if (m_file.isOpen()) {
        m_file.close(); // 关闭文件
    }

    if (m_reply->error() == QNetworkReply::NoError)
    {
        QString contentDisposition = m_reply->rawHeader("Content-Disposition");
        QString fileName;

        // 从 Content-Disposition 提取文件名
        if (contentDisposition.contains("filename=")) {
            fileName = contentDisposition.split("filename=").last().replace("\"", "").trimmed();
        } else {
            // 如果没有 Content-Disposition，则从 URL 提取文件名
            fileName = m_url.fileName();
            if (fileName.isEmpty()) {
                fileName = "downloaded_file"; // 默认文件名
            }
        }

        // 获取保存路径的目录
        QFileInfo fileInfo(m_file.fileName());
        QString newFilePath = fileInfo.absoluteDir().filePath(fileName);

        // 存在相同名称文件
        if(QFile::exists(newFilePath))
        {
            int cnt = 0;
            QString left = newFilePath.left(newFilePath.lastIndexOf('.'));
            QString right = newFilePath.right(newFilePath.size() - newFilePath.lastIndexOf('.'));
            while(QFile::exists(newFilePath))
            {
                newFilePath = QString("%1(%2)%3").arg(left).arg(++cnt).arg(right);
            }
        }

        if (QFile::rename(m_file.fileName(), newFilePath)) {
            qDebug() << "File renamed to:" << newFilePath;
        } else {
            qDebug() << "Failed to rename file to:" << newFilePath;
        }

        qDebug() << "Done!";
    }
    else
    {
        qDebug() << "Download failed with error:" << m_reply->errorString();
    }

    emit finished();
}

void DownloadTask::onError(QNetworkReply::NetworkError code)
{
    qDebug() << __FUNCTION__ << "ErrorCode" << code << QThread::currentThread();
}


