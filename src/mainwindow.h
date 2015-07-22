#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "configmanager.h"
#include "processor.h"
#include "fileservice.h"
#include "fileobjectmodel.h"
#include <QMainWindow>
#include <QTreeWidget>
#include <QListWidget>
#include <QSortFilterProxyModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

    private slots:
        void openFileDialog();

		void consoleOutput(QString output);
        void error(QString output);

		void dragEnterEvent(QDragEnterEvent *event);
		void dragMoveEvent(QDragMoveEvent *event);
		void dropEvent(QDropEvent *event);

        void fileActivated(QModelIndex index);
		void folderSelected(QModelIndex index);

	private:
        void openVPK(QString path);

		void openDir(FileObject *obj);

		QModelIndex indexForObject(QAbstractItemModel *model, FileObject *obj);
		QModelIndex tryFindIndex(QAbstractItemModel *model, QModelIndex parent, FileObject *obj);

		Ui::MainWindow *ui;

        ConfigManager mConfigManager;
		FileService *mFileService;
		Processor *mProcessor;

        QSortFilterProxyModel *mDirModel;
        FileObjectModel *mModel;

		QString mCurrentFile;
        QString mCurrentPath;
};

#endif // MAINWINDOW_H
