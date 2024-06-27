#ifndef TUNNELSERVICE_H
#define TUNNELSERVICE_H

#include <QProcess>
#include <QVariantMap>


class TunnelService
{
public:
    TunnelService();
    long connect(const QVariantMap &configObj);
    long checkConnection(const QVariantMap &configObj);
    long closeConnection();
    void onConnected();

private:
    long createConfigFile(const QVariantMap &configObj);
    long generateContent(const QVariantMap &configOb, QTextStream& stream);

    QProcess* m_proc = nullptr;
    QString m_configFile = "";

};

#endif // TUNNELSERVICE_H
