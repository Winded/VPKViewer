#include "configdialog.h"
#include "ui_configdialog.h"
#include <QCloseEvent>
#include <QMenu>
#include <QMessageBox>

ConfigDialog::ConfigDialog(ConfigService *service, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog),
    mService(service)
{
    ui->setupUi(this);

    mModel = new ConfigModel(service);
    ui->listView->setModel(mModel);

    mBinder = new DataWidgetBinder();
    mBinder->setModel(mModel);
    mBinder->addBinding(ui->name, "text", 0);
    mBinder->addBinding(ui->exePath, "text", 1);

    connect(ui->listView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(configSelected(QModelIndex)));
    connect(ui->listView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequested(QPoint)));

    connect(ui->newConfigButton, SIGNAL(pressed()), this, SLOT(addConfig()));
}

ConfigDialog::~ConfigDialog()
{
    delete mBinder;
    delete mModel;
    delete ui;
}

void ConfigDialog::accept() {
    if(mService->configs().empty()) {
        QMessageBox::critical(this, "No configurations!", "You must have at least one configuration");
        return;
    }
    if(mService->selectedIndex() == -1) {
        mService->setSelectedIndex(0);
    }
    mService->save();
    QDialog::accept();
}

void ConfigDialog::done(int result) {
    mService->load();
    QDialog::done(result);
}

void ConfigDialog::addConfig() {
    ConfigObject *obj = new ConfigObject("New configuration", "C:\\path\\to\\vpk.exe");
    mService->addConfig(obj);
    int i = mService->configs().indexOf(obj);
    QModelIndex idx = mModel->index(i, 0);
    ui->listView->selectionModel()->select(idx, QItemSelectionModel::ClearAndSelect);
    mBinder->setCurrentIndex(idx);
}

void ConfigDialog::configSelected(QModelIndex index) {
    mBinder->setCurrentIndex(index);
}

void ConfigDialog::contextMenuRequested(QPoint point) {
    QPoint globalPos = ui->listView->mapToGlobal(point);
    QModelIndex index = ui->listView->indexAt(point);
    if(index.isValid()) {
        ConfigObject *obj = (ConfigObject*)index.internalPointer();
        QMenu menu(this);
        menu.move(globalPos);
        QAction *del = menu.addAction("Delete");

        QAction *selection = menu.exec();
        if(selection == del) {
            deleteConfig(obj);
        }
    }
}

void ConfigDialog::deleteConfig(ConfigObject *cfg) {
    mService->removeConfig(cfg);
}
