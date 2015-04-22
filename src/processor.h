#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QObject>
#include <QProcess>
#include <QString>
#include "configmanager.h"
#include "fileservice.h"

/// \brief Handles running the vpk.exe process and returning data from it.
class Processor : public QObject
{
		Q_OBJECT
	public:

		explicit Processor(ConfigManager *cfgManager, QObject *parent = 0);

		QString exePath() const;

		/// \brief Get a list of files contained inside the given vpk.
		/// \returns Empty QList if error or no files found.
		QList<FileService::FileInfo> getFiles(QString vpk);

	signals:

		void programOutput(QString output);
		void errorHappened(QString error);

	public slots:

	private:

		QProcess::ExitStatus runCommand(QString cmd, QString &out, QString &err);

		ConfigManager *mConfigManager;

};

#endif // PROCESSOR_H
