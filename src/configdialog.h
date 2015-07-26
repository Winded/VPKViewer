#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include "configservice.h"
#include "configmodel.h"
#include "datawidgetbinder/datawidgetbinder.h"

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit ConfigDialog(ConfigService *service, QWidget *parent = 0);
        ~ConfigDialog();

    public slots:
        void accept();
        void done(int result);

    private slots:
        void addConfig();

        void configSelected(QModelIndex index);

        void contextMenuRequested(QPoint point);

    private:
        void deleteConfig(ConfigObject *cfg);

        Ui::ConfigDialog *ui;

        ConfigService *mService;
        ConfigModel *mModel;

        DataWidgetBinder *mBinder;
};

#endif // CONFIGDIALOG_H
