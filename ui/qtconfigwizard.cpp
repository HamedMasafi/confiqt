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
                       << _indexSelectBuildMethod
                       << _indexRun
                       << _indexFinish;
/* TODO: enable this later
    setOption(QWizard::HaveCustomButton1);
    setButtonText(QWizard::CustomButton1, "About...");

    setButtonLayout(QList<QWizard::WizardButton>()
                    << QWizard::CustomButton1
                    << QWizard::Stretch
                    << QWizard::BackButton
                    << QWizard::NextButton
                    << QWizard::CancelButton);

    connect(this, &QWizard::customButtonClicked, [this](int which){
        if (which == QWizard::CustomButton1)
            QMessageBox::information(this, tr("About"),
                                     tr("Some description goes here"));
    });
*/
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
