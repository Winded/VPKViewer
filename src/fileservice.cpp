#include "fileservice.h"
#include <QStringList>
#include <cassert>

FileService::FileService(QList<FileObject *> *files, QObject *parent) :
    QObject(parent)
{
    mRoot = new FileObject("root", 0, true);

    QHash<QString, FileObject*> dirs;
    for(FileObject *obj : *files) {
        QStringList pSplit = obj->path().split("/");
        if(pSplit.empty()) {
            continue;
        }
        pSplit.removeLast();
        QString sPath = "";
        FileObject *parent = mRoot;
        for(QString str : pSplit) {
            sPath += sPath == "" ? str : "/" + str;
            if(!dirs.contains(sPath)) {
                FileObject *dObj = new FileObject(sPath, 0, true);
                dObj->setParent(parent);
                parent = dObj;
                dirs[sPath] = dObj;
            }
            else {
                parent = dirs[sPath];
            }
        }
        obj->setParent(parent);
    }
}

FileService::~FileService() {
    delete mRoot;
}

FileObject *FileService::root() const {
    return mRoot;
}

FileObject::FileObject(QString path, int size, bool isDirectory, QObject *parent) :
    QObject(parent),
    mPath(path),
    mSize(size),
    mIsDirectory(isDirectory),
    mParent(0)
{
}

FileObject::~FileObject() {
    for(FileObject *child : mChildren) {
        delete child;
    }
}

QString FileObject::path() const {
    return mPath;
}

QString FileObject::name() const {
    QStringList split = mPath.split("/");
    if(split.empty()) {
        return QString();
    }
    return split.last();
}

int FileObject::size() const {
    return mSize;
}

bool FileObject::isDirectory() const {
    return mIsDirectory;
}

FileObject *FileObject::parent() const {
    return mParent;
}

void FileObject::setParent(FileObject *parent) {
    if(mParent) {
        mParent->mChildren.removeOne(this);
    }
    mParent = parent;
    mParent->mChildren.append(this);
}

QList<FileObject *> FileObject::children() const {
    return mChildren;
}
