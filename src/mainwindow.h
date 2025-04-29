#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QTreeWidgetItem;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupUi();

private:
    Ui::MainWindow *ui;

    QTreeWidgetItem *m_allTasksItem;
    QTreeWidgetItem *m_completedTasksItem;
    QTreeWidgetItem *m_downloadingTasksItem;
};
#endif // MAINWINDOW_H
