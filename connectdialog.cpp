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
        _categorySelectWindow->createCategorySelectors();

//        _categorySelectWindow->loadCategories(dirs);
        _categorySelectWindow->raise();
        _categorySelectWindow->showFullScreen();

        this->connect(_categorySelectWindow, SIGNAL(finished(int)), SLOT(done(int)));

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

void ConnectDialog::done(int code) {
    ScanWorker::Instance()->StopCam();
    emit ScanWorker::Instance()->finished();

    exit(0);
}
