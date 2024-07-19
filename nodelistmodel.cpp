#include "nodelistmodel.h"
#include "licenseservice.h"
#include <qtcpsocket.h>


NodeListModel::NodeListModel(QObject *parent)
    : QAbstractListModel(parent) {


}

NodeListModel::~NodeListModel()
{
    for (auto* watcher: m_watcherList) {
        watcher->cancel();
        watcher->deleteLater();
    }
    m_watcherList.clear();
}

void NodeListModel::clear()
{
    qDebug() << "clearing NodeListModel";

    m_IsDestroyed = true;

    for (auto* watcher: m_watcherList) {
        watcher->cancel();
//      watcher->waitForFinished();
        watcher->deleteLater();
    }
    m_watcherList.clear();

    m_nodes.clear();

    qDebug() << "finish clearing NodeListModel";
}

void NodeListModel::init()
{
    beginResetModel();
    clear();
    endResetModel();

    LicenseService license;

    QVariantList nodes;
    long ret = license.getNodes(nodes);
    if (ret != 1 ) {
        emit errorRaised("Couldn't get nodes configuration", "Make sure the license file is valid");
        license.createLicFile();
        return;
    }

    if (nodes.count() == 0) {
        // log error, it seems json compatability error
        emit errorRaised("Couldn't get nodes configuration", "Make sure the license file is valid");
        return;
    }

    for (QVariant nodeObj: nodes) {

        QVariantMap nodeMap = nodeObj.toMap();

        Node node;
        node.name = nodeMap.value("name").toString();
        node.region = nodeMap.value("region").toString();
        node.address = nodeMap.value("address").toString();
        node.owner = nodeMap.value("owner").toString();
        node.ipAddress = nodeMap.value("ipAddress").toString();
        node.activated = false;

        addItem(node);
    }
}

void NodeListModel::th_nodeUpdate(QPromise<void>& promise, int iIndex)
{
    if (m_IsDestroyed) {
        promise.finish();
        return;
    }

    Node& node = m_nodes[iIndex];
    node.activated = checkConnection(node.ipAddress);

    promise.finish();
}

void NodeListModel::update()
{
    QList<Node> fakeNodes;
    QString dtFormat = "yyyy-MM-dd hh:mm";
    Node n1;
    n1.lastAccessTime = QDateTime::fromString("2023-12-12 17:04", dtFormat);
    n1.currWorkTime = 120;
    n1.totalWorkTime = 400;
    n1.currStartNumber = 500;
    n1.totalStartNumber = 1000;
    n1.status = NodeStatus::Normal;
    n1.activated = true;

    fakeNodes << n1;

    Node n2;
    n2.lastAccessTime = QDateTime::fromString("2023-11-19 11:44", dtFormat);
    n2.currWorkTime = 12;
    n2.totalWorkTime = 400;
    n2.currStartNumber = 7;
    n2.totalStartNumber = 1000;
    n2.status = NodeStatus::Normal;
    n2.activated = true;

    fakeNodes << n2;

    Node n3;
    n3.lastAccessTime = QDateTime::fromString("2023-07-09 13:04", dtFormat);
    n3.currWorkTime = 50;
    n3.totalWorkTime = 500;
    n3.currStartNumber = 10;
    n3.totalStartNumber = 1000;
    n3.status = NodeStatus::Normal;
    n3.activated = true;

    fakeNodes << n3;

    Node n4;
    n4.lastAccessTime = QDateTime::fromString("2023-11-19 11:44",  dtFormat);
    n4.currWorkTime = 700;
    n4.totalWorkTime = 500;
    n4.currStartNumber = 1200;
    n4.totalStartNumber = 1000;
    n4.status = NodeStatus::ServiceRequired; // service required
    n4.activated = false;

    fakeNodes << n4;

    m_IsDestroyed = false;

    int iIndex = 0;
    for (Node& node: m_nodes) {
        Node fakeNode = fakeNodes.value(iIndex, Node());
        node.lastAccessTime = fakeNode.lastAccessTime;
        node.currWorkTime = fakeNode.currWorkTime;
        node.currStartNumber = fakeNode.currStartNumber;
        node.totalWorkTime = fakeNode.totalWorkTime;
        node.totalStartNumber = fakeNode.totalStartNumber;
        node.status = fakeNode.status;

        auto future = QtConcurrent::run(&NodeListModel::th_nodeUpdate, this, iIndex);

        QFutureWatcher<void>* watcher = new QFutureWatcher<void>();
        QObject::connect(watcher, &QFutureWatcher<void>::finished, [this, iIndex]() {
            qDebug() << "Thread finished for index = " << iIndex;

            if (m_IsDestroyed) return;

            QList<int> roles;
            roles << LastAccessTimeRole << WorkTimeRole << StartNumberRole << StatusRole << ActivatedRole;
            QModelIndex mFirstIndex = this->index(iIndex);
            QModelIndex mLastIndex = this->index(iIndex);
            emit dataChanged(mFirstIndex, mLastIndex, roles);

        });

        watcher->setFuture(future);
        m_watcherList.append(watcher);

        iIndex++;
    }
}

bool NodeListModel::checkConnection(QString address)
{
    QTcpSocket tcpSocket;
    QHostAddress host(address);
    tcpSocket.connectToHost(host, 22);
    if (tcpSocket.waitForConnected(4000)) {
        return true;
    }

    return true;
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
        return node.activated ? node.lastAccessTime.toString( "yyyy-MM-dd hh:mm"): "";
    case WorkTimeRole:
        return node.activated ? QString("%1h(%2\%)").arg(node.currWorkTime).arg(workTimeProc) : "";
    case StartNumberRole:
        return node.activated ? QString("%1(%2\%)").arg(node.currStartNumber).arg(startNumberProc) : "";
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
