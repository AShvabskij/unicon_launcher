#include "nodelistmodel.h"


NodeListModel::NodeListModel(QObject *parent)
    : QAbstractListModel(parent) {


}

void NodeListModel::init()
{
    QString dtFormat = "yyyy-MM-dd hh:mm";
    Node n1;
    n1.region = "Katar";
    n1.name = "Starter 1";
    n1.address = "Katar techo park";
    n1.owner = "Totem Ltd";
    n1.lastAccessTime = QDateTime::fromString("2023-12-12 17:04", dtFormat);
    n1.currWorkTime = 120;
    n1.totalWorkTime = 400;
    n1.currStartNumber = 500;
    n1.totalStartNumber = 1000;
    n1.status = NodeStatus::Normal;
    n1.ipAddress = "10.9.0.1";

    m_nodes << n1;

    Node n2;
    n2.region = "Katar";
    n2.name = "Starter 2";
    n2.address = "Katar techo park";
    n2.owner = "Totem Ltd";
    n2.lastAccessTime = QDateTime::fromString("2023-11-19 11:44", dtFormat);
    n2.currWorkTime = 12;
    n2.totalWorkTime = 400;
    n2.currStartNumber = 7;
    n2.totalStartNumber = 1000;
    n2.status = NodeStatus::Normal;
    n2.ipAddress = "10.9.0.2";

    m_nodes << n2;

    Node n3;
    n3.region = "Munhen";
    n3.name = "RND Lab";
    n3.address = "Germany Hermann-Ritter-Str. 110 Bremen";
    n3.owner = "IGEL Electric GmbH";
    n3.lastAccessTime = QDateTime::fromString("2023-07-09 13:04", dtFormat);
    n3.currWorkTime = 50;
    n3.totalWorkTime = 500;
    n3.currStartNumber = 10;
    n3.totalStartNumber = 1000;
    n3.status = NodeStatus::Normal;
    n3.ipAddress = "10.9.0.3";
    n3.activated = false;

    m_nodes << n3;

    Node n4;
    n4.region = "Munhen";
    n4.name = "Starter 1";
    n4.address = "Germany Munhen";
    n4.owner = "IGEL Electric GmbH";
    n4.lastAccessTime = QDateTime::fromString("2023-11-19 11:44",  dtFormat);
    n4.currWorkTime = 700;
    n4.totalWorkTime = 500;
    n4.currStartNumber = 1200;
    n4.totalStartNumber = 1000;
    n4.status = NodeStatus::ServiceRequired; // service required
    n4.ipAddress = "10.9.0.4";

    m_nodes << n4;
}

void NodeListModel::addItem(const Node &node)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_nodes << node;
    endInsertRows();
}

int NodeListModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_nodes.count();
}

QVariant NodeListModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_nodes.count()) return QVariant();

    const Node &node = m_nodes[index.row()];
    int workTimeProc = ((float)node.currWorkTime/(float)node.totalWorkTime)*100;
    int startNumberProc = ((float)node.currStartNumber/(float)node.totalStartNumber)*100;

    switch (role) {
    case NameRole:
        return node.name;
    case RegionRole:
        return node.region;
    case OwnerRole:
        return node.owner;
    case AddressRole:
        return node.address;
    case LastAccessTimeRole:
        return node.lastAccessTime.toString( "yyyy-MM-dd hh:mm");
    case WorkTimeRole:
        return QString("%1h(%2\%)").arg(node.currWorkTime).arg(workTimeProc);
    case StartNumberRole:
        return QString("%1(%2\%)").arg(node.currStartNumber).arg(startNumberProc);
    case StatusRole:
        return node.activated ? node.nodeStatusToString() : "Not available";
    case IPAddressRole:
        return node.ipAddress;
    case ActivatedRole:
        return node.activated;

    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> NodeListModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[NameRole] = "name";
    roles[RegionRole] = "region";
    roles[OwnerRole] = "owner";
    roles[AddressRole] = "address";
    roles[LastAccessTimeRole] = "lastAccessTime";
    roles[WorkTimeRole] = "workTime";
    roles[StartNumberRole] = "startNumber";
    roles[StatusRole] = "status";
    roles[IPAddressRole] = "ipAddress";
    roles[ActivatedRole] = "activated";

    return roles;
}
