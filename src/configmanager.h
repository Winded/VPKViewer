#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QVector>

class ConfigManager : public QObject
{
		Q_OBJECT
	public:

		struct Config {
				QString mName;
				QString mExePath;
		};

		explicit ConfigManager(QObject *parent = 0);

		/// \brief Get the vpk.exe path for the current config
		QString exePath() const;
		/// \brief Get the name of the current config
		QString configName() const;

		QList<QString> configNames() const;

		void getConfig(QString name);
		void selectConfig(QString name);

	signals:

	public slots:

	private:

		QList<Config> mConfigs;
		Config mCurrentConfig;

};

#endif // CONFIGMANAGER_H
