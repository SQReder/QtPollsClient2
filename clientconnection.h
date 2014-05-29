#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QColor>
#include <QSharedPointer>
#include "../Server/protocol.h"

class ClientConnection : public QObject
{
    Q_OBJECT
    explicit ClientConnection(QObject *parent = 0);
    static QSharedPointer<ClientConnection> _instance;

public:
    static QSharedPointer<ClientConnection> Instance();

    void doConnectToHost(QString addr, int port);
    void doDisconnect();
    void doSendCommand(qint8 command);
    void doSendCommand(qint8 command, QStringList data);

    void ValidateCode(QByteArray code);
signals:
    void AuthSuccess();
    void CodeVerified();
    void CodeNotVerified();
    void CodeAlreadyUsed();

private slots:
    void onSokConnected();
    void onSokDisconnected();
    void onSokReadyRead();
    void onSokDisplayError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *_sok;
    qint64 _blockSize;
};


#endif // CLIENTCONNECTION_H
