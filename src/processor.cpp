#include "processor.h"
#include <cassert>

namespace {
    const QString fileListRegex = "^([/a-z0-9]+) crc=0x\\d+ metadatasz=\\d+ fnumber=\\d+ ofs=0x\\d+ sz=(\\d+)";
}

Processor::Processor(ConfigManager *cfgManager, QObject *parent) :
	QObject(parent),
	mConfigManager(cfgManager)
{
}

QString Processor::exePath() const {
	return mConfigManager->exePath();
}

QList<Processor::FileInfo> Processor::getFiles(QString vpk) {
    QList<Processor::FileInfo> list;
	QString cmd = "L " + vpk;
	QString output, error;
	QProcess::ExitStatus status = runCommand(cmd, output, error);
	if(status == QProcess::CrashExit || !error.isEmpty()) {
		return list;
	}

	QStringList sList = output.split("\n");
    for(QString s : sList) {
        QRegExp r(fileListRegex);
        if(r.indexIn(s) == -1) {
            continue;
        }

        QString sPath = r.cap(1);
        QString sSize = r.cap(2);
        bool ok;
        int size = sSize.toInt(&ok);
        assert(ok);

        FileInfo info;
        info.mPath = sPath;
        info.mSize = size;
        list.append(info);
	}
    return list;
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
