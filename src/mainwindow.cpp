#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QProxyStyle>
#include <QShortcut>
#include <QMimeData>
#include <cassert>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	mFileService(new FileService()),
	mProcessor(new Processor(&mConfigManager, this)),
	mCurrentFile(QString()),
	mCurrentPath(QString())
{
	setAcceptDrops(true);

	ui->setupUi(this);

    ui->actionOpen->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
	ui->actionQuit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));

	ui->consoleDockWidget->setVisible(false);

	//Default config
	ConfigManager::Config defaultConfig("Test", "D:/Games/Steam/steamapps/common/Team Fortress 2/bin/vpk.exe");
	mConfigManager.addConfig(defaultConfig);
	mConfigManager.selectConfig(defaultConfig.mName);

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFileDialog()));

	ui->fileListWidget->setDragEnabled(true);
	ui->fileListWidget->setAcceptDrops(true);
	connect(ui->fileListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(listItemDoubleClicked(QListWidgetItem*)));

	connect(mProcessor, SIGNAL(programOutput(QString)), this, SLOT(consoleOutput(QString)));
	connect(mProcessor, SIGNAL(errorHappened(QString)), this, SLOT(error(QString)));

	connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete ui;
	delete mFileService;
	delete mProcessor;
}

void MainWindow::openFileDialog() {
    if(mConfigManager.exePath().isEmpty()) {
        QMessageBox::critical(this, "No vpk.exe path set", "Invalid configuration; no vpk.exe path present. "
                              "please make sure you have created a valid configuration.");
        return;
    }

    QString filePath = QFileDialog::getOpenFileName(this, "Open VPK package", QApplication::applicationDirPath(), "VPK file (*.vpk)");
    if(filePath.isEmpty()) {
        return;
    }

	openVPK(filePath);
}

void MainWindow::consoleOutput(QString output) {
	ui->consoleText->appendPlainText(output);
}

void MainWindow::error(QString output) {
	QPalette p = ui->consoleText->palette();
	p.setColor(QPalette::Text, Qt::red);
	ui->consoleText->setPalette(p);
	ui->consoleText->appendPlainText(output);
	ui->consoleDockWidget->show();
}

void MainWindow::listItemDoubleClicked(QListWidgetItem *item) {
	QString path = mListItems.key(item);
	if(!path.isNull() && mFileService->isFile(path)) {
		return;
	}

	if(path.isNull()) {
		// This means we are dealing with the .. list item
		mCurrentPath = FileService::getDir(mCurrentPath);
	}
	else {
		mCurrentPath = path;
	}

	refreshFileList();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
	event->acceptProposedAction();
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event) {
	event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event) {
	const QMimeData *mData = event->mimeData();
	if(!mData->hasUrls()) {
		return;
	}

	QString fileName = mData->urls().last().toLocalFile();
	if(QFileInfo(fileName).suffix() != "vpk") {
		return;
	}

	openVPK(fileName);
}

void MainWindow::openVPK(QString path) {
	QList<FileService::FileInfo> files = mProcessor->getFiles(path);
	mFileService->setFiles(files);

	mCurrentFile = path;
	mCurrentPath = QString();
	refreshFolderList();
	refreshFileList();
}

void MainWindow::refreshFolderList() {
	ui->folderTreeWidget->clear();
	ui->folderTreeWidget->setColumnCount(1);
	mFolders.clear();

	QProxyStyle s;
	QTreeWidgetItem *root = new QTreeWidgetItem;
	root->setText(0, "root");
	root->setIcon(0, s.standardIcon(QStyle::SP_DirIcon));
	ui->folderTreeWidget->addTopLevelItem(root);

	for(QString folder : mFileService->allFolders()) {
		QTreeWidgetItem *parent = root;
		QString dir = FileService::getDir(folder);
		if(!dir.isNull() && mFolders.value(dir)) {
			parent = mFolders.value(dir);
		}
		QTreeWidgetItem *item = new QTreeWidgetItem;
		item->setText(0, FileService::getName(folder));
		item->setIcon(0, s.standardIcon(QStyle::SP_DirIcon));
		parent->addChild(item);
		mFolders.insert(folder, item);
	}
}

void MainWindow::refreshFileList() {
	ui->fileListWidget->clear();
	mListItems.clear();
	QProxyStyle s;

	if(!mCurrentPath.isNull()) {
		QListWidgetItem *item = new QListWidgetItem;
		item->setText("..");
		item->setIcon(s.standardIcon(QStyle::SP_DirIcon));
		ui->fileListWidget->addItem(item);
	}

	QList<QString> folders = mFileService->foldersInDir(mCurrentPath);
	QList<FileService::FileInfo> files = mFileService->filesInDir(mCurrentPath);

	for(QString folder : folders) {
		QString name = FileService::getName(folder);
		QListWidgetItem *item = new QListWidgetItem;
		item->setText(name);
		item->setIcon(s.standardIcon(QStyle::SP_DirIcon));
		ui->fileListWidget->addItem(item);
		mListItems.insert(folder, item);
	}

	for(FileService::FileInfo info : files) {
		QString name = FileService::getName(info.mPath);
		QListWidgetItem *item = new QListWidgetItem;
		item->setText(name);
		item->setIcon(s.standardIcon(QStyle::SP_FileIcon));
		ui->fileListWidget->addItem(item);
		mListItems.insert(info.mPath, item);
	}
}
