#ifndef CONFIGSERVICE_H
#define CONFIGSERVICE_H

#include <QObject>
#include <QVector>

class ConfigObject
{
    public:
        ConfigObject(QString name, QString exePath);

        QString name() const;
        void setName(QString name);

        QString exePath() const;
        void setExePath(QString exePath);

    private:
        QString mName;
        QString mExePath;
};

class ConfigService : public QObject
{
		Q_OBJECT

        Q_PROPERTY(int selectedIndex READ selectedIndex WRITE setSelectedIndex NOTIFY selectedIndexChanged)
    public:
        explicit ConfigService(QObject *parent = 0);
        ~ConfigService();

        ConfigObject *selectedConfig() const;
        void setSelectedConfig(ConfigObject *cfg);

        int selectedIndex() const;
        void setSelectedIndex(int index);

        QList<ConfigObject *> configs() const;
        void addConfig(ConfigObject *obj);
        void removeConfig(ConfigObject *obj);

        void load();
        void save();

    signals:
        void selectedIndexChanged();
        void loaded();

        void configAdded(int index);
        void configRemoved(int index);

    private:
        QList<ConfigObject*> mConfigs;
        int mSelectedIndex;
};

#endif // CONFIGSERVICE_H
