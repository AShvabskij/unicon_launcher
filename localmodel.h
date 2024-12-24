#ifndef LOCALMODEL_H
#define LOCALMODEL_H

#include <QObject>
#include <QPromise>
#include <QtConcurrent/QtConcurrent>

class LocalModel : public QObject
{
    Q_OBJECT
public:
    LocalModel();
    ~LocalModel();
    Q_INVOKABLE void scanIP();
    void th_scanIP(QPromise<void> &promise);
    Q_INVOKABLE QString getURL();

signals:
    void connected();
    void errorRaised(QString errorTxt, QString informativeError);

private:
    bool checkConnection(QString address);

    QStringList m_listIP;

    QFuture<void> m_future;
    QFutureWatcher<void> m_watcher;

};

#endif // LOCALMODEL_H
