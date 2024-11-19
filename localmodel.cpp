#include "localmodel.h"

#include <qdebug>
#include <qtcpsocket.h>

#include "RSP_IP_WIN_discovery/RSP_IP_DISCOVERY.h"

LocalModel::LocalModel() {}

void LocalModel::scanIP()
{
    future = QtConcurrent::run(&LocalModel::th_scanIP, this);

    QObject::connect(&watcher, &QFutureWatcher<void>::finished, [this]() {
        qDebug() << "Asynchronous scan IP finished!";
        if (m_listIP.count() == 0) {
            QString errMsg = "No devices are connected!";
            qDebug() << errMsg;

            emit errorRaised(errMsg, "Make sure a device is connected directly or is in the local network");
        } else {
            emit connected();
        }
    });

    watcher.setFuture(future);
}

QTextStream& qStdOut()
{
    static QTextStream ts( stdout );
    return ts;
}

void LocalModel::th_scanIP(QPromise<void>& promise)
{
    qDebug() << "Asynchronous scan IP started!";

    RSP_IP_DISCOVERY rsp_ip;

    rsp_ip.run_IP_discovery();

    qDebug() << "Run IP discovery finished!";

    std::vector <std::string> ip_list;

    ip_list = rsp_ip.get_IP_list();

    m_listIP.clear();

    for (std::string& s : ip_list)
    {
        QString address = s.c_str();
        qDebug() << "detected address" << address << Qt::endl;

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
    if (m_listIP.count() ==  0) {
        qDebug() << "getURL: No addresses found!" << Qt::endl;
    }

    if (m_listIP.count() > 1) {
        qDebug() << "getURL: found addresses =" << m_listIP.count() << Qt::endl;
    }

    QString res = !m_listIP.empty() ? "http://" + m_listIP.first() + ":3000" : "";

    qDebug() << "getURL: result = " << res << Qt::endl;

    return res;
}
