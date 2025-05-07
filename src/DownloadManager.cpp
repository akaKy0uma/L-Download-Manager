#include "DownloadManager.h"
#include "DownloadTask.h"

#include <QMessageBox>
#include <QThread>

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

    qDebug() << __FUNCTION__ << QThread::currentThread();

    DownloadTask *task = new DownloadTask(url, savePath, nullptr);
    // connect(task, &DownloadTask::progress, this, &DownloadManager::onTaskProgress);
    // connect(task, &DownloadTask::finished, this, &DownloadManager::onTaskFinished);
    // connect(task, &DownloadTask::error, this, &DownloadManager::onTaskError);

    m_taskMap.insert(url.toDisplayString(), task);
    task->startInThread();
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


