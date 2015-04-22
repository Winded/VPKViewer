#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QVector>

class ConfigManager : public QObject
{
		Q_OBJECT
	public:

		struct Config {
				Config() {}
				Config(QString name, QString exePath) :
					mName(name), mExePath(exePath)
				{
				}

				QString mName;
				QString mExePath;
		};

		explicit ConfigManager(QObject *parent = 0);

		/// \brief Get the vpk.exe path for the current config
		QString exePath() const;
		/// \brief Get the name of the current config
		QString configName() const;

		QList<QString> configNames() const;

		void addConfig(Config cfg);

		Config getConfig(QString name, bool *ok = 0);
		void selectConfig(QString name, bool *ok = 0);

	signals:

	public slots:

	private:

		QList<Config> mConfigs;
		Config mCurrentConfig;

};

#endif // CONFIGMANAGER_H
