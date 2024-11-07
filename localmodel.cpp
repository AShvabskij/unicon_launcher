#include "localmodel.h"

#include <qdebug>
#include <qtcpsocket.h>

#include "RSP_IP_WIN_discovery/RSP_IP_DISCOVERY.h"

LocalModel::LocalModel() {}

void LocalModel::scanIP()
{
    future = QtConcurrent::run(&LocalModel::th_scanIP, this);

    QObject::connect(&watcher, &QFutureWatcher<void>::finished, [this]() {
        qDebug() << "Asynchronous member function finished!";
        if (m_listIP.count() == 0) {
            emit errorRaised("No devices are connected!", "Make sure a device is connected directly or is in the local network");
        } else {
            emit connected();
        }
    });

    watcher.setFuture(future);

}

void LocalModel::th_scanIP(QPromise<void>& promise)
{
    RSP_IP_DISCOVERY rsp_ip;

    rsp_ip.run_IP_discovery();

    std::vector <std::string> ip_list;

    ip_list = rsp_ip.get_IP_list();

    m_listIP.clear();

    for (std::string& s : ip_list)
    {
        QString address = s.c_str();
        qDebug() << address << Qt::endl;

        m_listIP.append(address);

        // QTcpSocket tcpSocket;
        // QHostAddress host(address);
        // tcpSocket.connectToHost(host, 22);
        // if (tcpSocket.waitForConnected(1000)) {
        //     m_listIP.append(address);
        // }
    }

    promise.finish();
}

QString LocalModel::getURL()
{
    QString res = !m_listIP.empty() ? "http://" + m_listIP.first() + ":3000" : "";

    return res;
}
