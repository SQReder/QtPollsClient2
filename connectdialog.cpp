#include "connectDialog.h"
#include "categoryselectwindow.h"
#include "ui_connectdialog.h"

#include <QtGui>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

ScanWorker *ConnectDialog::_core = nullptr;
QSharedPointer<QThread> ConnectDialog::_scanThread = QSharedPointer<QThread>();

QString picsPath;


ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    auto _client = ClientConnection::Instance();
    connect(_client.data(), SIGNAL(AuthSuccess()), this, SLOT(onAuthSuccess()));

    _scanThread = QSharedPointer<QThread>(new QThread());
    try {
        _core = ScanWorker::Instance().data();
        _core->moveToThread(_scanThread.data());
        connect(_scanThread.data(), SIGNAL(started()), _core, SLOT(work()));
        connect(_core, SIGNAL(finished()), _scanThread.data(), SLOT(quit()));
        connect(_core, SIGNAL(codeScanned(QString)), this, SLOT(onCodeScanned(QString)));
    } catch (std::exception &e) {
        throw e;
    }

}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

void ConnectDialog::on_pbConnect_toggled(bool checked)
{
    auto client = ClientConnection::Instance();
    if (checked) {
        if (ui->lePicsPath->text() == "") {
            onAddToLog("Select folder with pics", Qt::darkRed);
            return;
        }

        picsPath = ui->lePicsPath->text();
        if (picsPath.right(1) != "/")
            picsPath.append("/");

        client->doConnectToHost(ui->leHost->text(), ui->sbPort->value());
        ui->pbConnect->setText("Process...");
    } else {
        client->doDisconnect();
    }

}

void ConnectDialog::onAuthSuccess() {
    try {
        _categorySelectWindow = new CategorySelectWindow(this);

//        _categorySelectWindow->loadCategories(dirs);
        _categorySelectWindow->raise();
        _categorySelectWindow->showFullScreen();

        this->hide();
    } catch (std::exception &e){
        emit onAddToLog(QString("") + QString(e.what()));
    }
}

void ConnectDialog::onAddToLog(QString text, QColor color)
{
    ui->lwLog->insertItem(0, QTime::currentTime().toString()+" "+text);
    ui->lwLog->item(0)->setTextColor(color);
}

void ConnectDialog::on_pbSelectFolder_clicked()
{
    QString path;
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    if(dialog.exec()) {
        path = dialog.selectedFiles()[0];
        ui->lePicsPath->setText(path);
    }
}
