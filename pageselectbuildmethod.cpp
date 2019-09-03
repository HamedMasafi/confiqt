#include "qtconfigwizard.h"
#include "pageselectbuildmethod.h"

PageSelectBuildMethod::PageSelectBuildMethod(ConfigManager *config, QWidget *parent)
    : WizardPageBase(config, parent)
{
    setupUi(this);
    plainTextEditParameteres->hide();
}

void PageSelectBuildMethod::on_pushButtonShowParameteres_clicked()
{
    plainTextEditParameteres->show();
    plainTextEditParameteres->setPlainText(_config->createCommand().join(" \\\n\t"));
}

int PageSelectBuildMethod::nextId() const
{
    if (radioButtonBuildNow->isChecked())
        return wizard()->_indexRun;
    return wizard()->_indexFinish;
}

bool PageSelectBuildMethod::validatePage()
{
    QFile optFile(_config->buildPath() + "config.opt");
    if (optFile.open(QIODevice::Text | QIODevice::WriteOnly)) {
        auto opts = _config->createCommand();
        foreach (QString o, opts)
            optFile.write((o + "\n").toLocal8Bit());
        optFile.close();
    }
    QFile statusFile(_config->buildPath() + "config.status");
    if (statusFile.open(QIODevice::Text | QIODevice::WriteOnly)) {
        QString redoCommand = QString("#!/bin/sh\nexec %1/configure -redo \"$@\"")
                .arg(_config->sourcePath());
        statusFile.write(redoCommand.toLatin1());
        statusFile.close();
    }
    statusFile.setPermissions(statusFile.permissions() | QFileDevice::ExeUser);
    return true;
}
