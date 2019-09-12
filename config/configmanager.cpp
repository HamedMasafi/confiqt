#include "configmanager.h"
#include "jsonconfig.h"
#include "option.h"
#include <optionsmodel.h>
#include "featuresmodel.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QProcess>
#include <QRegularExpression>
#include <QSysInfo>

QList<Option*> ConfigManager::options() const
{
    return d->options;
}

QList<Feature*> ConfigManager::features() const
{
    return d->features;
}

QStringList ConfigManager::submodules() const
{
    return d->submodules;
}

QStringList ConfigManager::selectedModules() const
{
    return d->selectedModules;
}

void ConfigManager::setSelectedModules(const QStringList &selectedModules)
{
    d->selectedModules = selectedModules;
    process();
}

QStringList ConfigManager::platforms() const
{
    return d->platforms;
}

QStringList ConfigManager::createFeatures() const
{
    QStringList args;
    auto itFeatures = d->featuresStates.begin();
    while (itFeatures != d->featuresStates.end()) {
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
    auto itOptions = d->optionsStates.begin();
    while (itOptions != d->optionsStates.end()) {
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

    auto modules = d->submodules;
    foreach (QString selectedModule, d->selectedModules)
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
    d->featuresModel->setState(featureName, state);
//    d->featuresStates.insert(featureName, state);
}

QVariant ConfigManager::optionState(const QString &name) const
{
    if (d->optionsStates.value(name).isValid())
    qDebug() << "state for" << name <<"is"<<d->optionsStates.value(name);
    return d->optionsStates.value(name);
}

void ConfigManager::setOptionsState(const Option *option, const QVariant &state)
{
//    if (!option) {
//        qDebug() << "The option" << option->name() << "not found!";
//        return;
//    }
//    qDebug() << "setting" << option->name() << ":"<<state << "(" << option->typeString() << ")";

//    if (option->type() == Option::AddString) {
//        auto list = d->optionsStates.value(option->name()).toList();
//        list.append(state);
//        d->optionsStates.insert(option->name(), list);
//    } else {
//        d->optionsStates.insert(option->name(), state);
//    }
    d->optionsModel->setState(option->name(), state);
}

void ConfigManager::clearOptionsStates()
{
    d->optionsStates.clear();
}

void ConfigManager::setOptionsState(const QString &optionName, const QVariant &state)
{
//    setOptionsState(option(optionName), state);
    d->optionsModel->setState(optionName, state);
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
            d->configs.insert(m.captured(1));
        }
    }
}

void ConfigManager::readConfig()
{
    d->configs.clear();
    auto platform = defaultPlatform();
    qmakeConfFile(d->sourcePath + "qtbase/mkspecs/" + platform + "/qmake.conf");
}

bool ConfigManager::confirmLicense() const
{
    return optionState("confirm-license").toBool();
}

bool ConfigManager::isSaveNeeded() const
{
    if (d->sourcePath.isEmpty() || d->buildPath.isEmpty())
        return false;

    if (d->featuresStates.size() || d->optionsStates.size())
        return true;

    return false;
}

bool ConfigManager::hasConfig(const QString &name) const
{
    return d->configs.contains(name);
}

QString ConfigManager::buildPath() const
{
    return d->buildPath;
}

QString ConfigManager::installPath() const
{
    return optionState("prefix").toString();
}

QProcess *ConfigManager::createProcess() const
{
    auto p = new QProcess;
    p->setProgram(d->sourcePath + "configure");
    p->setArguments(createCommand());
    p->setWorkingDirectory(d->buildPath);
    return p;
}

void ConfigManager::clearFeatureStates()
{
    d->featuresStates.clear();
}

Qt::CheckState ConfigManager::featureState(const QString &featureName) const
{
    if (d->featuresStates.contains(featureName))
        return d->featuresStates.value(featureName);
    return Qt::PartiallyChecked;
}

QVariantList ConfigManager::nomake() const
{
    return optionState("nomake").toList();
}

QStringList ConfigManager::devices() const
{
    return d->devices;
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

    d->options.append(c.options());
    d->features.append(c.features());
}

void ConfigManager::importSettings()
{
    // Read configurations
    QFile optFile(d->buildPath + "config.opt");
    if (optFile.open(QIODevice::Text | QIODevice::ReadOnly)) {

        QByteArrayList opts = optFile.readAll().split('\n');
        optFile.close();

        d->selectedModules = d->submodules;

        while (opts.count()) {
            auto ba = opts.takeFirst();
            if (ba.isEmpty())
                continue;

            if (!ba.startsWith("-")) {
                qDebug() << "error in line:" << ba;
            }
            ba = ba.remove(0, 1);

            if (ba.startsWith("no-feature-")) {
                setFeatureState(ba.remove(0, 11), Qt::Unchecked);
                continue;
            }
            if (ba.startsWith("feature-")) {
                setFeatureState(ba.remove(0, 8), Qt::Checked);
                continue;
            }

            if (ba == "skip") {
                d->selectedModules.removeOne(opts.takeFirst());
                continue;
            }

            Option *o = nullptr;

            QString val;
            if (!o)
                o = optionByOpt(ba, val);

            if (o){
                QVariant var;
                o->readCommandLine(val, var, opts);
                if (var.isValid())
                    setOptionsState(o, var);
            } else {
                qWarning() << "Unknown parametere:" << ba;
            }
        }

        emit configuresUpdated();
    }
}

void ConfigManager::deleteSettings()
{
    QDir dir(d->buildPath);

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
    foreach(Option *o, d->options)
        if (o->name() == name)
            return o;
    return nullptr;
}

Option *ConfigManager::optionByOpt(const QString &opt, QString &val)
{
    foreach(Option *o, d->options) {
        if (opt == o->name())
            return o;
        if (opt.endsWith(o->name())) {
            val = opt.mid(0, opt.length() - o->name().length() - 1 /* the One if for ending dash */ );
            return o;
        }
    }
    return nullptr;
}

Feature *ConfigManager::feature(const QString &name)
{
    foreach(Feature *f, d->features)
        if (f->name == name)
            return f;
    return nullptr;
}

QStringList ConfigManager::licenses() const
{
    return d->licenses;
}

ConfigManager::ConfigManager() : d(new ConfigData)
{
    d->optionsModel = new OptionsModel(this);
    d->featuresModel = new FeaturesModel(this);
}

QString ConfigManager::sourcePath() const
{
    return d->sourcePath;
}

void ConfigManager::process()
{
    d->options.clear();
    d->features.clear();
    foreach (QString module, d->selectedModules)
        if (d->submodules.contains(module))
            readConfig(d->sourcePath + module + "/", module);
    emit configuresUpdated();
}

OptionsModel *ConfigManager::optionsModel() const
{
    return d->optionsModel;
}

FeaturesModel *ConfigManager::featuresModel() const
{
    return d->featuresModel;
}

bool ConfigManager::save(const QString &path, const ConfigManager::SaveParametereType &params)
{
    QStringList opts;
    QString outPath;
    if (path == QString())
        outPath = d->buildPath + "config.opt";
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
        QFile statusFile(d->buildPath + "config.status");
        if (!statusFile.open(QIODevice::Text | QIODevice::WriteOnly))
            return false;

        QString redoCommand = QString("#!/bin/sh\nexec %1configure -redo \"$@\"")
                .arg(d->sourcePath);
        statusFile.write(redoCommand.toLatin1());
        statusFile.close();

    }

    return true;
}

void ConfigManager::setSourcePath(QString sourcePath)
{
    if (!sourcePath.endsWith("/"))
        sourcePath .append("/");

    if (d->sourcePath == sourcePath)
        return;

    clear();
    d->sourcePath = sourcePath;

    // Read modules
    QByteArray b = readFile(d->sourcePath + ".gitmodules");
    QRegularExpression r("\\[submodule \"(\\w+)\"\\]");
    d->submodules.clear();
    QRegularExpressionMatchIterator it = r.globalMatch(b);
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        QString proFilePath = QString("%1/%2/%2.pro")
                .arg(d->sourcePath, match.captured(1));
        if (QFile::exists(proFilePath))
            d->submodules.append(match.captured(1));
    }
    emit modulesUpdated();
    setSelectedModules(d->submodules);

    // Read platforms
    QDir mkspec(d->sourcePath + "qtbase/mkspecs/");
    d->platforms = platformsInDir(d->sourcePath + "qtbase/mkspecs/");
    d->devices = platformsInDir(d->sourcePath + "qtbase/mkspecs/devices/");
    emit platformsUpdated();

    // Read licenses
    QDir licenses(d->sourcePath);
    auto licFiles = licenses.entryList(QStringList() << "LICENSE.*");
    foreach (QString l, licFiles)
        d->licenses.append(l.replace("LICENSE.", ""));
    emit licensesUpdated();

    readConfig();

    emit sourcePathChanged(d->sourcePath);
}

void ConfigManager::setBuildPath(QString buildPath)
{
    if (!buildPath.endsWith("/"))
        buildPath.append("/");

    if (d->buildPath == buildPath)
        return;

    d->buildPath = buildPath;

    emit buildPathChanged(d->buildPath);
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
    qDeleteAll(d->options.begin(), d->options.end());
    qDeleteAll(d->features.begin(), d->features.end());

    d->options.clear();
    d->features.clear();
    d->platforms.clear();
    d->devices.clear();
    d->submodules.clear();
    d->licenses.clear();
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
