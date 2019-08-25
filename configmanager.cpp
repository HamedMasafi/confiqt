#include "configmanager.h"
#include "jsonconfig.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QProcess>
#include <QRegularExpression>

QList<Option> ConfigManager::options() const
{
    return _options;
}

QList<Feature> ConfigManager::features() const
{
    return _features;
}

QStringList ConfigManager::submodules() const
{
    return _submodules;
}

QStringList ConfigManager::selectedModules() const
{
    return _selectedModules;
}

void ConfigManager::setSelectedModules(const QStringList &selectedModules)
{
    _selectedModules = selectedModules;
}

QStringList ConfigManager::platforms() const
{
    return _platforms;
}

void ConfigManager::setFeatureState(const QString &featureName, const Qt::CheckState &state)
{
    _featuresStates.insert(featureName, state);
}

void ConfigManager::clearOptionsStates()
{
    _optionsStates.clear();
}

void ConfigManager::setOptionsState(const QString &optionName, const QString &state)
{
    _optionsStates.insert(optionName, state);
}

bool ConfigManager::confirmLicense() const
{
    return m_confirmLicense;
}

bool ConfigManager::useCommercial() const
{
    return m_useCommercial;
}

QString ConfigManager::buildPath() const
{
    return m_buildPath;
}

QString ConfigManager::installPath() const
{
    return m_installPath;
}

QStringList ConfigManager::createCommand() const
{
    QStringList args;

    args << "-prefix"
         << m_installPath;

    if (m_useCommercial)
        args << "-commercial";
    else
        args << "-opensource";

    if (m_confirmLicense)
        args << "-confirm-license";

    auto modules = _submodules;
    foreach (QString selectedModule, _selectedModules)
        modules.removeAll(selectedModule);

    foreach (QString module, modules)
        args << "-skip"
             << module;

    auto itFeatures = _featuresStates.begin();
    while (itFeatures != _featuresStates.end()) {
        switch (itFeatures.value()) {
        case Qt::Checked:
            args << "-feature-" + itFeatures.key();
            break;

        case Qt::Unchecked:
            args << "-no-feature-" + itFeatures.key();
            break;

        case Qt::PartiallyChecked:
            break;
        }
        ++itFeatures;
    }

    auto itOptions = _optionsStates.begin();
    while (itOptions != _optionsStates.end()) {
        if (itOptions.value() != QString()) {
            if (itOptions.value() == "true")
                args << "-" + itOptions.key();
            else if (itOptions.value() == "false")
                args << "-no-" + itOptions.key();
            else
                args << "-" + itOptions.value() + "-" + itOptions.key();
        }
        ++itOptions;
    }

    return args;
}

QProcess *ConfigManager::createProcess() const
{
    auto p = new QProcess;
    p->setProgram(m_sourcePath + "configure");
    p->setArguments(createCommand());
    p->setWorkingDirectory(m_buildPath);
    return p;
}

void ConfigManager::clearFeatureStates()
{
    _featuresStates.clear();
}

QByteArray ConfigManager::readFile(const QString &path)
{
    QFile jsonFile(path);
    if (!jsonFile.open(QIODevice::Text | QIODevice::ReadOnly))
        return QByteArray();

    QByteArray buffer = jsonFile.readAll();
    jsonFile.close();

    return buffer;
}

QJsonObject ConfigManager::readConfig(const QString &path)
{
    JsonConfig c(path);
    if (!c.isValid()) {
//        qDebug() << path << "is not valid;";
        return QJsonObject();
    }
    auto o = c.subConfigs();
    if (o.size())
        foreach (QString sub, o) {
            readConfig(path + sub);
        }


    _options.append(c.options());
    _features.append(c.features());
    return QJsonObject();
}

ConfigManager::ConfigManager()
{

}

QString ConfigManager::sourcePath() const
{
    return m_sourcePath;
}

void ConfigManager::process()
{
    foreach (QString module, _selectedModules)
        if (_submodules.contains(module))
            QJsonObject o = readConfig(m_sourcePath + module + "/");
}

void ConfigManager::setSourcePath(QString sourcePath)
{
    if (m_sourcePath == sourcePath)
        return;

    if (sourcePath.endsWith("/"))
        m_sourcePath = sourcePath;
    else
        m_sourcePath = sourcePath + "/";

    QByteArray b = readFile(m_sourcePath + ".gitmodules");
    QRegularExpression r("\\[submodule \"(\\w+)\"\\]");
    _submodules.clear();
    QRegularExpressionMatchIterator it = r.globalMatch(b);
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        QString proFilePath = QString("%1/%2/%2.pro")
                .arg(m_sourcePath, match.captured(1));
        if (QFile::exists(proFilePath))
            _submodules.append(match.captured(1));
    }

    QDir mkspec(m_sourcePath + "qtbase/mkspecs/");
    _platforms = mkspec.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    emit sourcePathChanged(m_sourcePath);
}

void ConfigManager::setBuildPath(QString buildPath)
{
    if (m_buildPath == buildPath)
        return;

    m_buildPath = buildPath;
    emit buildPathChanged(m_buildPath);
}

void ConfigManager::setInstallPath(QString installPath)
{
    if (m_installPath == installPath)
        return;

    m_installPath = installPath;
    emit installPathChanged(m_installPath);
}

void ConfigManager::setConfirmLicense(bool confirmLicense)
{
    if (m_confirmLicense == confirmLicense)
        return;

    m_confirmLicense = confirmLicense;
    emit confirmLicenseChanged(m_confirmLicense);
}

void ConfigManager::setUseCommercial(bool useCommercial)
{
    if (m_useCommercial == useCommercial)
        return;

    m_useCommercial = useCommercial;
    emit useCommercialChanged(m_useCommercial);
}
