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
    ui->graphics->hide();
    ui->lblTitle->hide();

    switch(status) {
    case vsAccepted:
        ui->lblVoteStatus->setText("<html><head/><body><p style=\"color:#008000;\">Спасибо, Ваш голос принят</p></body></html>");
        ui->pbBack->hide();
        ui->label->hide();
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

    ui->lblVoteStatus->show();

    QTimer* timer = new QTimer(this);
    this->connect(timer, SIGNAL(timeout()), SLOT(onTimerStop()));
    //    timer->setInterval(5000);
    timer->setSingleShot(true);
    timer->start(5000);
}

void VoteDialog::onTimerStop()
{
    ui->lblVoteStatus->hide();
    ui->graphics->show();
    ui->pbBack->show();
    ui->lblTitle->show();
    ui->label->show();

    if (status == vsAccepted) {
        emit ThisHides();
        this->close();
    }
}

void VoteDialog::onShowVoteDialog(CategoryImage::CategoryImagePtr image)
{
    _currentImage = image;
    auto thumb = image->getImage(ui->graphics->geometry().size());
    ui->lblTitle->setText(image->name());
    ui->graphics->setPixmap(thumb);
    ui->graphics->show();
    ui->lblVoteStatus->setVisible(false);

    this->showFullScreen();
    this->raise();

    _waitForCode = true;
}

void VoteDialog::on_pbBack_clicked()
{
    emit ThisHides();
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
    status = VoteStatus::vsCodeUSed;
    ShowStatus();
}

void VoteDialog::onWrongCode()
{
    _waitForCode = true;
    status = VoteStatus::vsWrong;
    ShowStatus();
}

