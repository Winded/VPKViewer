#include "processor.h"

Processor::Processor(ConfigManager *cfgManager, QObject *parent) :
	QObject(parent),
	mConfigManager(cfgManager)
{
}

QString Processor::exePath() const {
	return mConfigManager->exePath();
}

QVector<Processor::FileInfo> Processor::getFiles(QString vpk) {
	QVector<Processor::FileInfo> list;
	QString cmd = "L " + vpk;
	QString output, error;
	QProcess::ExitStatus status = runCommand(cmd, output, error);
	if(status == QProcess::CrashExit || !error.isEmpty()) {
		return list;
	}

	QStringList sList = output.split("\n");
	for(QString s : sList) {
		// TODO: Line parsing. Regex?
	}
}

QProcess::ExitStatus Processor::runCommand(QString cmd, QString &out, QString &err) {
	QString fullCmd = exePath() + " " + cmd;

	QProcess p(this);
	p.start(fullCmd, QProcess::ReadWrite);
	while(p.state() != QProcess::NotRunning) {}

	out = QString(p.readAllStandardOutput());
	err = QString(p.readAllStandardError());
	QProcess::ExitStatus status = p.exitStatus();

	if(!err.isEmpty()) {
		emit errorHappened(err);
	}

	p.close();
	return status;
}
