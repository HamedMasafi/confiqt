#include <QDebug>

#include "pagecrosscompile.h"
#include "pagefeatures.h"
#include "pagelibs.h"
#include "pagelicense.h"
#include "pagemodules.h"
#include "pageselectbuildmethod.h"
#include "pageselectpaths.h"
#include "pageselectplatform.h"
#include "pagewelcome.h"
#include "qtconfigwizard.h"

QtConfigWizard::QtConfigWizard(QWidget *parent) :
    QWizard(parent)
{
    setupUi(this);

    _config = new ConfigManager;

    addPage(new PageWelcome(this));
    addPage(new PageSelectPaths(_config, this));
    addPage(new PageLicense(_config, this));
    addPage(new PageSelectPlatform(_config, this));
    addPage(new PageCrossCompile(_config, this));
    addPage(new PageModules(_config, this));
    addPage(new PageFeatures(_config, this));
    addPage(new PageLibs(_config, this));
    addPage(new PageSelectBuildMethod(_config, this));
}

void QtConfigWizard::on_QtConfigWizard_accepted()
{
    qDebug() << _config->createCommand();

}
