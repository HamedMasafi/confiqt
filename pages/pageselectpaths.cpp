#include "pageselectpaths.h"
#include "configmanager.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QStyle>

PageSelectPaths::PageSelectPaths(ConfigManager *config, QWidget *parent)
    : WizardPageBase(config, parent)
{
    setupUi(this);
    labelDefaultMkspec->setText(_config->defaultPlatform());

    QSettings set(QSettings::UserScope);
    lineEditSourcePath->setText(set.value("sourcePath", "").toString());
    lineEditBuildPath->setText(set.value("buildPath", "").toString());
    lineEditInstallPath->setText(set.value("installPath", "").toString());
}

bool PageSelectPaths::validatePage()
{
    QDir d;
    if (!d.exists(lineEditBuildPath->text())) {
        bool ok = d.mkpath(lineEditBuildPath->text());
        if (!ok) {
            QMessageBox::warning(this,
                                 title(),
                                 "Unable to create build path: " + lineEditBuildPath->text());
            return false;
        }
    }

    if (!d.exists(lineEditSourcePath->text())) {
        QMessageBox::warning(this, title(), "The source path does not exists");
        return false;
    }

    if (!d.exists(lineEditInstallPath->text())) {
        bool ok = d.mkpath(lineEditInstallPath->text());
        if (!ok) {
            QMessageBox::warning(this,
                                 title(),
                                 "Unable to create install path: " + lineEditInstallPath->text());
            return false;
        }
    }

    if (lineEditSourcePath->text() == lineEditInstallPath->text()) {
        QMessageBox::warning(this, title(), "Can not build Qt in the source dir");
        return false;
    }

    _config->setSourcePath(lineEditSourcePath->text());
    _config->setBuildPath(lineEditBuildPath->text());
    _config->setInstallPath(lineEditInstallPath->text());

    QSettings set(QSettings::UserScope);
    set.setValue("sourcePath", lineEditSourcePath->text());
    set.setValue("buildPath", lineEditBuildPath->text());
    set.setValue("installPath", lineEditInstallPath->text());
    set.sync();

    if (QFile::exists(lineEditBuildPath->text() + "/config.opt")) {
        QMessageBox msg;
        msg.setText(R"(The selected build path contain files from previus build <br />
                    Choose desired action:
                    <ul>
                        <li>Import settings: import settings from previous build</li>
                        <li>Delete: Delete previous build's history</li>
                        <li>Cancel: Back to choose another path</li>
                    </ul>)");
        msg.setIcon(QMessageBox::Question);
        msg.setDefaultButton(QMessageBox::Yes);
        msg.addButton("Import settings", QMessageBox::YesRole)
            ->setIcon(qApp->style()->standardIcon(QStyle::SP_FileIcon));
        msg.addButton("Delete", QMessageBox::NoRole)
            ->setIcon(qApp->style()->standardIcon(QStyle::SP_TrashIcon));
        msg.setStandardButtons(QMessageBox::Cancel /* | QMessageBox::Yes | QMessageBox::No*/);
        auto res = msg.exec();
        qDebug() << res;
        switch (res) {
        case 0: //import
            _config->importPreviousBuild();
            break;
        case 1: //delete
            _config->deleteSettings();
            break;
        case QMessageBox::Cancel:
            return false;
        default:
            return false;
        }
    }

    return true;
}

void PageSelectPaths::on_pushButtonSelectSourcePath_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, QString(), lineEditSourcePath->text());
    if (path != QString())
        lineEditSourcePath->setText(path);
}

void PageSelectPaths::on_pushButtonSelectBuildPath_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, QString(), lineEditBuildPath->text());
    if (path != QString())
        lineEditBuildPath->setText(path);
}

void PageSelectPaths::on_pushButtonSelectInstallPath_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, QString(), lineEditInstallPath->text());
    if (path != QString())
        lineEditInstallPath->setText(path);
}

void PageSelectPaths::on_pushButtonImport_clicked()
{
    auto fileName = QFileDialog::getOpenFileName(this, "Open file");
    if (!fileName.isEmpty() && _config->importJson(fileName)) {
        lineEditBuildPath->setText(_config->buildPath());
        lineEditInstallPath->setText(_config->installPath());
        lineEditSourcePath->setText(_config->sourcePath());
    }
}
