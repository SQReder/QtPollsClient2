#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>
#include <QSharedPointer>

#include "clientconnection.h"
#include "scanworker.h"

class CategorySelectWindow;
namespace Ui {
class Dialog;
}

class ConnectDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = 0);
    ~ConnectDialog();
    static QSharedPointer<QThread> _scanThread;
    static ScanWorker* _core;

private slots:
    void onAuthSuccess();
    void on_pbConnect_toggled(bool checked);

    void on_pbSelectFolder_clicked();

signals:
    void ShowCategorySelector();

private:
    Ui::Dialog *ui;
    QSharedPointer<CategorySelectWindow> _categorySelectWindow;
};

extern QString picsPath;
#endif // DIALOG_H
