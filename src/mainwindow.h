#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QTreeWidgetItem;
class DownloadManager;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupUi();
    void setupConnection();

private slots:
    void onAddNewTask();

private:
    Ui::MainWindow *ui;

    QTreeWidgetItem *m_allTasksItem;
    QTreeWidgetItem *m_completedTasksItem;
    QTreeWidgetItem *m_downloadingTasksItem;

    DownloadManager *m_downloadManager;
    QString m_defaultSavePath;
};
#endif // MAINWINDOW_H
