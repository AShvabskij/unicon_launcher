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
    long connect(const QVariantMap &configObj);
    long checkConnection();
    long closeConnection();
    void finish();

private:
    long createConfigFile(const QVariantMap &configObj);
    long generateContent(const QVariantMap &configObj, QTextStream& stream);

    QProcess* m_proc = nullptr;
    QString m_configFile = "";
    QString m_serverIP = "";


};

#endif // TUNNELSERVICE_H
