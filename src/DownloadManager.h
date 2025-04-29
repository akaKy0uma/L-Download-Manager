#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>

class DownloadTask;
class DownloadManager : public QObject
{
    Q_OBJECT
public:
    explicit DownloadManager(QObject *parent = nullptr);
    void addTask(const QUrl &url, const QString &savePath);
    void pauseTask(int index);
    void resumeTask(int index);
    void cancelTask(int index);

signals:
    void taskAdded(DownloadTask *task);
    void taskRemoved(int index);

private:
    QList<DownloadTask *> m_tasks;
};

#endif // DOWNLOADMANAGER_H
