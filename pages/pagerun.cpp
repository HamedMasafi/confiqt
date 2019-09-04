#include "pagerun.h"

#include <QProcess>

PageRun::PageRun(ConfigManager *config, QWidget *parent) :
    WizardPageBase(config, parent)
{
    setupUi(this);
}

void PageRun::initializePage()
{
    run();
}

bool PageRun::isComplete() const
{
    return false;
}

void PageRun::run()
{
    auto p = _config->createProcess();
    connect(p, &QProcess::readyReadStandardError, [this, p](){
        auto b = p->readAllStandardError();
        plainTextEditBuildOutput->appendPlainText(b);
    });
    connect(p, &QProcess::readyReadStandardOutput, [this, p](){
        auto b = p->readAllStandardOutput();
        plainTextEditBuildOutput->appendPlainText(b);
    });
    connect(p, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [this](int code, QProcess::ExitStatus status){
        if (status == QProcess::NormalExit)
            plainTextEditBuildOutput->appendPlainText("Configure successful");
        else
            plainTextEditBuildOutput->appendPlainText(QString("Configure failed with code: %1").arg(code));
    });
    p->start();
}
