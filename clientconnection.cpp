#include <QByteArray>
#include <QHostAddress>

#include "clientconnection.h"
#include "../Server/protocol.h"
#include "../Server/logger.h"

QSharedPointer<ClientConnection> ClientConnection::_instance = QSharedPointer<ClientConnection>();

ClientConnection::ClientConnection(QObject *parent) :
    QObject(parent)
{
    _sok = new QTcpSocket(this);
    connect(_sok, SIGNAL(readyRead()), this, SLOT(onSokReadyRead()));
    connect(_sok, SIGNAL(connected()), this, SLOT(onSokConnected()));
    connect(_sok, SIGNAL(disconnected()), this, SLOT(onSokDisconnected()));
    connect(_sok, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(onSokDisplayError(QAbstractSocket::SocketError)));
}

QSharedPointer<ClientConnection> ClientConnection::Instance()
{
    if (_instance.isNull())
        _instance = QSharedPointer<ClientConnection>(new ClientConnection());
    return _instance;
}


void ClientConnection::onSokDisplayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        Logger::error("Remote host closed");
        break;
    case QAbstractSocket::HostNotFoundError:
        Logger::error("The host was not found");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        Logger::error("The connection was refused by the peer.");
        break;
    default:
        Logger::error("The following error occurred: "+_sok->errorString());
    }
}

void ClientConnection::onSokConnected()
{
    _blockSize = 0;
    Logger::success("Connected to"+_sok->peerAddress().toString()+":"+QString::number(_sok->peerPort()));

    //try autch
    doSendCommand(ProtocolCommand::comAuthRequest);
}

void ClientConnection::onSokDisconnected()
{
    Logger::success("Disconnected from"+_sok->peerAddress().toString()+":"+QString::number(_sok->peerPort()));
}

void ClientConnection::onSokReadyRead() {
    QDataStream in(_sok);
    //если считываем новый блок первые 2 байта это его размер
    if (_blockSize == 0) {
        //если пришло меньше 2 байт ждем пока будет 2 байта
        if (_sok->bytesAvailable() < (qint64)sizeof(qint64))
            return;
        //считываем размер (2 байта)
        in >> _blockSize;
        qDebug() << "_blockSize now " << _blockSize;
    }
    //ждем пока блок прийдет полностью
    if (_sok->bytesAvailable() < _blockSize)
        return;
    else
        //можно принимать новый блок
        _blockSize = 0;
    //3 байт - команда серверу
    quint8 command;
    in >> command;
    qDebug() << "Received command " << command;


    switch(ProtocolCommand(command))
    {
    case ProtocolCommand::comAuthSuccess:
        emit AuthSuccess();
        break;
    case ProtocolCommand::comCodeVerified:
        emit CodeVerified();
        break;
    case ProtocolCommand::comCodeNotVerified:
        emit CodeNotVerified();
        break;
    case ProtocolCommand::comCodeAlreadyUsed:
        emit CodeAlreadyUsed();
        break;
    default:
        break;
    }

}

void ClientConnection::doConnectToHost(QString addr, int port) {
    _sok->connectToHost(addr, port);
}

void ClientConnection::doDisconnect() {
    _sok->close();
}

void ClientConnection::doSendCommand(qint8 command, QStringList data) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (qint64)0;
    out << command;
    for(auto &item : data)
        out << item;

    out.device()->seek(0);
    out << (qint64)(block.size() - sizeof(qint64));
    _sok->write(block);
    _sok->flush();
}

void ClientConnection::doSendCommand(qint8 command) {
    QStringList blankList;
    doSendCommand(command, blankList);
}
