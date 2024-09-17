#ifndef EQUIPMENTLISTMODEL_H
#define EQUIPMENTLISTMODEL_H

#include <QObject>
#include <QJsonValue>
#include <QJsonObject>

#include <QAbstractListModel>
#include <QtQml/qqmlregistration.h>

class EquipmentListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum EquipmentRoles {
        NameRole = Qt::UserRole + 1,
        RegionRole,
        AddressRole,
    };

    struct Equipment
    {
        QString name;
        QString region;
        QString address;

        Equipment() {
            name = "";
            region = "";
            address = "";
        }

        Equipment(const QJsonValue& value) {
            if (value.isObject()) {
                QJsonObject obj = value.toObject();

                this->name = obj.contains("name") ? obj["name"].toString() : this->name;
                this->region = obj.contains("region") ? obj["region"].toString() : this->region;
                this->address = obj.contains("address") ? obj["address"].toString() : this->address;
            }
        }

        Equipment(const QVariantMap& val) {
            this->name = val["name"].toString();
            this->region = val["region"].toString();
            this->address = val["address"].toString();
        }

    };

    EquipmentListModel(QObject *parent = nullptr);
    ~EquipmentListModel() = default;

    void addItem(const Equipment &item);
    void clear();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE void update(int index, const QJsonValue val);


protected:
    QHash<int, QByteArray> roleNames() const;

private:

    QList<Equipment> m_dataList;

};

#endif // EQUIPMENTLISTMODEL_H
