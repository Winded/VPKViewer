#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "configmanager.h"
#include "processor.h"
#include "fileservice.h"
#include <QMainWindow>
#include <QTreeWidget>
#include <QListWidget>

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

		void listItemDoubleClicked(QListWidgetItem *item);

		void dragEnterEvent(QDragEnterEvent *event);
		void dragMoveEvent(QDragMoveEvent *event);
		void dropEvent(QDropEvent *event);

	private:
        void openVPK(QString path);
		void refreshFolderList();
		void refreshFileList();

		Ui::MainWindow *ui;

        ConfigManager mConfigManager;
		FileService *mFileService;
		Processor *mProcessor;

		QString mCurrentFile;
		QString mCurrentPath;

		QMap<QString, QTreeWidgetItem*> mFolders;
		QMap<QString, QListWidgetItem*> mListItems;
};

#endif // MAINWINDOW_H
