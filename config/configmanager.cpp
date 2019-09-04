#include "configmanager.h"
#include "jsonconfig.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QProcess>
#include <QRegularExpression>

QList<Option*> ConfigManager::options() const
{
    return _options;
}

QList<Feature*> ConfigManager::features() const
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

QStringList ConfigManager::createFeatures() const
{
    QStringList args;
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
    return args;
}

QStringList ConfigManager::createLibs() const
{
    QStringList args;
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

QStringList ConfigManager::createCommons() const
{
    QStringList args;
    args << "-prefix "
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

    return args;
}

QStringList ConfigManager::createCommand() const
{
    QStringList args;


    args.append(createCommons());

    foreach (QString nm, _nomake)
        args << "-nomake"
             << nm;

    args.append(createFeatures());
    args.append(createLibs());

    return args;
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

bool ConfigManager::isSaveNeeded() const
{
    if (m_sourcePath.isEmpty() || m_buildPath.isEmpty())
        return false;

    if (_featuresStates.size() || _optionsStates.size())
        return true;

    return false;
}

QString ConfigManager::buildPath() const
{
    return m_buildPath;
}

QString ConfigManager::installPath() const
{
    return m_installPath;
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

QStringList ConfigManager::devices() const
{
    return _devices;
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

void ConfigManager::importSettings()
{
    // Read configurations
    QFile optFile(m_buildPath + "config.opt");
    qDebug() << "Path="<< m_buildPath + "config.opt";
    if (optFile.open(QIODevice::Text | QIODevice::ReadOnly)) {

        QList<QByteArray> opts = optFile.readAll().split('\n');
        optFile.close();

        _selectedModules = _submodules;
        setConfirmLicense(opts.contains("-confirm-license"));
        setUseCommercial(!opts.contains("-opensource"));
        while (opts.count()) {
            auto ba = opts.takeFirst();

            if (ba == "-prefix") {
                setInstallPath(opts.takeFirst());
                continue;
            }

            if (ba.isEmpty())
                continue;

            if (ba.startsWith("-no-feature-")) {
                setFeatureState(ba.replace("-no-feature-", ""), Qt::Unchecked);
                continue;
            }
            if (ba.startsWith("-feature-")) {
                setFeatureState(ba.replace("-feature-", ""), Qt::Checked);
                continue;
            }

            if (ba == "-skip") {
                ba = opts.takeFirst();
                _selectedModules.removeOne(ba);
                continue;
            }
//            if (ba == "-nomake") {
//                ba = opts.takeFirst();
//                _nomake.append(ba);
//                continue;
//            }

            QString val;
            Option *o = optionByOpt(ba, val);
            if (!o->name.isEmpty()){
                if (o->type == Option::AddString) {
                    qDebug() << o->name << "need next line";
                    opts.takeFirst();
                }
            } else {
                qDebug() << ba << "not found";
            }
        }
    }
}

void ConfigManager::deleteSettings()
{
    QDir dir(m_buildPath);

    dir.setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::Hidden);
    foreach( QString dirItem, dir.entryList() )
        dir.remove( dirItem );

    dir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    foreach( QString dirItem, dir.entryList() )
    {
        QDir subDir(dir.absoluteFilePath(dirItem));
        subDir.removeRecursively();
    }
}

Option *ConfigManager::option(const QString &name)
{
    foreach(Option *o, _options)
        if (o->name == name)
            return o;
    return nullptr;
}

Option *ConfigManager::optionByOpt(const QString &opt, QString &val)
{
    foreach(Option *o, _options) {
        if (opt == o->name)
            return o;
        if (opt.endsWith(o->name)) {
            val = opt;
            val = val.replace(o->name, "");
            val = val.mid(1, val.length() - 2);
            if (!val.contains("-"))
                return o;
        }
    }
    return nullptr;
}

Feature *ConfigManager::feature(const QString &name)
{
    foreach(Feature *f, _features)
        if (f->name == name)
            return f;
    return nullptr;
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
    _options.clear();
    _features.clear();
    foreach (QString module, _selectedModules)
        if (_submodules.contains(module))
            readConfig(m_sourcePath + module + "/", module);
    emit configuresUpdated();
}

bool ConfigManager::save(const QString &path, const ConfigManager::SaveParametereType &params)
{
    QStringList opts;
    QString outPath;
    if (path == QString())
        outPath = m_buildPath + "config.opt";
    else
        outPath = path;

    QStringList args;

    if (params & Commons) args.append(createCommons());
    if (params & Features) args.append(createFeatures());
    if (params & Options) args.append(createLibs());

    QFile optFile(outPath);
    if (!optFile.open(QIODevice::Text | QIODevice::WriteOnly))
        return false;

    foreach (QString o, args)
        optFile.write((o + "\n").toLocal8Bit());
    optFile.close();

    if (path == QString()) {
        QFile statusFile(m_buildPath + "config.status");
        if (!statusFile.open(QIODevice::Text | QIODevice::WriteOnly))
            return false;

        QString redoCommand = QString("#!/bin/sh\nexec %1configure -redo \"$@\"")
                .arg(m_sourcePath);
        statusFile.write(redoCommand.toLatin1());
        statusFile.close();

    }

    return true;
}

void ConfigManager::setSourcePath(QString sourcePath)
{
    if (!sourcePath.endsWith("/"))
        sourcePath .append("/");

    if (m_sourcePath == sourcePath)
        return;

    clear();
    m_sourcePath = sourcePath;

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
    _platforms = platformsInDir(m_sourcePath + "qtbase/mkspecs/");
    _devices = platformsInDir(m_sourcePath + "qtbase/mkspecs/devices/");
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
    if (!buildPath.endsWith("/"))
        buildPath.append("/");

    if (m_buildPath == buildPath)
        return;

    m_buildPath = buildPath;

    emit buildPathChanged(m_buildPath);
}

void ConfigManager::setInstallPath(QString installPath)
{
    if (!installPath.endsWith("/"))
        installPath.append("/");

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

void ConfigManager::clear()
{
    qDeleteAll(_options.begin(), _options.end());
    qDeleteAll(_features.begin(), _features.end());

    _options.clear();
    _features.clear();
    _platforms.clear();
    _devices.clear();
    _submodules.clear();
    _licenses.clear();
}

QStringList ConfigManager::platformsInDir(const QString &path)
{
    QStringList ret;
    QDir mkspec(path);
    auto dirs = mkspec.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (auto d, dirs) {
        if (QFile::exists(path + "/" + d + "/qmake.conf"))
            ret.append(d);
    }
    return ret;
}
