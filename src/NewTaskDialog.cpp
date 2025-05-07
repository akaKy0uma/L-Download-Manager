#include "NewTaskDialog.h"
#include "ui_NewTaskDialog.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

NewTaskDialog::NewTaskDialog(QWidget *parent, QString defaultPath)
    : QDialog(parent)
    , ui(new Ui::NewTaskDialog)
{
    ui->setupUi(this);
    ui->path->setText(defaultPath);
    ui->isDefault->setCheckState(Qt::CheckState::Checked);

    // 测试链接
    ui->url->setText("http://vjs.zencdn.net/v/oceans.mp4");
}

NewTaskDialog::~NewTaskDialog()
{
    delete ui;
}

void NewTaskDialog::on_btnOK_clicked()
{
    if (ui->url->text().isEmpty()) {
        QMessageBox::warning(this, tr("Invalid Input"), tr("Please enter a valid URL."));
        return;
    }

    if (ui->path->text().isEmpty()) {
        QMessageBox::warning(this, tr("Invalid Input"), tr("Please select a valid save path."));
        return;
    }

    QUrl url(ui->url->text());
    if (!url.isValid() || url.scheme().isEmpty()) {
        QMessageBox::warning(this, tr("Invalid URL"), tr("The URL you entered is not valid."));
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

