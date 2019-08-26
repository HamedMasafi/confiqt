#include "pageselectpaths.h"
#include "configmanager.h"

#include <QDir>
#include <QMessageBox>

PageSelectPaths::PageSelectPaths(ConfigManager *config, QWidget *parent)
    : WizardPageBase(config, parent)
{
    setupUi(this);

}

bool PageSelectPaths::validatePage()
{
    QDir d;
    if (!d.exists(lineEditBuildPath->text())) {
        bool ok = d.mkpath(lineEditBuildPath->text());
        if (!ok)
            QMessageBox::warning(this, title(),
                                 "Unable to create build path: " +lineEditBuildPath->text());
        return false;
    }

    if (!d.exists(lineEditSourcePath->text())) {
        QMessageBox::warning(this, title(),
                             "The source path does not exists");
        return false;
    }

    if (!d.exists(lineEditInstallPath->text())) {
        bool ok = d.mkpath(lineEditInstallPath->text());
        if (!ok)
            QMessageBox::warning(this, title(),
                                 "Unable to create install path: " +lineEditInstallPath->text());
        return false;
    }

    if (lineEditSourcePath->text() == lineEditInstallPath->text()) {
        QMessageBox::warning(this, title(),
                             "Can not build Qt in the source dir");
        return false;
    }



    _config->setSourcePath(lineEditSourcePath->text());
    _config->setBuildPath(lineEditBuildPath->text());
    _config->setInstallPath(lineEditInstallPath->text());

    return true;
}
