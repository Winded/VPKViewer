#include "mainwindow.h"
#include "configdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

    ConfigService cfgService;
    cfgService.load();
    if(cfgService.configs().empty()) {
        ConfigDialog dialog(&cfgService);
        dialog.exec();
        if(cfgService.configs().empty()) {
            return 0;
        }
    }

    MainWindow w(&cfgService);
	w.show();

	return a.exec();
}
