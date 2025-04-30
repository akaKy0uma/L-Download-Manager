#ifndef NEWTASKDIALOG_H
#define NEWTASKDIALOG_H

#include <QDialog>

namespace Ui {
class NewTaskDialog;
}

class NewTaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewTaskDialog(QWidget *parent, QString defaultPath);
    ~NewTaskDialog();

    QString getDownloadUrl();
    QString getSavePath();
    bool isDefaultPath();

private slots:
    void on_btnOK_clicked();
    void on_btnCancel_clicked();

    void on_btnBrowse_clicked();

private:
    Ui::NewTaskDialog *ui;
};

#endif // NEWTASKDIALOG_H
