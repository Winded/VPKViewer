#include "processor.h"
#include <cassert>

namespace {
	const QString fileListRegex = "^([/\\._a-z0-9]+) crc=0x\\d+ metadatasz=\\d+ fnumber=\\d+ ofs=0x\\d+ sz=(\\d+)";
}

Processor::Processor(ConfigService *cfgService, QObject *parent) :
	QObject(parent),
    mConfigService(cfgService)
{
}

void Processor::getFiles(QString vpk, QList<FileObject *> *outList) {
	QString cmd = "L \"" + vpk + "\"";
	QString output, error;
	QProcess::ExitStatus status = runCommand(cmd, output, error);
	if(status == QProcess::CrashExit || !error.isEmpty()) {
        return;
	}

	QStringList sList = output.split("\n");
	for(QString s : sList) {
		QStringList ls = s.split(" ");
		if(ls.size() < 6) {
			continue;
		}

		QString sPath = ls[0];
		QString sSize = ls[5].split("=")[1];
        bool ok;
        int size = sSize.toInt(&ok);
        assert(ok);

        FileObject *obj = new FileObject(sPath, size);
        outList->append(obj);
    }
}

QProcess::ExitStatus Processor::runCommand(QString cmd, QString &out, QString &err) {
    ConfigObject *cfg = mConfigService->selectedConfig();
    assert(cfg);
    QString fullCmd = "\"" + cfg->exePath() + "\"" + " " + cmd;

	QProcess p(this);
	p.start(fullCmd, QProcess::ReadWrite);
    while(!p.waitForFinished(250)) {}

	out = QString(p.readAllStandardOutput());
	err = QString(p.readAllStandardError());
	QProcess::ExitStatus status = p.exitStatus();

	emit programOutput(out);
	if(!err.isEmpty()) {
		emit errorHappened(err);
	}

	p.close();
	return status;
}
