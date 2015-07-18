#include "fileobjectmodel.h"

FileObjectModel::FileObjectModel(QObject *parent) :
    QAbstractItemModel(parent),
    mFileService(0),
    mShowDirectories(true),
    mShowFiles(true)
{
}

int FileObjectModel::rowCount(const QModelIndex &parent) const {
    if(!mFileService) {
        return 0;
    }

    if(!parent.isValid()) {
        return 1;
    }

    FileObject *obj = (FileObject*)parent.internalPointer();
    QList<FileObject*> children = getChildren(obj);
    return children.count();
}

int FileObjectModel::columnCount(const QModelIndex &parent) const {
    return 4;
}

QModelIndex FileObjectModel::index(int row, int column, const QModelIndex &parent) const {
    if(!mFileService) {
        return QModelIndex();
    }

    if(!parent.isValid()) {
        return createIndex(row, column, mFileService->root());
    }

    FileObject *pObj = (FileObject*)parent.internalPointer();
    QList<FileObject*> children = getChildren(pObj);
    if(row < 0 || row >= children.count()) {
        return QModelIndex();
    }
    FileObject *obj = children.at(row);

    return createIndex(row, column, obj);
}

QModelIndex FileObjectModel::parent(const QModelIndex &child) const {
    if(!mFileService) {
        return QModelIndex();
    }

    FileObject *obj = (FileObject*)child.internalPointer();
    FileObject *pObj = obj->parent();
    if(!pObj) {
        return QModelIndex();
    }

    if(!pObj->parent()) {
        return createIndex(0, 0, pObj);
    }
    int i = pObj->parent()->children().indexOf(pObj);
    return createIndex(i, 0, pObj);
}

QVariant FileObjectModel::data(const QModelIndex &index, int role) const {
    if(!mFileService) {
        return QVariant();
    }

    if(!index.isValid()) {
        return QVariant();
    }

    if(role != Qt::DisplayRole && role != Qt::EditRole) {
        return QVariant();
    }

    FileObject *obj = (FileObject*)index.internalPointer();
    switch(index.column()) {
    case 0:
        return obj->name();
    case 1:
        return obj->path();
    case 2:
        return obj->size();
    case 3:
        return obj->isDirectory();
    }
    return QVariant();
}

bool FileObjectModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    return false;
}

FileService *FileObjectModel::fileService() const {
    return mFileService;
}

void FileObjectModel::setFileService(FileService *fileService) {
    beginResetModel();
    mFileService = fileService;
    endResetModel();
}

bool FileObjectModel::isShowingDirectories() const {
    return mShowDirectories;
}

void FileObjectModel::setIsShowingDirectories(bool showing) {
    mShowDirectories = showing;
}

bool FileObjectModel::isShowingFiles() const {
    return mShowFiles;
}

void FileObjectModel::setIsShowingFiles(bool showing) {
    mShowFiles = showing;
}

QList<FileObject *> FileObjectModel::getChildren(FileObject *obj) const {
    QList<FileObject*> children = obj->children();
    QList<FileObject*> shownChildren;
    if(mShowDirectories) {
        for(FileObject *fo : children) {
            if(fo->isDirectory()) {
                shownChildren.append(fo);
            }
        }
    }
    if(mShowFiles) {
        for(FileObject *fo : children) {
            if(!fo->isDirectory()) {
                shownChildren.append(fo);
            }
        }
    }
    return shownChildren;
}

