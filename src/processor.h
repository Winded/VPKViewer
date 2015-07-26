#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QObject>
#include <QProcess>
#include <QString>
#include "configservice.h"
#include "fileservice.h"

/// \brief Handles running the vpk.exe process and returning data from it.
class Processor : public QObject
{
		Q_OBJECT
	public:

        explicit Processor(ConfigService *cfgService, QObject *parent = 0);

        /// \brief Fill the given list with files from the given vpk
        void getFiles(QString vpk, QList<FileObject*> *outList);

	signals:

		void programOutput(QString output);
		void errorHappened(QString error);

	public slots:

	private:

		QProcess::ExitStatus runCommand(QString cmd, QString &out, QString &err);

        ConfigService *mConfigService;

};

#endif // PROCESSOR_H
