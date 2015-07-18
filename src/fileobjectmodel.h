#ifndef FILEOBJECTMODEL_H
#define FILEOBJECTMODEL_H

#include <QAbstractItemModel>
#include "fileservice.h"

class FileObjectModel : public QAbstractItemModel
{
        Q_OBJECT
    public:
        FileObjectModel(QObject *parent = 0);

        int rowCount(const QModelIndex &parent) const;
        int columnCount(const QModelIndex &parent) const;

        QModelIndex index(int row, int column, const QModelIndex &parent) const;
        QModelIndex parent(const QModelIndex &child) const;

        QVariant data(const QModelIndex &index, int role) const;
        bool setData(const QModelIndex &index, const QVariant &value, int role);

        FileService *fileService() const;
        void setFileService(FileService *fileService);

        bool isShowingDirectories() const;
        void setIsShowingDirectories(bool showing);

        bool isShowingFiles() const;
        void setIsShowingFiles(bool showing);

    private:
        QList<FileObject *> getChildren(FileObject *obj) const;

        FileService *mFileService;
        bool mShowDirectories;
        bool mShowFiles;
};

#endif // FILEOBJECTMODEL_H
