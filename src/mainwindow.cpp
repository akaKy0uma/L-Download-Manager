#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "DownloadManager.h"
#include "NewTaskDialog.h"

#include <QDebug>
#include <QString>
#include <QUrl>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setupUi();
    setupConnection();
    m_defaultSavePath = QDir::currentPath() + "/download/";

    m_downloadManager = new DownloadManager;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUi()
{
    ui->setupUi(this);
    // 设置 QTableWidget 的列宽自动适应窗口大小
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    // 添加treeItem
    m_allTasksItem = new QTreeWidgetItem(ui->treeWidget);
    m_allTasksItem->setText(0, "All Tasks");

    m_downloadingTasksItem = new QTreeWidgetItem(ui->treeWidget);
    m_downloadingTasksItem->setText(0, "Downloading Tasks");

    m_completedTasksItem = new QTreeWidgetItem(ui->treeWidget);
    m_completedTasksItem->setText(0, "Completed Tasks");
}

void MainWindow::setupConnection()
{
    connect(ui->btn_newTask, &QPushButton::clicked, this, &MainWindow::onAddNewTask);
}

void MainWindow::onAddNewTask()
{
    NewTaskDialog dialog(this, m_defaultSavePath);
    if (dialog.exec() == QDialog::Accepted) {
        QString url = dialog.getDownloadUrl();
        QString savePath = dialog.getSavePath();

        // 将新任务添加到下载管理器
        m_downloadManager->addTask(QUrl(url), savePath);

        // 更新默认路径
        if(dialog.isDefaultPath())
            m_defaultSavePath = savePath;
    }
}
