#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QObject>
#include <QProcess>
#include "configmanager.h"

/// \brief Handles running the vpk.exe process and returning data from it.
class Processor : public QObject
{
		Q_OBJECT
	public:
		struct FileInfo {
				QString mPath;
				int mSize;
		};

		explicit Processor(ConfigManager *cfgManager, QObject *parent = 0);

		QString exePath() const;

		/// \brief Get a list of files contained inside the given vpk.
		/// \returns Empty QVector if error or no files found.
		QVector<FileInfo> getFiles(QString vpk);

	signals:

		void errorHappened(QString error);

	public slots:

	private:

		QProcess::ExitStatus runCommand(QString cmd, QString &out, QString &err);

		ConfigManager *mConfigManager;

};

#endif // PROCESSOR_H
