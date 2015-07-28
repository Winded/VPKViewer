#include "configservice.h"
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>
#include <QDir>

ConfigService::ConfigService(QObject *parent) :
    QObject(parent),
    mSelectedIndex(-1)
{
}

ConfigService::~ConfigService() {
    for(ConfigObject *obj : mConfigs) {
        delete obj;
    }
}

ConfigObject *ConfigService::selectedConfig() const {
    if(mSelectedIndex < 0 || mSelectedIndex >= mConfigs.count()) {
        return 0;
    }
    return mConfigs.at(mSelectedIndex);
}

void ConfigService::setSelectedConfig(ConfigObject *cfg) {
    int index = mConfigs.indexOf(cfg);
    if(index == -1) {
        return;
    }
    mSelectedIndex = index;
    emit selectedIndexChanged();
}

int ConfigService::selectedIndex() const {
    return mSelectedIndex;
}

void ConfigService::setSelectedIndex(int index) {
    mSelectedIndex = index;
    emit selectedIndexChanged();
}

QList<ConfigObject *> ConfigService::configs() const {
    return mConfigs;
}

void ConfigService::addConfig(ConfigObject *obj) {
    mConfigs.append(obj);
    emit configAdded(mConfigs.indexOf(obj));
}

void ConfigService::removeConfig(ConfigObject *obj) {
    int i = mConfigs.indexOf(obj);
    if(i == -1) {
        return;
    }
    mConfigs.removeAt(i);
    emit configRemoved(i);
    delete obj;
}

void ConfigService::load() {
    setSelectedIndex(-1);
    qDeleteAll(mConfigs);
    mConfigs.clear();

    QString path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/VPKViewer/config.json";
    QFile file(path);
    if(!file.open(QFile::ReadOnly)) {
        return;
    }

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &err);
    if(err.error != QJsonParseError::NoError) {
        return;
    }

    QJsonObject obj = doc.object();
    QJsonArray jConfigs = obj["configs"].toArray();
    for(QJsonValue value : jConfigs) {
        QJsonObject cfgObj = value.toObject();
        QString name = cfgObj["name"].toString();
        QString exePath = cfgObj["exePath"].toString();
        ConfigObject *cfg = new ConfigObject(name, exePath);
        mConfigs.append(cfg);
    }
    int index = obj["selectedIndex"].toInt();
    setSelectedIndex(index);

    file.close();
    emit loaded();
}

void ConfigService::save() {
	QString sDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/VPKViewer";
	QString path = sDir + "/config.json";
	QDir dir(sDir);
	if(!dir.exists()) {
		dir.mkpath(sDir);
	}
	QFile file(path);
    if(!file.open(QFile::WriteOnly)) {
        return;
    }

    QJsonObject obj;
    obj["selectedConfig"] = mSelectedIndex;
    QJsonArray jConfigs;
    for(ConfigObject *cfg : mConfigs) {
        QJsonObject cfgObj;
        cfgObj["name"] = cfg->name();
        cfgObj["exePath"] = cfg->exePath();
        jConfigs.append(cfgObj);
    }
    obj["configs"] = jConfigs;
    QJsonDocument doc(obj);

    QByteArray json = doc.toJson();
    file.write(json);
    file.close();
}


ConfigObject::ConfigObject(QString name, QString exePath) :
    mName(name), mExePath(exePath)
{
}

QString ConfigObject::name() const {
    return mName;
}

void ConfigObject::setName(QString name) {
    mName = name;
}

QString ConfigObject::exePath() const {
    return mExePath;
}

void ConfigObject::setExePath(QString exePath) {
    mExePath = exePath;
}
