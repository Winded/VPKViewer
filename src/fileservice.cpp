#include "fileservice.h"
#include <QStringList>
#include <cassert>

FileService::FileService()
{
}

QList<FileService::FileInfo> FileService::allFiles() const {
	return mFiles;
}

QList<QString> FileService::allFolders() const {
	QList<QString> folders;

	for(FileInfo info : mFiles) {
		QStringList split = info.mPath.split("/");

		QString s;
		for(int i = 0; i < split.size() - 1; i++) {
			s += i > 0 ? "/" + split[i] : split[i];
			if(!folders.contains(s)) {
				folders.append(s);
			}
		}
	}

	return folders;
}

QList<FileService::FileInfo> FileService::filesInDir(QString dir) const {
	QList<FileInfo> files;

	for(FileInfo info : mFiles) {
		QString d = FileService::getDir(info.mPath);
		if(d == dir) {
			files.append(info);
		}
	}

	return files;
}

QList<QString> FileService::foldersInDir(QString dir) const {
	QList<QString> folders;

	for(FileInfo info : mFiles) {
		QStringList split = info.mPath.split("/");

		QString s;
		for(int i = 0; i < split.size() - 1; i++) {
			QString curStr = split[i];
			QString oldPath = s;
			s += i > 0 ? "/" + curStr : curStr;

			if(oldPath == dir && !folders.contains(s)) {
				folders.append(s);
			}
		}
	}

	return folders;
}

bool FileService::isFile(QString path) const {
	for(FileInfo info : mFiles) {
		if(info.mPath == path) {
			return true;
		}
	}
	return false;
}

void FileService::setFiles(QList<FileService::FileInfo> files) {
	mFiles = files;
}

QString FileService::getDir(QString path) {
	QStringList split = path.split("/");
	if(split.size() < 2) {
		return QString();
	}

	QString s;
	for(int i = 0; i < split.size() - 1; i++) {
		s += i > 0 ? "/" + split[i] : split[i];
	}
	return s;
}

QString FileService::getName(QString path) {
	QStringList split = path.split("/");
	if(split.size() == 0) {
		return QString();
	}

	return split[split.size() - 1];
}
