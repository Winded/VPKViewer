#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "configservice.h"
#include "processor.h"
#include "fileservice.h"
#include "fileobjectmodel.h"
#include "objectpropertybinder/objectpropertybinder.h"
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
        explicit MainWindow(ConfigService *configService, QWidget *parent = 0);
		~MainWindow();

    private slots:
        void openFileDialog();

		void openConfigDialog();
		void selectedConfigChanged(int index);

		void consoleOutput(QString output);
        void error(QString output);

        void goUp();

		void dragEnterEvent(QDragEnterEvent *event);
		void dragMoveEvent(QDragMoveEvent *event);
		void dropEvent(QDropEvent *event);

		void closeEvent(QCloseEvent *event);

        void fileActivated(QModelIndex index);
		void folderSelected(QModelIndex index);

	private:
        void openVPK(QString path);

        void openDir(QModelIndex index);

		Ui::MainWindow *ui;

        ConfigService *mConfigService;
		FileService *mFileService;
		Processor *mProcessor;

        QSortFilterProxyModel *mDirModel;
		FileObjectModel *mModel;

		QString mCurrentFile;
        QString mCurrentPath;
};

#endif // MAINWINDOW_H
