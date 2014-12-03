#ifndef UDPHANDLER_H
#define UDPHANDLER_H

#include <QObject>
#include <QUdpSocket>
#include <QList>

class MusicSphere;
class MusicTracker;

class UDPHandler : public QObject
{
    Q_OBJECT
public:
    UDPHandler(MusicSphere *ms, QObject *parent = 0);
    UDPHandler(MusicTracker *ms, QObject *parent = 0);
    virtual ~UDPHandler();

    void requestData();

signals:
    void sendFunction(QList<float> function);

public slots:
    void readyRead();

private:
    QUdpSocket *m_socket;

};

#endif // UDPHANDLER_H
