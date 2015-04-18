#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "configmanager.h"
#include <QMainWindow>

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

        void quit();

	private:
        void openVPK(QString path);

		Ui::MainWindow *ui;

        ConfigManager mConfigManager;
};

#endif // MAINWINDOW_H
