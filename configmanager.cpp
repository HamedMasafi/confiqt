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
    process();
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

    foreach (QString nm, _nomake)
        args << "-nomake"
             << nm;

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

QStringList ConfigManager::nomake() const
{
    return _nomake;
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

void ConfigManager::readConfig(const QString &path, const QString &moduleName)
{
    JsonConfig c(path, moduleName);
    if (!c.isValid()) {
//        qDebug() << path << "is not valid;";
        return;
    }
    auto o = c.subConfigs();
    if (o.size())
        foreach (QString sub, o) {
            readConfig(path + sub, moduleName);
        }

    _options.append(c.options());
    _features.append(c.features());
}

QStringList ConfigManager::licenses() const
{
    return _licenses;
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
            readConfig(m_sourcePath + module + "/", module);
    emit configuresUpdated();
}

void ConfigManager::setSourcePath(QString sourcePath)
{
    if (m_sourcePath == sourcePath)
        return;

    if (sourcePath.endsWith("/"))
        m_sourcePath = sourcePath;
    else
        m_sourcePath = sourcePath + "/";

    // Read modules
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
    emit modulesUpdated();
    setSelectedModules(_submodules);

    // Read platforms
    QDir mkspec(m_sourcePath + "qtbase/mkspecs/");
    _platforms = mkspec.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    emit platformsUpdated();

    // Read licenses
    QDir licenses(m_sourcePath);
    auto licFiles = licenses.entryList(QStringList() << "LICENSE.*");
    foreach (QString l, licFiles)
        _licenses.append(l.replace("LICENSE.", ""));
    emit licensesUpdated();

    emit sourcePathChanged(m_sourcePath);
}

void ConfigManager::setBuildPath(QString buildPath)
{
    if (m_buildPath == buildPath)
        return;

    if (buildPath.endsWith("/"))
        m_buildPath = buildPath;
    else
        m_buildPath = buildPath + "/";
    // Read configurations
    QFile optFile(m_buildPath);
    if (optFile.open(QIODevice::Text | QIODevice::ReadOnly)) {

        QList<QByteArray> opts = optFile.readAll().split('\n');
        optFile.close();

        _selectedModules = _submodules;
        setConfirmLicense(opts.contains("-confirm-license"));
        setUseCommercial(!opts.contains("-opensource"));
        while (opts.count()) {
            auto ba = opts.takeFirst();

            if (ba.startsWith("-no-feature-"))
                setFeatureState(ba.replace("-no-feature-", ""), Qt::Unchecked);
            if (ba.startsWith("-no-feature-"))
                setFeatureState(ba.replace("-feature-", ""), Qt::Checked);

            if (ba == "-skip") {
                ba = opts.takeFirst();
                _selectedModules.removeOne(ba);
                continue;
            }
            if (ba == "-nomake") {
                ba = opts.takeFirst();
                _nomake.append(ba);
                continue;
            }

        }
    }

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
