#ifndef QTUNNEL_H
#define QTUNNEL_H

#include <QObject>
#include <qprocess.h>

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
    void errorRaised();

private slots:
    void deleteConfigFile();

private:
    void createLicFile();
    long createConfigFile();
    long generateContent(const QJsonObject& sourceObj, QTextStream& stream);

    long saveObj(const QString fileName, const QJsonObject &obj, bool useBinaryFormat);

    QString m_error = "";
    bool m_connected = "";
    QProcess* m_proc = nullptr;

    QString m_configFile = "";


};

#endif // QTUNNEL_H
