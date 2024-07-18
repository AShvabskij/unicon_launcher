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
    Q_INVOKABLE void scanIP();
    void th_scanIP(QPromise<void> &promise);
    Q_INVOKABLE QString getURL();

signals:
    void connected();
    void errorRaised(QString error, QString informativeError);

private:
    QStringList m_listIP;

    QFuture<void> future;
    QFutureWatcher<void> watcher;

};

#endif // LOCALMODEL_H
