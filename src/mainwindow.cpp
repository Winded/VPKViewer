#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

    ui->actionOpen->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    ui->actionQuit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFileDialog()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(quit()));
}

MainWindow::~MainWindow()
{
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

void MainWindow::quit() {
    close();
}

void MainWindow::openVPK(QString path) {

}
