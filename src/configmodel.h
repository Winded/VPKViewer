#ifndef CONFIGMODEL_H
#define CONFIGMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include "configservice.h"

class ConfigModel : public QAbstractItemModel
{
        Q_OBJECT
    public:
        explicit ConfigModel(ConfigService *service, QObject *parent = 0);

        int rowCount(const QModelIndex &parent) const;
        int columnCount(const QModelIndex &parent) const;

        QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        QModelIndex parent(const QModelIndex &child) const;

        QVariant data(const QModelIndex &index, int role) const;
        bool setData(const QModelIndex &index, const QVariant &value, int role);

    private slots:
        void configAdded(int index);
        void configRemoved(int index);
        void refresh();

    private:
        ConfigService *mService;
};

#endif // CONFIGMODEL_H
