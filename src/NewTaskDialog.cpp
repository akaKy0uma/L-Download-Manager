#include "NewTaskDialog.h"
#include "ui_NewTaskDialog.h"

#include <QDebug>
#include <QFileDialog>

NewTaskDialog::NewTaskDialog(QWidget *parent, QString defaultPath)
    : QDialog(parent)
    , ui(new Ui::NewTaskDialog)
{
    ui->setupUi(this);
    ui->path->setText(defaultPath);
    ui->isDefault->setCheckState(Qt::CheckState::Checked);
}

NewTaskDialog::~NewTaskDialog()
{
    delete ui;
}

void NewTaskDialog::on_btnOK_clicked()
{
    if (ui->url->text().isEmpty() || ui->path->text().isEmpty()) {
        qDebug() << "URL or save path is empty!";
        return;
    }
    this->accept();
}

void NewTaskDialog::on_btnCancel_clicked()
{
    this->reject();
}

QString NewTaskDialog::getDownloadUrl()
{
    return ui->url->text();
}

QString NewTaskDialog::getSavePath()
{
    return ui->path->text();
}

bool NewTaskDialog::isDefaultPath()
{
    return ui->isDefault->isChecked();
}

void NewTaskDialog::on_btnBrowse_clicked()
{
    QString savePath = QFileDialog::getSaveFileName(this, "Select Save Path");
    if (!savePath.isEmpty()) {
        ui->path->setText(savePath);
    }
}

