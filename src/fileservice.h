#ifndef FILESERVICE_H
#define FILESERVICE_H

#include <QString>
#include <QList>
#include <QObject>
#include <QHash>

class FileObject
{
    public:
        FileObject(QString path, int size, bool isDirectory = false);
        ~FileObject();

        QString path() const;
        QString name() const;

        int size() const;

        bool isDirectory() const;

        FileObject *parent() const;
        void setParent(FileObject *parent);

        QList<FileObject *> children() const;

    private:
        QString mPath;
        int mSize;
        bool mIsDirectory;

        FileObject *mParent;
        QList<FileObject*> mChildren;
};

/// \brief Represents either a file or a folder inside a VPK package.
class FileService : public QObject
{
        Q_OBJECT
    public:
        explicit FileService(QList<FileObject*> *files, QObject *parent = 0);
        ~FileService();

        FileObject *root() const;

	private:

        FileObject *mRoot;
};

#endif // FILESERVICE_H
