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
#include "pagenomake.h"
#include "qtconfigwizard.h"



QtConfigWizard::QtConfigWizard(QWidget *parent) :
    QWizard(parent)
{
    setupUi(this);

    _config = new ConfigManager;

#define x(name) _index##name = addPage(new Page##name(_config, this));
    FOREACH_PAGES(x)
#undef x
            _simpleIds << _indexWelcome
                       << _indexSelectPaths
                       << _indexLicense
                       << _indexFeatures
                       << _indexSelectBuildMethod;
}

void QtConfigWizard::on_QtConfigWizard_accepted()
{
    qDebug() << _config->createCommand();
}

int QtConfigWizard::nextId() const
{
    if (!field("simpleMode").toBool())
        return QWizard::nextId();

    if (currentId() == _simpleIds.last())
        return -1;

    return _simpleIds[_simpleIds.indexOf(currentId()) + 1];
}
