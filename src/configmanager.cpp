#include "configmanager.h"
#include <cassert>

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

void ConfigManager::addConfig(ConfigManager::Config cfg) {
	bool s = false;
	getConfig(cfg.mName, &s);
	assert(!s);
	mConfigs.append(cfg);
}

ConfigManager::Config ConfigManager::getConfig(QString name, bool *ok) {
	for(Config cfg : mConfigs) {
		if(cfg.mName == name) {
			if(ok) *ok = true;
			return cfg;
		}
	}
	if(ok) *ok = false;
	return Config();
}

void ConfigManager::selectConfig(QString name, bool *ok) {
	bool s = false;
	Config cfg = getConfig(name, &s);
	if(!s) {
		if(ok) *ok = false;
		return;
	}
	mCurrentConfig = cfg;
	if(ok) *ok = true;
}
