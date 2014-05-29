#include "connectDialog.h"
#include "categoryselectwindow.h"
#include "ui_connectdialog.h"


#include <QtGui>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include "../Server/logger.h"


QString picsPath;

ConnectDialog::ConnectDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    auto _client = ClientConnection::Instance();
    connect(_client.data(), SIGNAL(AuthSuccess()), this, SLOT(onAuthSuccess()));

    Logger::assignList(ui->lwLog);
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
            Logger::error("Select folder with pics");
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
        CreateScannerWorkerThread();

        _categorySelectWindow = new CategorySelectWindow(this);
        _categorySelectWindow->createCategorySelectors();

        _categorySelectWindow->show();
        qDebug() << _categorySelectWindow->pos() << _categorySelectWindow->size();

        _categorySelectWindow->raise();
        _categorySelectWindow->showFullScreen();

        this->hide();
    } catch (std::exception &e){
        Logger::error(QString(e.what()));
    }
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
