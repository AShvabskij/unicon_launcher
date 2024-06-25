#ifndef NODELISTMODEL_H
#define NODELISTMODEL_H

#include <QAbstractListModel>
#include <qdatetime.h>
#include <QtQml/qqmlregistration.h>

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
    int currWorkTime;
    int totalWorkTime;
    int currStartNumber;
    int totalStartNumber;
    NodeStatus status;
    QString ipAddress;
    bool activated = true;

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
        workTimeRole,
        startNumberRole,
        statusRole,
        activeRole,
        ipAddressRole

    };

    NodeListModel(QObject *parent = nullptr);
    Q_INVOKABLE void init();

    void addItem(const Node &node);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<Node> m_nodes;

};

#endif // NODELISTMODEL_H
