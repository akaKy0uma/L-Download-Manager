#include "DownloadManager.h"
#include "DownloadTask.h"

#include <QMessageBox>

DownloadManager::DownloadManager(QObject *parent)
    : QObject{parent}
{

}

void DownloadManager::addTask(const QUrl &url, const QString &savePath)
{
    if(m_taskMap.count(url.toDisplayString()))
    {
        QMessageBox::warning(NULL, tr("Duplicate Task"), tr("This task already exists."));
        return;
    }

    DownloadTask* task = new DownloadTask(url, savePath);
    m_taskMap[url.toDisplayString()] = task;

    task->start();
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


