#include "DownloadManager.h"
#include "DownloadTask.h"

DownloadManager::DownloadManager(QObject *parent)
    : QObject{parent}
{

}

void DownloadManager::addTask(const QUrl &url, const QString &savePath)
{

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


