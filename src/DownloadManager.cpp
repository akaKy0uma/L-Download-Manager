#include "DownloadManager.h"
#include "DownloadTask.h"

DownloadManager::DownloadManager(QObject *parent)
    : QObject{parent}
{

}

void DownloadManager::addTask(const QUrl &url, const QString &savePath)
{
    qDebug() << "New task added: URL =" << url << ", Save Path =" << savePath;
}

void DownloadManager::pauseTask(int index)
{

}

void DownloadManager::resumeTask(int index)
{

}

void DownloadManager::cancelTask(int index)
{

}


