#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QProxyStyle>
#include <QShortcut>
#include <QMimeData>
#include <cassert>
#include "configdialog.h"

MainWindow::MainWindow(ConfigService *configService, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
    mConfigService(configService),
    mFileService(0),
    mProcessor(new Processor(configService, this)),
	mCurrentFile(QString()),
	mCurrentPath(QString())
{
	setAcceptDrops(true);

	ui->setupUi(this);

    ui->actionOpen->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
	ui->actionQuit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));

	ui->consoleDockWidget->setVisible(false);

    QProxyStyle s;
    ui->upButton->setIcon(s.standardIcon(QStyle::SP_ArrowUp));

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

    connect(ui->upButton, SIGNAL(pressed()), this, SLOT(goUp()));

	for(ConfigObject *obj : mConfigService->configs()) {
		ui->configComboBox->addItem(obj->name());
	}
	ui->configComboBox->setCurrentIndex(mConfigService->selectedIndex());
	connect(ui->editConfigsPushButton, SIGNAL(pressed()), this, SLOT(openConfigDialog()));
	connect(ui->configComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectedConfigChanged(int)));

    connect(ui->fileList, SIGNAL(activated(QModelIndex)), this, SLOT(fileActivated(QModelIndex)));
	connect(ui->dirTree->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(folderSelected(QModelIndex)));

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
    if(!mConfigService->selectedConfig()) {
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

void MainWindow::openConfigDialog() {
	ConfigDialog dialog(mConfigService, this);
	dialog.exec();
	ui->configComboBox->clear();
	for(ConfigObject *obj : mConfigService->configs()) {
		ui->configComboBox->addItem(obj->name());
	}
	ui->configComboBox->setCurrentIndex(mConfigService->selectedIndex());
}

void MainWindow::selectedConfigChanged(int index) {
	mConfigService->setSelectedIndex(index);
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

void MainWindow::goUp() {
    if(!mFileService) {
        return;
    }

    QModelIndex curIdx = ui->fileList->rootIndex();
    QModelIndex idx = curIdx.parent();
    if(idx.isValid()) {
        openDir(idx);
    }
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

void MainWindow::closeEvent(QCloseEvent *event) {
	mConfigService->save();
}

void MainWindow::fileActivated(QModelIndex index) {
    FileObject *obj = (FileObject*)index.internalPointer();
	if(obj->isDirectory()) {
        openDir(index);
	}
}

void MainWindow::folderSelected(QModelIndex index) {
    QModelIndex idx = mDirModel->mapToSource(index);
    openDir(idx);
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

void MainWindow::openDir(QModelIndex index) {
    QModelIndex dirIdx = mDirModel->mapFromSource(index);
    ui->dirTree->selectionModel()->setCurrentIndex(dirIdx, QItemSelectionModel::ClearAndSelect);
    ui->fileList->setRootIndex(index);
}
