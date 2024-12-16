#ifndef TUNNELSERVICE_H
#define TUNNELSERVICE_H

#include <QProcess>
#include <QVariantMap>


class TunnelService
{
public:
    TunnelService();

    static TunnelService* instance() {
        static TunnelService i;
        return &i;
    }

    QString genPrivateKey();
    QString genPublicKey(QString privateKey);
    long connect(const QVariantMap &configObj);
    long checkConnection();
    long closeConnection();
    void finish();
    bool isConnected();
    QString getSubnet();

private:
    long createConfigFile(const QVariantMap &configObj);
    long generateContent(const QVariantMap &configObj, QTextStream& stream);

    QProcess* m_proc = nullptr;
    QString m_configFile = "";
    QString m_serverIP = "";
    QString m_subnet = "";
    bool m_isConnected = false;

};

#endif // TUNNELSERVICE_H
