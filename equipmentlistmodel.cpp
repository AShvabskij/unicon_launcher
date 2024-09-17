#include "equipmentlistmodel.h"

const int EQUIPMENT_COUNT = 3;

EquipmentListModel::EquipmentListModel(QObject *parent) {

    QVariantMap item;
    item["name"] = "";
    item["region"] = "";
    item["address"] = "";

    for (int i =0; i < EQUIPMENT_COUNT; i++) {
        m_dataList.append(item);
    }
}

void EquipmentListModel::addItem(const Equipment &item)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_dataList << item;
    endInsertRows();
}

void EquipmentListModel::clear()
{
    beginResetModel();
    m_dataList.clear();
    endResetModel();
}

int EquipmentListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_dataList.count();
}

QHash<int, QByteArray> EquipmentListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[RegionRole] = "region";
    roles[AddressRole] = "address";

    return roles;
}

QVariant EquipmentListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_dataList.count())
        return QVariant();

    const auto& item = m_dataList[index.row()];
    if (role == NameRole)
        return item.name;
    else if (role == RegionRole)
        return item.region;
    else if (role == AddressRole)
        return item.address;

    return QVariant();
}

void EquipmentListModel::update(int index, const QJsonValue val)
{
    if (index < 0 || index >= m_dataList.size())
        return;

    QVariantMap var = val.toVariant().toMap();
    Equipment e(val);

    m_dataList[index] = e;  // Replace the person at the given index

    // Notify the view about the change
    // QModelIndex modelIndex = createIndex(index, 0);
    // emit dataChanged(modelIndex, modelIndex, {NameRole, RegionRole, AddressRole});
}
