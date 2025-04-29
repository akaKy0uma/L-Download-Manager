#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setupUi();
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
