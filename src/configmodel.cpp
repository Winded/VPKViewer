#include "configmodel.h"

ConfigModel::ConfigModel(ConfigService *service, QObject *parent) :
    QAbstractItemModel(parent),
    mService(service)
{
    connect(service, SIGNAL(loaded()), this, SLOT(refresh()));
    connect(service, SIGNAL(configAdded(int)), this, SLOT(configAdded(int)));
    connect(service, SIGNAL(configRemoved(int)), this, SLOT(configRemoved(int)));
}

int ConfigModel::rowCount(const QModelIndex &parent) const {
    if(parent.isValid()) {
        return 0;
    }
    return mService->configs().count();
}

int ConfigModel::columnCount(const QModelIndex &parent) const {
    return 2;
}

QModelIndex ConfigModel::index(int row, int column, const QModelIndex &parent) const {
    if(parent.isValid()) {
        return QModelIndex();
    }

    QList<ConfigObject*> cfgs = mService->configs();
    if(row < 0 || row >= cfgs.count()) {
        return QModelIndex();
    }
    ConfigObject *cfg = cfgs.at(row);
    return createIndex(row, column, cfg);
}

QModelIndex ConfigModel::parent(const QModelIndex &child) const {
    return QModelIndex();
}

QVariant ConfigModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid()) {
        return QVariant();
    }
    if(role != Qt::DisplayRole && role != Qt::EditRole) {
        return QVariant();
    }

    ConfigObject *obj = (ConfigObject*)index.internalPointer();
    switch(index.column()) {
    case 0:
        return obj->name();
    case 1:
        return obj->exePath();
    }
    return QVariant();
}

bool ConfigModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(!index.isValid()) {
        return false;
    }
    if(role != Qt::EditRole) {
        return false;
    }

    ConfigObject *obj = (ConfigObject*)index.internalPointer();
    switch(index.column()) {
    case 0:
        obj->setName(value.toString());
        break;
    case 1:
        obj->setExePath(value.toString());
        break;
    default:
        return false;
    }
    emit dataChanged(index, index);
    return true;
}

void ConfigModel::configAdded(int index) {
    index = index > 0 ? index - 1 : 0;
    beginInsertRows(QModelIndex(), index, index);
    endInsertRows();
}

void ConfigModel::configRemoved(int index) {
    index = index > 0 ? index - 1 : 0;
    beginRemoveRows(QModelIndex(), index, index);
    endRemoveRows();
}

void ConfigModel::refresh() {
    beginResetModel();
    endResetModel();
}
