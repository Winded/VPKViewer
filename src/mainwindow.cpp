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
    mFileService(0),
	mProcessor(new Processor(&mConfigManager, this)),
	mCurrentFile(QString()),
	mCurrentPath(QString())
{
	setAcceptDrops(true);

	ui->setupUi(this);

    ui->actionOpen->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
	ui->actionQuit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));

	ui->consoleDockWidget->setVisible(false);

    mModel = new FileObjectModel();
    mDirModel = new QSortFilterProxyModel();
    mDirModel->setSourceModel(mModel);
    mDirModel->setFilterKeyColumn(3);
    mDirModel->setFilterFixedString("true");

    ui->dirTree->setModel(mDirModel);
    for(int i = 1; i < 4; i++) {
        ui->dirTree->hideColumn(i);
    }
    ui->fileList->setModel(mModel);

	//Default config
	ConfigManager::Config defaultConfig("Test", "D:/Games/Steam/steamapps/common/Team Fortress 2/bin/vpk.exe");
	mConfigManager.addConfig(defaultConfig);
	mConfigManager.selectConfig(defaultConfig.mName);

    connect(ui->fileList, SIGNAL(activated(QModelIndex)), this, SLOT(fileActivated(QModelIndex)));

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFileDialog()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    ui->fileList->setDragEnabled(true);
    ui->fileList->setAcceptDrops(true);

	connect(mProcessor, SIGNAL(programOutput(QString)), this, SLOT(consoleOutput(QString)));
    connect(mProcessor, SIGNAL(errorHappened(QString)), this, SLOT(error(QString)));
}

MainWindow::~MainWindow()
{
    delete mDirModel;
    delete mModel;
	delete mFileService;
	delete mProcessor;
    delete ui;
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

void MainWindow::fileActivated(QModelIndex index) {
    FileObject *obj = (FileObject*)index.internalPointer();
    if(obj->isDirectory()) {
        openDir(index);
    }
}

void MainWindow::openVPK(QString path) {
    QList<FileObject*> fileList;
    mProcessor->getFiles(path, &fileList);

    FileService *service = new FileService(&fileList);
    mModel->setFileService(service);
    if(mFileService) {
        delete mFileService;
    }
    mFileService = service;

    QModelIndex rIdx = mModel->index(0, 0, QModelIndex());
    openDir(rIdx);

	mCurrentFile = path;
    mCurrentPath = QString();
}

void MainWindow::openDir(QModelIndex &index) {
    ui->dirTree->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
    ui->fileList->setRootIndex(index);
}
