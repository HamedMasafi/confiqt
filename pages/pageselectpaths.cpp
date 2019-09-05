#include "pageselectpaths.h"
#include "configmanager.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

PageSelectPaths::PageSelectPaths(ConfigManager *config, QWidget *parent)
    : WizardPageBase(config, parent)
{
    setupUi(this);
    labelDefaultMkspec->setText(_config->defaultPlatform());
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

    if (QFile::exists(lineEditBuildPath->text() + "/.qmake.stash")) {
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
        msg.addButton("Import settings", QMessageBox::YesRole);
        msg.addButton("Delete", QMessageBox::NoRole);
        msg.setStandardButtons(QMessageBox::Cancel/* | QMessageBox::Yes | QMessageBox::No*/);
        auto res = msg.exec();
        qDebug() << res;
        switch (res) {
        case 0://import
            _config->importSettings();
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
