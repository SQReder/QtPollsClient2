#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>

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

public slots:
    void onAddToLog(QString text, QColor color = Qt::black);

private slots:
    void onAuthSuccess();
    void on_pbConnect_toggled(bool checked);

    void on_pbSelectFolder_clicked();

private:
    Ui::Dialog *ui;
    CategorySelectWindow *_categorySelectWindow;
};

extern QString picsPath;
#endif // DIALOG_H
