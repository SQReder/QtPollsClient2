#include "votedialog.h"
#include "ui_votedialog.h"
#include "clientconnection.h"
#include <scanworker.h>

VoteDialog::VoteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VoteDialog)
{
    ui->setupUi(this);
    _waitForCode = false;

    auto worker = ScanWorker::Instance();
    QObject::connect(worker, SIGNAL(codeScanned(QString)), this, SLOT(onCodeScanned(QString)));

    auto client = ClientConnection::Instance();
    ClientConnection *cl = client.data();
    QObject::connect(cl, SIGNAL(CodeVerified()),    this, SLOT(onCodeVerified()));
    QObject::connect(cl, SIGNAL(CodeNotVerified()), this, SLOT(onWrongCode()));
    QObject::connect(cl, SIGNAL(CodeAlreadyUsed()), this, SLOT(onCodeAlreadyUsed()));
}

VoteDialog::~VoteDialog()
{
    delete ui;
}

void VoteDialog::ShowStatus()
{
    switch(status) {
    case vsAccepted:
        ui->lblVoteStatus->setText("<html><head/><body><p style=\"color:#008000;\">Спасибо, Ваш голос принят</p></body></html>");
        break;
    case vsCodeUSed:
        ui->lblVoteStatus->setText("<html><head/><body><p style=\" font-weight:600; color:#bf0000;\">Этот код уже использован для голосования в этой категории</p></body></html>");
        break;
    case vsWrong:
        ui->lblVoteStatus->setText("<html><head/><body><p style=\" font-weight:600; color:#bf0000;\">Неверный код</p></body></html>");
        break;
    default:;
    }

    this->move(QPoint(0,0));

    ui->lblVoteStatus->setVisible(true);

    QTimer* timer = new QTimer(this);
    this->connect(timer, SIGNAL(timeout()), SLOT(onTimerStop()));
    timer->setInterval(6000);
    timer->setSingleShot(true);
    timer->start();
}

void VoteDialog::onShowVoteDialog(CategoryImage::CategoryImagePtr image)
{
    _currentImage = image;
    auto thumb = image->getImage(ui->graphics->geometry().size());
    ui->lblTitle->setText(image->name());
    this->showFullScreen();
    this->raise();
    ui->graphics->setPixmap(thumb);

    ui->lblVoteStatus->setVisible(false);

    _waitForCode = true;
}

void VoteDialog::on_pbBack_clicked()
{
    this->hide();
}

void VoteDialog::onCodeScanned(QString code)
{
    if (_waitForCode) {
        QStringList params;
        params.append(code);
        params.append(_currentImage->name());
        params.append(_currentImage->category());

        auto client = ClientConnection::Instance();
        client->doSendCommand(ProtocolCommand::comVoteUp, params);
        _waitForCode = false;
    }
}

void VoteDialog::onCodeVerified()
{
//    this->hide();
    status = VoteStatus::vsAccepted;
    ShowStatus();
}

void VoteDialog::onCodeAlreadyUsed()
{
    _waitForCode = true;
    status = vsCodeUSed;
    ShowStatus();
}

void VoteDialog::onWrongCode()
{
    _waitForCode = true;
    status = vsWrong;
    ShowStatus();
}

void VoteDialog::onTimerStop()
{
    ui->lblVoteStatus->setVisible(false);
    if (status == vsAccepted)
        this->hide();
}

