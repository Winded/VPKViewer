#include "configmanager.h"

ConfigManager::ConfigManager(QObject *parent) :
	QObject(parent)
{
}

QString ConfigManager::exePath() const {
	return mCurrentConfig.mExePath;
}

QString ConfigManager::configName() const {
	return mCurrentConfig.mName;
}

QList<QString> ConfigManager::configNames() const {
	QList<QString> list;
	for(Config cfg : mConfigs) {
		list.append(cfg.mName);
	}
	return list;
}
