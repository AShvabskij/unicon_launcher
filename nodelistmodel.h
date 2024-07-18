#ifndef NODELISTMODEL_H
#define NODELISTMODEL_H

#include <QAbstractListModel>
#include <qdatetime.h>
#include <QtQml/qqmlregistration.h>

#include <QPromise>
#include <QtConcurrent/QtConcurrent>

enum NodeStatus {
    Normal = 0,
    ServiceRequired
};

struct Node
{
    QString name;
    QString region;
    QString owner;
    QString address;
    QDateTime lastAccessTime;
    int currWorkTime = 0;
    int totalWorkTime = 0;
    int currStartNumber = 0;
    int totalStartNumber = 0;
    NodeStatus status;
    QString ipAddress;
    bool activated = false;

    QString nodeStatusToString() const {
        switch (this->status) {
        case Normal: return "";
        case ServiceRequired: return "service required!";
        default: return "";
        }
    }

};

class NodeListModel: public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum PersonRoles {
        NameRole = Qt::UserRole + 1,
        RegionRole,
        OwnerRole,
        AddressRole,
        LastAccessTimeRole,
        WorkTimeRole,
        StartNumberRole,
        StatusRole,
        ActivatedRole,
        IPAddressRole

    };

    NodeListModel(QObject *parent = nullptr);
    ~NodeListModel();

    Q_INVOKABLE void init();
    Q_INVOKABLE void update();

    void addItem(const Node &node);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;


signals:
    void errorRaised(QString error, QString informativeError);

protected:
    QHash<int, QByteArray> roleNames() const;

    QFuture<void> future;
    QFutureWatcher<void> watcher;

private:
    bool checkConnection(QString address);
    void th_update();

    QList<Node> m_nodes;

};

#endif // NODELISTMODEL_H
