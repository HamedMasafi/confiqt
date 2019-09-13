#include <QDebug>
#include <QMessageBox>

#include "pages/pagecrosscompile.h"
#include "pages/pagefeatures.h"
#include "pages/pageoptions.h"
#include "pages/pagelicense.h"
#include "pages/pagemodules.h"
#include "pages/pageselectbuildmethod.h"
#include "pages/pageselectpaths.h"
#include "pages/pageselectplatform.h"
#include "pages/pagewelcome.h"
#include "pages/pagecommon.h"
#include "pages/pagenomake.h"
#include "pages/pagerun.h"
#include "pages/pagefinish.h"
#include "ui/qtconfigwizard.h"
#include "ui/saveconfirmdialog.h"



QtConfigWizard::QtConfigWizard(QWidget *parent) :
    QWizard(parent)
{
    setupUi(this);

    _config = new ConfigManager;

#define x(name) \
        _page##name = new Page##name(_config, this); \
        _index##name = addPage(_page##name);

    FOREACH_PAGES(x)
#undef x
            _simpleIds << _indexWelcome
                       << _indexSelectPaths
                       << _indexLicense
                       << _indexCommon
                       << _indexFeatures
                       << _indexOptions
                       << _indexSelectBuildMethod
                       << _indexRun
                       << _indexFinish;
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

void QtConfigWizard::closeEvent(QCloseEvent *event)
{
    if (_config->isSaveNeeded()) {
        SaveConfirmDialog d(_config, this);
        if (d.exec() == QDialog::Rejected)
            event->ignore();
    }
}

void QtConfigWizard::on_QtConfigWizard_rejected()
{
    if (_config->isSaveNeeded()) {
        SaveConfirmDialog d(_config, this);
        d.exec();
    }
}
