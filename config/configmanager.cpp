#include "configmanager.h"
#include "jsonconfig.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QProcess>
#include <QRegularExpression>
#include <QSysInfo>

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

QStringList ConfigManager::createOptions() const
{
    QStringList args;
    auto itOptions = _optionsStates.begin();
    while (itOptions != _optionsStates.end()) {
        if (itOptions.value() != QVariant()) {
            auto opt = option(itOptions.key());

            if (opt) {
                auto tmp = opt->applyValue(itOptions.value());
                if (tmp != QStringList())
                    args << tmp;
            }
        }
        ++itOptions;
    }
    return args;
}

QStringList ConfigManager::createCommons() const
{
    QStringList args;

    auto modules = _submodules;
    foreach (QString selectedModule, _selectedModules)
        modules.removeAll(selectedModule);

    foreach (QString module, modules)
        args << "-skip"
             << module;

    return args;
}

QStringList ConfigManager::createCommand(const ConfigManager::SaveParametereType &params) const
{
    QStringList args;

    if (params & Commons) args.append(createCommons());
    if (params & Features) args.append(createFeatures());
    if (params & Options) args.append(createOptions());

    return args;
}

void ConfigManager::setFeatureState(const QString &featureName, const Qt::CheckState &state)
{
    _featuresStates.insert(featureName, state);
}

QVariant ConfigManager::optionState(const QString &name) const
{
    return _optionsStates.value(name);
}

void ConfigManager::clearOptionsStates()
{
    _optionsStates.clear();
}

void ConfigManager::setOptionsState(const QString &optionName, const QVariant &state)
{
    _optionsStates.insert(optionName, state);
}

QString ConfigManager::defaultPlatform() const
{
    QString platform_notes;
    return defaultPlatform(platform_notes);
}

QString ConfigManager::defaultPlatform(QString &platform_notes) const
{
    QString spec;

#if defined (Q_OS_WIN)
    auto paths = qEnvironmentVariable("PATH").split(":");
    std::function<bool(const QString&)> isFilePresent = [&paths](const QString &fileName) {
        foreach (QString p, paths)
            if (QFile::exists(p + "/" + fileName))
                return true;
        return false;
    };


    if (isFilePresent("%icl.exe%"))
        spec = "win32-icc";
    else if (isFilePresent("cl.exe"))
        spec = "win32-msvc";
    else if (isFilePresent("clang-cl.exe"))
        spec = "win32-clang-msvc";
    else if (isFilePresent("clang.exe"))
        spec = "win32-clang-g++";
    else if (isFilePresent("g++.exe"))
        spec = "win32-g++";

#elif defined (Q_OS_Darwin)
    spec = "macx-clang";
#elif defined (Q_OS_AIX)
    platform_notes="AIX: aix-g++ aix-g++-64"
    spec = "aix-g++";
#elif defined (Q_OS_GNU)
    spec = "hurd-g++";
#elif defined (Q_OS_FREEBSD)
    auto versionParts = QSysInfo::productVersion().split(".");
    if (!versionParts.length())
        spec = QString();

    if (versionParts.at(0).toInt() >= 10) {
        platform_notes = "freebsd-g++";
        spec = "freebsd-clang";
    } else {
        platform_notes = "freebsd-clang";
        spec = "freebsd-g++";
    }
#elif defined (Q_OS_OPENBSD)
    spec = "openbsd-g++";
#elif defined (Q_OS_NETBSD)
    spec = "netbsd-g++";
#elif defined (Q_OS_HPUS)
    if (QSysInfo::currentCpuArchitecture().contains(QLatin1String("64")))
        spec = "hpuxi-g++-64";
#elif defined (Q_OS_LINUX)
    platform_notes = "linux-clang linux-icc";
    spec = "linux-g++";
#elif defined (Q_OS_SOLARIS)
    platform_notes = "solaris-g++-64 solaris-cc-64";
    spec = "solaris-cc";
#elif defined (Q_OS_CYGWIN)
    spec = "cygwin-g++";
#elif defined (Q_OS_LYNX)
    spec = "lynxos-g++";
#elif defined (Q_OS_QNX)
    spec = "unsupported/qnx-g++";
#endif
    return spec;
}

void ConfigManager::qmakeConfFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        qDebug() << "Unable to open file:"<<file.fileName();
        return;
    }

    auto lines = file.readAll().split('\n');
    QRegularExpression rInclude("include\\((\\S+)\\)");
    QRegularExpression rVariable("QMAKE(\\S+)\\s*\\=\\s*(\\S+)");
    foreach (auto line, lines) {
        auto m = rInclude.match(line);
        if (m.hasMatch()) {
            qmakeConfFile(QFileInfo(file).absolutePath() + "/" + m.captured(1));
            continue;
        }
        m = rVariable.match(line);
        if (m.hasMatch()) {
            _configs.insert(m.captured(1));
        }
    }
}

void ConfigManager::readConfig()
{
    _configs.clear();
    auto platform = defaultPlatform();
    qmakeConfFile(m_sourcePath + "qtbase/mkspecs/" + platform + "/qmake.conf");
}

bool ConfigManager::confirmLicense() const
{
    return optionState("confirm-license").toBool();
}

bool ConfigManager::isSaveNeeded() const
{
    if (m_sourcePath.isEmpty() || m_buildPath.isEmpty())
        return false;

    if (_featuresStates.size() || _optionsStates.size())
        return true;

    return false;
}

bool ConfigManager::hasConfig(const QString &name) const
{
    return _configs.contains(name);
}

QString ConfigManager::buildPath() const
{
    return m_buildPath;
}

QString ConfigManager::installPath() const
{
    return optionState("prefix").toString();
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

Qt::CheckState ConfigManager::featureState(const QString &featureName) const
{
    if (_featuresStates.contains(featureName))
        return _featuresStates.value(featureName);
    return Qt::PartiallyChecked;
}

QVariantList ConfigManager::nomake() const
{
    return optionState("nomake").toList();
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
        if (opts.contains("-opensource"))
            setLicenseType(LicenceType::OpenSource);
        else if (opts.contains("-commercial"))
            setLicenseType(LicenceType::Commercial);

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
            if (o){
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

Option *ConfigManager::option(const QString &name) const
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

    QStringList args = createCommand(params);

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

    readConfig();

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
    setOptionsState("prefix", installPath);
}

void ConfigManager::setConfirmLicense(bool confirmLicense)
{
    setOptionsState("confirm-license", confirmLicense);
}

ConfigManager::LicenceType ConfigManager::licenseType() const
{
    if (optionState("opensource").toBool())
        return ConfigManager::LicenceType::OpenSource;
    if (optionState("commercial").toBool())
        return ConfigManager::LicenceType::Commercial;
    return ConfigManager::LicenceType::Unset;
}

void ConfigManager::setLicenseType(const LicenceType &licenseType)
{
    setOptionsState("opensource", licenseType == LicenceType::OpenSource);
    setOptionsState("commercial", licenseType == LicenceType::Commercial);
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
