#ifndef VPKOBJECT_H
#define VPKOBJECT_H

#include <QString>
#include <QList>

/// \brief Represents either a file or a folder inside a VPK package.
class FileService
{
	public:
		struct FileInfo {
				QString mPath;
				int mSize;
		};

		enum Type {
			File,
			Folder
		};

		FileService();

		QList<FileInfo> allFiles() const;
		QList<QString> allFolders() const;
		QList<FileInfo> filesInDir(QString dir) const;
		QList<QString> foldersInDir(QString dir) const;

		bool isFile(QString path) const;

		void setFiles(QList<FileInfo> files);

		/// \brief Get directory of the given path
		static QString getDir(QString path);
		/// \brief Get the name of the file or directory from the given path
		static QString getName(QString path);

	private:

		QList<FileInfo> mFiles;
};

#endif // VPKOBJECT_H
