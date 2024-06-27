#ifndef QTUNNEL_H
#define QTUNNEL_H

#include <QObject>
#include <qprocess.h>
#include "licenseservice.h"


class QTunnel : public QObject
{
    Q_OBJECT
public:
    explicit QTunnel(QObject *parent = nullptr);
    ~QTunnel();

    Q_INVOKABLE void connect();

    Q_INVOKABLE QString lastError();

signals:
    void connected();
    void errorRaised(QString error, QString informativeError);

private slots:
    void deleteConfigFile();
    void checkConnection();
private:
    long createConfigFile();
    long generateContent(const QVariantMap &configOb, QTextStream& stream);


    QString m_error = "";
    bool m_connected = "";
    QProcess* m_proc = nullptr;

    QString m_configFile = "";


};

#endif // QTUNNEL_H
